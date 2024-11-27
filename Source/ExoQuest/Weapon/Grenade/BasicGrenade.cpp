#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"


#include "Enemy/EnemyFSM.h" // 데미지 처리


ABasicGrenade::ABasicGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	
	collisionComponent->SetSimulatePhysics(true);

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = collisionComponent;

	// 충돌 이벤트 연결
	collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/SM_Grenade.SM_Grenade'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	//meshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 비활성화
	meshComponent->SetupAttachment(RootComponent);

	// 이동 컴포넌트 추가
	//movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//movementComponent->SetUpdatedComponent(collisionComponent);

	
}

void ABasicGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicGrenade::OnHit
(UPrimitiveComponent* HitComp, 
AActor* OtherActor, UPrimitiveComponent* OtherComp, 
FVector NormalImpulse, 
const FHitResult& Hit)
{
	// 소유자와의 충돌 무시
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Grenade Hit!"));
	}

	// 폭발 효과 표시
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}
	// 디버그 드로잉으로 충돌 확인
	// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.0f, 12, FColor::Red, false, 1.0f);

	// Destroy();
}