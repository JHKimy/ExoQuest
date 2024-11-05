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
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
    FVector CameraLocation;
    FRotator CameraRotation;
    playerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

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
}



void AShotgun::ResetFire()
{
    bCanFire = true;
}