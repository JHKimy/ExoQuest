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
	// ## Y���� ���� ���� ##
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/ShotGun/SM_ShotGun.SM_ShotGun'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	// MuzzleLocation Scene Component ���� �� �ʱ�ȭ
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// �⺻ ��ġ ���� (�ѱ� �պκ����� ��ġ ����)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 41.f, 10.f)); // X, Y, Z ��ǥ ���� ����


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

#include "DrawDebugHelpers.h" // ����� ���� �Լ� ���

void AShotgun::Fire()
{
    // �߻� ���� ���� üũ �� Ÿ�̸� ����
    if (!bCanFire) return;
    bCanFire = false;
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &AShotgun::ResetFire, FireRateDelay, false);

    //==========================
    // ī�޶󿡼� ��ǥ���� �ľ�
    //==========================
    // ��Ʈ�ѷ��κ��� ī�޶��� ��ġ�� ȸ�� ���� ��������
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

    // ī�޶� �������� ���� Ʈ���̽��� �����Ͽ� ù ��° ��ǥ ��ġ ���
    FVector CameraForwardVector = CameraRotation.Vector();
    FVector TraceStart = CameraLocation;
    FVector TraceEnd = TraceStart + (CameraForwardVector * 10000.f);

    // ù ��° ���� Ʈ���̽� �����Ͽ� �浹 ���� ���
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    FVector TargetLocation = TraceEnd; // Ÿ���� ���� ��� �⺻��
    
    // ECC_Visibility ä�η� �ľ�
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
    {
        TargetLocation = HitResult.Location; // �浹 �� Ÿ�� ��ġ ����
    }



    // =================================
    // �ѱ� ��ġ���� ��ǥ �������� �߻�
    // =================================
    // �ѱ� ��ġ �� �⺻ �߻� ���� ���
    FVector Muzzle = muzzleLocation->GetComponentLocation();
    FVector BaseShootDirection = (TargetLocation - Muzzle).GetSafeNormal();

    // �̸� ������ ���� ���� ������ ����
    TArray<FRotator> RandomSpreadArray;
    RandomSpreadArray.Reserve(Pellets); // �迭 ũ�� �̸� ����
    for (int32 i = 0; i < Pellets; ++i)
    {
        float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
        float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);
        RandomSpreadArray.Emplace(FRotator(RandomPitch, RandomYaw, 0.0f));
    }

    // ============================
    // ���� ȿ���� ����� źȯ �߻�
    // ============================
    // źȯ �߻� �ð�ȭ �� �浹 ó��
    for (const FRotator& RandomSpread : RandomSpreadArray)
    {
        // ������ ȸ������ �������� �߻� ���� ����
        FVector AdjustedShootDirection = RandomSpread.RotateVector(BaseShootDirection);
        FVector FinalTraceEnd = Muzzle + (AdjustedShootDirection * FireRange);

        // ����� ���� �׸���
        DrawDebugLine(GetWorld(), Muzzle, FinalTraceEnd, FColor::Red, false, 2.0f, 0, 1.0f);

        // �� źȯ�� ���� ���� Ʈ���̽� �� ���� ���� ǥ��
        FHitResult PelletHitResult;
        if (GetWorld()->LineTraceSingleByChannel(PelletHitResult, Muzzle, FinalTraceEnd, ECC_Visibility, Params))
        {
            DrawDebugSphere(GetWorld(), PelletHitResult.Location, 5.0f, 12, FColor::Red, false, 2.0f);

            // ���� �¾Ҵ��� üũ �� ó��
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

    //  �ݵ� ũ�� ����
    float VerticalRecoil = FMath::RandRange(recoilVerticalMin, recoilVerticalMax);   // ���� Ƣ�� ����
    float HorizontalRecoil = FMath::RandRange(recoilHorizontalMin, recoilHorizontalMax); // �¿� ��鸲

    //  ���� �������� ȸ�� ����
    PlayerController->AddPitchInput(-VerticalRecoil);
    PlayerController->AddYawInput(HorizontalRecoil);

    // RecoverRecoil();

    ////  �ݵ� ������ ���� Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimer, this, &AShotgun::RecoverRecoil, 0.01f, true);
}

void AShotgun::RecoverRecoil()
{

    float DeltaTime = GetWorld()->GetDeltaSeconds(); // �����Ӻ� DeltaTime ��������
    float RecoverySpeed = 5.0f; // �ݵ� ȸ�� �ӵ� (Ŀ������ ������)

    // ���� ī�޶� ȸ���� ��������
    FRotator CurrentRotation = PlayerController->GetControlRotation();

    //  ������� �Է� ���� (���콺 ������ Ȯ��)
    FVector2D MouseInput;
    PlayerController->GetInputMouseDelta(MouseInput.X, MouseInput.Y);

    // ����ڰ� ���콺�� �����̸� ��� ���� �ߴ�
    if (FMath::Abs(MouseInput.X) > 0.05f || FMath::Abs(MouseInput.Y) > 0.05f)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimer);
        return;
    }

    FRotator NewRotation = CurrentRotation;
    NewRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, recoveryRotator.Pitch, DeltaTime * RecoverySpeed);

    // ȸ�� ����
    PlayerController->SetControlRotation(NewRotation);

    // ȸ�� �Ϸ� �˻� (���� ����ġ�� �����ϸ� Ÿ�̸� ����)
    if (FMath::Abs(CurrentRotation.Pitch - recoveryRotator.Pitch) < 0.1f)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimer);
    }

}