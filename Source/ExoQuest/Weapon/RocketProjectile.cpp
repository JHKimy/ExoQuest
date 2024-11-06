#include "Weapon/RocketProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"

#include "EngineUtils.h" // TActorIterator ����� ���� ���

#include "Enemy/EnemyFSM.h" // ������ ó��

// ## ���� Muzzle�� y���� �ٶ󺸰� �����Ƿ� ���� ##

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;


	// �浹ü ���� �� ����
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(15.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = collisionComponent;

	// �浹 �̺�Ʈ ����
	collisionComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);


	// �޽� ������Ʈ �߰�
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RocketLauncher/RocketProjectile/SM_RocketProjectile.SM_RocketProjectile'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	meshComponent->SetRelativeLocation(FVector(0.f, 0.f, -12.f));
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ
	meshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� ��Ȱ��ȭ
	meshComponent->SetupAttachment(RootComponent);


	Speed = 4000.f; // �ʱ� �ӵ��� ����
	maxSpeed = 5000.f;
	gravity = 2.f;
	damage = 50.f;

	// �̵� ������Ʈ �߰�
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);

	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInitialVelocityInLocalSpace = true;
	movementComponent->ProjectileGravityScale = gravity;



	// ���� �߻� ������ Y�� �������� ����
	FVector LaunchDirection = FVector(0.0f, Speed, 0.0f); // Y���� ���� �߻��ϵ��� ����
	movementComponent->Velocity = LaunchDirection;
}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();



	// ī�޶��� ������ ������ �ʱ� �ӵ� ����
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// �߻� ���� ���� (ī�޶� �ٶ󺸴� ����)
		FVector LaunchDirection = CameraRotation.Vector(); // ī�޶� �ٶ󺸴� ������ ���� ����
		movementComponent->Velocity = LaunchDirection * Speed; // �߻� �ӵ� ����
	}



	// ���忡 �ִ� ��� ACharacterBase �ν��Ͻ����� �浹�� �����ϵ��� ����
	for (TActorIterator<ACharacterBase> It(GetWorld()); It; ++It)
	{
		ACharacterBase* CharacterInstance = *It;
		if (CharacterInstance)
		{
			collisionComponent->IgnoreActorWhenMoving(CharacterInstance, true);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(destroyTimerHandle, FTimerDelegate::CreateLambda([this]()->void
		{
			Destroy();
		}), 10.0f, false);
	// 10�� ������ �˾Ƽ� ����
	
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// �����ڿ��� �浹 ����
	if (OtherActor == GetOwner())
	{
		return;
	}

	// ���� ȿ�� ǥ��
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	// ������ ó��
	UGameplayStatics::ApplyRadialDamage(GetWorld(), damage, GetActorLocation(), 200.0f, nullptr, TArray<AActor*>(), this);

	// ���� ����
	Destroy();



	if (UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(OtherActor->GetComponentByClass(UEnemyFSM::StaticClass() ) ))
	{
		EnemyFSM->OnDamageProcess();
	}

}