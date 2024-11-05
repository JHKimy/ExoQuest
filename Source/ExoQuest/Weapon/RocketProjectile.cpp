#include "Weapon/RocketProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(15.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = collisionComponent;

	// 충돌 이벤트 연결
	collisionComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);

	// 메쉬 컴포넌트 추가
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetupAttachment(RootComponent);
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌은 SphereComponent가 처리

	// 이동 컴포넌트 추가
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);
	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = Speed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->ProjectileGravityScale = 0.0f; // 중력 무시

	// 속도, 데미지 초기화
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
	// 폭발 효과 표시
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	// 데미지 처리
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), 200.0f, nullptr, TArray<AActor*>(), this);

	// 로켓 제거
	Destroy();
}

