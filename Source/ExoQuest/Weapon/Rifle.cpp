#include "Weapon/Rifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
#include "Camera/CameraComponent.h"           // ī�޶� ������Ʈ

// y���� �չ��� �Ǿ��ִ� ����

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

	// MuzzleLocation Scene Component ���� �� �ʱ�ȭ
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// �⺻ ��ġ ���� (�ѱ� �պκ����� ��ġ ����)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 2.f)); // X, Y, Z ��ǥ ���� ����
	
	// ������
	damage = 5;

    lastFireTime = -fireCooldown; // ó���� �ٷ� �߻� �����ϵ��� �ʱ�ȭ
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARifle::Fire()
{
    // ���� �ð� ��������
    float currentTime = GetWorld()->GetTimeSeconds();

    // ������ �߻� ���� 0.1�ʰ� ������ �ʾ����� �߻����� ����
    if (currentTime - lastFireTime < fireCooldown)
    {
        return;
    }

    lastFireTime = currentTime; // ������ �߻� �ð� ������Ʈ


    // 1. ī�޶� ��ġ�� ���� ��������
    FVector CameraLocation;
    FRotator CameraRotation;
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    }
    else
    {
        return; // �÷��̾� ��Ʈ�ѷ��� ������ �Լ� ����
    }

    // 2. ī�޶� �������� ���� Ʈ���̽� �����Ͽ� ��ǥ ���� ���
    FVector CameraForwardVector = CameraRotation.Vector();
    FVector CameraTraceStart = CameraLocation;
    FVector CameraTraceEnd = CameraTraceStart + (CameraForwardVector * 10000.0f); // 10000 ���� �Ÿ�����

    FHitResult CameraHitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // �ڱ� �ڽ� ����

    FVector TargetLocation = CameraTraceEnd; // �⺻�� ����
    if (GetWorld()->LineTraceSingleByChannel(CameraHitResult, CameraTraceStart, CameraTraceEnd, ECC_Visibility, Params))
    {
        TargetLocation = CameraHitResult.Location; // �浹 ������ ��ǥ ��ġ�� ��
    }

    // 3. �ѱ� ��ġ���� ��ǥ �������� �߻� ����
    FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // �ѱ� ��ġ
    FVector ShootDirection = (TargetLocation - MuzzleLocation).GetSafeNormal(); // �ѱ����� ��ǥ ���������� ���� ����

    FVector MuzzleTraceEnd = MuzzleLocation + (ShootDirection * 5000.0f); // ��ǥ �������� ���� Ʈ���̽�

    // 4. �ѱ����� ��ǥ �������� ���� Ʈ���̽� �� �浹 ó��
    FHitResult MuzzleHitResult;
    bool bMuzzleHit = GetWorld()->LineTraceSingleByChannel(MuzzleHitResult, MuzzleLocation, MuzzleTraceEnd, ECC_Visibility, Params);

    // ����� ���� �׸��� (�ѱ����� ��ǥ ��������)
    DrawDebugLine(
        GetWorld(),
        MuzzleLocation,  // ������ (�ѱ� ��ġ)
        MuzzleTraceEnd,  // ���� (��ǥ ���� �������� 5000 ����)
        FColor::Red,     // �� ����
        false,           // ���� ���� (�Ͻ���)
        1.0f,            // ���� �ð�
        0,               // �β� (����� ������ ä��)
        1.0f             // ���� �β�
    );

    // 5. �浹�� ��� ó��
    if (bMuzzleHit)
    {
        // �浹 ������ ����� �� ǥ��
        DrawDebugPoint(
            GetWorld(),
            MuzzleHitResult.ImpactPoint, // �浹 ����
            10.0f,                       // ���� ũ��
            FColor::Red,                 // ���� ����
            false,                       // �Ͻ��� ǥ��
            1.0f                         // ���� �ð�
        );

        // �Ѿ� ���� ȿ�� ����
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, MuzzleHitResult.ImpactPoint);

        // �浹�� ���Ͱ� ������ Ȯ���ϰ� ������ ó��
        AActor* HitActor = MuzzleHitResult.GetActor();
        if (HitActor)
        {
            // ���� FSM ������Ʈ�� �����ͼ� ������ ó��
            if (UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(HitActor->GetComponentByClass(UEnemyFSM::StaticClass())))
            {
                EnemyFSM->OnDamageProcess();
            }
        }
    }
}


