#include "Weapon/RocketProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� �� ����
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(15.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = collisionComponent;

	// �浹 �̺�Ʈ ����
	collisionComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);

	// �޽� ������Ʈ �߰�
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetupAttachment(RootComponent);
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹�� SphereComponent�� ó��

	// �̵� ������Ʈ �߰�
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);
	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = Speed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->ProjectileGravityScale = 0.0f; // �߷� ����

	// �ӵ�, ������ �ʱ�ȭ
	Speed = 2000.0f;
	Damage = 50.0f;
}

void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ���� ȿ�� ǥ��
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	// ������ ó��
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 200.0f, nullptr, TArray<AActor*>(), this);

	// ���� ����
	Destroy();
}

