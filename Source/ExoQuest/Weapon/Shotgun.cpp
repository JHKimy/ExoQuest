#include "Weapon/Shotgun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"

AShotgun::AShotgun()
{
	PrimaryActorTick.bCanEverTick = true;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// ## Y축이 앞을 향함 ##
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/ShotGun/SM_ShotGun.SM_ShotGun'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	// MuzzleLocation Scene Component 생성 및 초기화
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// 기본 위치 설정 (총구 앞부분으로 위치 설정)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 41.f, 10.f)); // X, Y, Z 좌표 조정 가능


}

// Called when the game starts or when spawned
void AShotgun::BeginPlay()
{
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));

    PlayerController = GetWorld()->GetFirstPlayerController();

}

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#include "DrawDebugHelpers.h" // 디버그 관련 함수 헤더

void AShotgun::Fire()
{
    // 발사 가능 여부 체크 및 타이머 설정
    if (!bCanFire) return;
    bCanFire = false;
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AShotgun::ResetFire, FireRateDelay, false);

    //==========================
    // 카메라에서 목표지점 파악
    //==========================
    // 컨트롤러로부터 카메라의 위치와 회전 정보 가져오기
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // 카메라 방향으로 라인 트레이스를 설정하여 첫 번째 목표 위치 계산
    FVector CameraForwardVector = CameraRotation.Vector();
    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraForwardVector * 10000.f);

    // 첫 번째 라인 트레이스 실행하여 충돌 지점 계산
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    FVector TargetLocation = TraceEnd; // 타겟이 없을 경우 기본값
    
    // ECC_Visibility 채널로 파악
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
    {
        TargetLocation = HitResult.Location; // 충돌 시 타겟 위치 설정
    }



    // =================================
    // 총구 위치에서 목표 지점으로 발사
    // =================================
    // 총구 위치 및 기본 발사 방향 계산
    FVector Muzzle = muzzleLocation->GetComponentLocation();
    FVector BaseShootDirection = (TargetLocation - Muzzle).GetSafeNormal();

    // 미리 생성한 랜덤 퍼짐 각도를 저장
    TArray<FRotator> RandomSpreadArray;
    RandomSpreadArray.Reserve(Pellets); // 배열 크기 미리 설정
    for (int32 i = 0; i < Pellets; ++i)
    {
        float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
        float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);
        RandomSpreadArray.Emplace(FRotator(RandomPitch, RandomYaw, 0.0f));
    }

    // ============================
    // 퍼짐 효과가 적용된 탄환 발사
    // ============================
    // 탄환 발사 시각화 및 충돌 처리
    for (const FRotator& RandomSpread : RandomSpreadArray)
    {
        // 무작위 회전값을 기준으로 발사 방향 설정
        FVector AdjustedShootDirection = RandomSpread.RotateVector(BaseShootDirection);
        FVector FinalTraceEnd = Muzzle + (AdjustedShootDirection * FireRange);

        // 디버그 라인 그리기
        DrawDebugLine(GetWorld(), Muzzle, FinalTraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

        // 각 탄환에 대해 라인 트레이스 및 맞은 지점 표시
        FHitResult PelletHitResult;
        if (GetWorld()->LineTraceSingleByChannel(PelletHitResult, Muzzle, FinalTraceEnd, ECC_Visibility, Params))
        {
            DrawDebugSphere(GetWorld(), PelletHitResult.Location, 5.0f, 12, FColor::Red, false, 2.0f);

            // 적이 맞았는지 체크 및 처리
            AActor* HitActor = PelletHitResult.GetActor();
            if (HitActor && HitActor->GetComponentByClass(UEnemyFSM::StaticClass()))
            {
                UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(HitActor->GetComponentByClass(UEnemyFSM::StaticClass()));
                if (EnemyFSM)
                {
                    EnemyFSM->OnDamageProcess();
                }
            }
        }
    }
    if (PlayerController && cameraShakeClass)
    {
        PlayerController->PlayerCameraManager->StartCameraShake(cameraShakeClass);
    }


    ApplyRecoil();
}



void AShotgun::ResetFire()
{
    bCanFire = true;
}

void AShotgun::ApplyRecoil()
{
    recoveryRotator = PlayerController->GetControlRotation();

    //  반동 크기 설정
    float VerticalRecoil = FMath::RandRange(recoilVerticalMin, recoilVerticalMax);   // 위로 튀는 정도
    float HorizontalRecoil = FMath::RandRange(recoilHorizontalMin, recoilHorizontalMax); // 좌우 흔들림

    //  현재 시점에서 회전 적용
    PlayerController->AddPitchInput(-VerticalRecoil);
    PlayerController->AddYawInput(HorizontalRecoil);

    // RecoverRecoil();

    ////  반동 복구를 위한 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimer, this, &AShotgun::RecoverRecoil, 0.01f, true);
}

void AShotgun::RecoverRecoil()
{

    float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임별 DeltaTime 가져오기
    float RecoverySpeed = 5.0f; // 반동 회복 속도 (커질수록 빨라짐)

    // 현재 카메라 회전값 가져오기
    FRotator CurrentRotation = PlayerController->GetControlRotation();

    //  사용자의 입력 감지 (마우스 움직임 확인)
    FVector2D MouseInput;
    PlayerController->GetInputMouseDelta(MouseInput.X, MouseInput.Y);

    // 사용자가 마우스를 움직이면 즉시 복구 중단
    if (FMath::Abs(MouseInput.X) > 0.05f || FMath::Abs(MouseInput.Y) > 0.05f)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimer);
        return;
    }

    FRotator NewRotation = CurrentRotation;
    NewRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, recoveryRotator.Pitch, DeltaTime * RecoverySpeed);

    // 회전 적용
    PlayerController->SetControlRotation(NewRotation);

    // 회복 완료 검사 (거의 원위치에 도달하면 타이머 종료)
    if (FMath::Abs(CurrentRotation.Pitch - recoveryRotator.Pitch) < 0.1f)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimer);
    }

}