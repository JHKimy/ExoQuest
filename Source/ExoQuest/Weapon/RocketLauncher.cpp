#include "Weapon/RocketLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
//#include "RocketProjectile.h" // ARocketProjectile Ŭ���� ����
#include "Weapon/RocketProjectile.h"

#include "DrawDebugHelpers.h"


ARocketLauncher::ARocketLauncher()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RocketLauncher/SM_RocketLauncher.SM_RocketLauncher'"));
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
	muzzleLocation->SetRelativeLocation(FVector(0.f, 60.f, 2.f)); // X, Y, Z ��ǥ ���� ����

    // �������Ʈ �������̵�
    // RocketProjectileClass �ʱ�ȭ
    static ConstructorHelpers::FClassFinder<ARocketProjectile> RocketProjectileBP(TEXT("/Game/BluePrint/Weapon/BP_RocketProjectile.BP_RocketProjectile_C"));
    if (RocketProjectileBP.Succeeded())
    {
        RocketProjectileClass = RocketProjectileBP.Class;
    }

    bCanFire = true; // ���� �� �߻� ���� ���·� ����
}

void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	
    PlayerController = GetWorld()->GetFirstPlayerController();

}

void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �߻� �� ���� ��� �ð�ȭ
	if (bCanFire)
	{
		ShowProjectilePrediction();
	}

}

void ARocketLauncher::Fire()
{
    if (!bCanFire) return;
    
    bCanFire = false; // �߻� �� ��Ÿ�� ���� �߻� �Ұ�

    //UKismetSystemLibrary::PrintString(this, TEXT("Firing Rocket!"), true, true, FColor::Red, 2.0f);

    if (RocketProjectileClass)
    {
        FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // �ѱ� ��ġ
        FRotator MuzzleRotation = muzzleLocation->GetComponentRotation(); // �ѱ� ȸ��

        // ���� �߻�
        ARocketProjectile* Rocket = GetWorld()->SpawnActor<ARocketProjectile>(RocketProjectileClass, MuzzleLocation, MuzzleRotation);

        if (Rocket)
        {
           // UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawned Successfully!"), true, true, FColor::Green, 2.0f);
        }
        else
        {
            //UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawn Failed!"), true, true, FColor::Red, 2.0f);
        }
    }
    else
    {
        //UKismetSystemLibrary::PrintString(this, TEXT("Projectile Class Not Set"), true, true, FColor::Red, 2.0f);
    }

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, this, &ARocketLauncher::ResetFire, FireRateDelay, false);
    
    ApplyRecoil();
}


void ARocketLauncher::ResetFire()
{
    bCanFire = true; // ��Ÿ�� ����, �߻� ���� ���·� ��ȯ
}



void ARocketLauncher::ShowProjectilePrediction()
{
    if (RocketProjectileClass && muzzleLocation)
    {
        FVector StartLocation = muzzleLocation->GetComponentLocation();
        FRotator LaunchRotation;


		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		LaunchRotation = CameraRotation;

        // ī�޶� ���� �Ǵ� �ѱ� ������ ������� �� �ʱ� �ӵ� ����
        FVector LaunchVelocity = LaunchRotation.Vector() * 4000.0f; // ���� �߻� �ӵ��� ��ġ�ϴ� �ʱ� �ӵ� ����

        // FPredictProjectilePathParams ����
        FPredictProjectilePathParams PredictParams;
        PredictParams.StartLocation = StartLocation;
        PredictParams.LaunchVelocity = LaunchVelocity;
        PredictParams.bTraceWithCollision = true;
        PredictParams.ProjectileRadius = 15.0f; // �߻�ü ũ��� ��ġ�ϵ��� ����
        PredictParams.MaxSimTime = 5.0f; // �ùķ��̼� �ð� (��)

        PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ()*2;

        
        PredictParams.SimFrequency = 15.0f;
        PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
        PredictParams.DrawDebugTime = 0.1f;

        FPredictProjectilePathResult PredictResult;
        UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
    }
}

void ARocketLauncher::ApplyRecoil()
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
    GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimer, this, &ARocketLauncher::RecoverRecoil, 0.01f, true);
}

void ARocketLauncher::RecoverRecoil()
{

    float DeltaTime = GetWorld()->GetDeltaSeconds(); // �����Ӻ� DeltaTime ��������
    float RecoverySpeed = 1.0f; // �ݵ� ȸ�� �ӵ� (Ŀ������ ������)

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