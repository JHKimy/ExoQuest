#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"


#include "Enemy/EnemyFSM.h" // ������ ó��


ABasicGrenade::ABasicGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� �� ����
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	
	collisionComponent->SetSimulatePhysics(true);

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	RootComponent = collisionComponent;

	// �浹 �̺�Ʈ ����
	collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/SM_Grenade.SM_Grenade'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ
	//meshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� ��Ȱ��ȭ
	meshComponent->SetupAttachment(RootComponent);

	// �̵� ������Ʈ �߰�
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
	// �����ڿ��� �浹 ����
	if (OtherActor == GetOwner())
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Grenade Hit!"));
	}

	// ���� ȿ�� ǥ��
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}
	// ����� ��������� �浹 Ȯ��
	// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.0f, 12, FColor::Red, false, 1.0f);

	// Destroy();
}