#include "Weapon/Rifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
#include "Camera/CameraComponent.h"           // 카메라 컴포넌트

// y축이 앞방향 되어있는 에셋

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RIfle/SM_Rifle.SM_Rifle'"));
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
	muzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 2.f)); // X, Y, Z 좌표 조정 가능
	
	// 데미지
	damage = 5;

    lastFireTime = -fireCooldown; // 처음엔 바로 발사 가능하도록 초기화
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));

    PlayerController = GetWorld()->GetFirstPlayerController();

}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARifle::Fire()
{
    // 현재 시간 가져오기
    float currentTime = GetWorld()->GetTimeSeconds();

    // 마지막 발사 이후 0.1초가 지나지 않았으면 발사하지 않음
    if (currentTime - lastFireTime < fireCooldown)
    {
        return;
    }

    lastFireTime = currentTime; // 마지막 발사 시간 업데이트


    // 1. 카메라 위치와 방향 가져오기
    FVector CameraLocation;
    FRotator CameraRotation;
    if (PlayerController)
    {
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    }
    else
    {
        return; // 플레이어 컨트롤러가 없으면 함수 종료
    }

    // 2. 카메라 방향으로 라인 트레이스 실행하여 목표 지점 계산
    FVector CameraForwardVector = CameraRotation.Vector();
    FVector CameraTraceStart = CameraLocation;
    FVector CameraTraceEnd = CameraTraceStart + (CameraForwardVector * 10000.0f); // 10000 유닛 거리까지

    FHitResult CameraHitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신 제외

    FVector TargetLocation = CameraTraceEnd; // 기본값 설정
    if (GetWorld()->LineTraceSingleByChannel(CameraHitResult, CameraTraceStart, CameraTraceEnd, ECC_Visibility, Params))
    {
        TargetLocation = CameraHitResult.Location; // 충돌 지점이 목표 위치가 됨
    }

    // 3. 총구 위치에서 목표 지점으로 발사 설정
    FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // 총구 위치
    FVector ShootDirection = (TargetLocation - MuzzleLocation).GetSafeNormal(); // 총구에서 목표 지점으로의 방향 벡터

    FVector MuzzleTraceEnd = MuzzleLocation + (ShootDirection * 5000.0f); // 목표 지점으로 라인 트레이스

    // 4. 총구에서 목표 지점까지 라인 트레이스 및 충돌 처리
    FHitResult MuzzleHitResult;
    bool bMuzzleHit = GetWorld()->LineTraceSingleByChannel(MuzzleHitResult, MuzzleLocation, MuzzleTraceEnd, ECC_Visibility, Params);

    // 디버그 라인 그리기 (총구에서 목표 지점까지)
    DrawDebugLine(
        GetWorld(),
        MuzzleLocation,  // 시작점 (총구 위치)
        MuzzleTraceEnd,  // 끝점 (목표 지점 방향으로 5000 유닛)
        FColor::Red,     // 선 색상
        false,           // 지속 여부 (일시적)
        1.0f,            // 지속 시간
        0,               // 두께 (디버그 라인의 채널)
        1.0f             // 선의 두께
    );

    // 5. 충돌한 경우 처리
    if (bMuzzleHit)
    {
        // 충돌 지점에 디버그 점 표시
        DrawDebugPoint(
            GetWorld(),
            MuzzleHitResult.ImpactPoint, // 충돌 지점
            10.0f,                       // 점의 크기
            FColor::Red,                 // 점의 색상
            false,                       // 일시적 표시
            1.0f                         // 지속 시간
        );

        // 총알 파편 효과 생성
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffect, MuzzleHitResult.ImpactPoint);

        // 충돌한 액터가 적인지 확인하고 데미지 처리
        AActor* HitActor = MuzzleHitResult.GetActor();
        if (HitActor)
        {
            // 적의 FSM 컴포넌트를 가져와서 데미지 처리
            if (UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(HitActor->GetComponentByClass(UEnemyFSM::StaticClass())))
            {
                EnemyFSM->OnDamageProcess();
            }
        }
    }

    if (PlayerController && cameraShakeClass)
    {
        PlayerController->PlayerCameraManager->StartCameraShake(cameraShakeClass);
    }


    // 반동 효과 
    ApplyRecoil();
}

void ARifle::ApplyRecoil()
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
    GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimer, this, &ARifle::RecoverRecoil, 0.01f, true);
}

void ARifle::RecoverRecoil()
{

    float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임별 DeltaTime 가져오기
    float RecoverySpeed = 1.0f; // 반동 회복 속도 (커질수록 빨라짐)

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

