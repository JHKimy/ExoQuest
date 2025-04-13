#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"

#include "Enemy/EnemyFSM.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Player/EQPlayerController.h"
#include "EngineUtils.h"

ABasicGrenade::ABasicGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionComponent->SetCollisionObjectType(ECC_PhysicsBody);
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	collisionComponent->SetLinearDamping(0.5f);
	collisionComponent->SetAngularDamping(1.0f);
	RootComponent = collisionComponent;

	collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/BasicGrenade/SM_Grenade.SM_Grenade'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetupAttachment(RootComponent);
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComponent->SetSimulatePhysics(false);

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);
	movementComponent->InitialSpeed = 3000.f;
	movementComponent->MaxSpeed = 3000.f;
	movementComponent->ProjectileGravityScale = 2.0f;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInitialVelocityInLocalSpace = true;
	movementComponent->bShouldBounce = true;
	movementComponent->Bounciness = 0.5f;
	movementComponent->Friction = 0.5f;
	movementComponent->BounceVelocityStopSimulatingThreshold = 5.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectAsset(TEXT("/Game/Asset/Effect/P_GrenadeExplosion.P_GrenadeExplosion"));
	if (ExplosionEffectAsset.Succeeded())
	{
		explosionEffect = ExplosionEffectAsset.Object;
	}
}

void ABasicGrenade::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector LaunchDirection = CameraRotation.Vector();
		movementComponent->Velocity = LaunchDirection * movementComponent->InitialSpeed;
	}

	for (TActorIterator<ACharacterBase> It(GetWorld()); It; ++It)
	{
		ACharacterBase* CharacterInstance = *It;
		if (CharacterInstance)
		{
			collisionComponent->IgnoreActorWhenMoving(CharacterInstance, true);
		}
	}
}

void ABasicGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner()) return;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName(TEXT("Explosion"));
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;

	UKismetSystemLibrary::Delay(this, 3.f, LatentInfo);
}

void ABasicGrenade::Explosion()
{
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}
	// 범위 데미지
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	//  디버그용 폭발 반경 표시 (녹색 구)
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		explosionRadius,   // 반지름
		32,                // 세그먼트 (많을수록 부드러움)
		FColor::Green,
		false,             // 지속 여부 (false면 일정 시간 뒤 사라짐)
		2.0f               // 표시 시간 (초)
	);
	UGameplayStatics::ApplyRadialDamage(
		this,
		damage,
		GetActorLocation(),
		explosionRadius,
		nullptr, // DamageType
		IgnoredActors,
		this,
		GetInstigatorController(), // 공격자 컨트롤러
		true  // Do full damage
	);
	Destroy();
}