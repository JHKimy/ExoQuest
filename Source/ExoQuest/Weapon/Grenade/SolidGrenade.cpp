#include "Weapon/Grenade/SolidGrenade.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASolidGrenade::ASolidGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� �� ����
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	//collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	collisionComponent->SetLinearDamping(0.5f);  // ���� ���� ���� (������ ����� ȿ��)
	collisionComponent->SetAngularDamping(1.0f); // ȸ�� ���� ���� (�ٿ �� ȸ�� ����)
	//collisionComponent->SetSimulatePhysics(true);
	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	//collisionComponent->BodyInstance.SetMassOverride(0.f);

	RootComponent = collisionComponent;
	collisionComponent->OnComponentHit.AddDynamic(this, &ASolidGrenade::OnHit);



	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/SolidGrenade/solidGrenade_new.solidGrenade_new'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->AddRelativeLocation(FVector(0.f, 0.f, -8.f));
	meshComponent->SetupAttachment(RootComponent);


	Speed		= 3500.f;
	maxSpeed	= 3500.f;
	gravity		= 2.f;


	// �̵� ������Ʈ �߰�
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);

	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInitialVelocityInLocalSpace = true;
	movementComponent->ProjectileGravityScale = gravity;

	// �ٿ ���� ���� �߰�
	movementComponent->bShouldBounce = true;  // �ٿ Ȱ��ȭ
	movementComponent->Bounciness = 0.2f; // �ٿ ���� (�⺻��: 0.6~1.0)
	movementComponent->Friction = 0.5f; // ������ ���� (�⺻��: 0.2~0.3)
	movementComponent->BounceVelocityStopSimulatingThreshold = 5.0f; // Ư�� �ӵ� ���Ͽ����� �ٿ ����

}

void ASolidGrenade::BeginPlay()
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

	collisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ASolidGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASolidGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// �������Ʈ�� �����Ǿ� �ִ��� Ȯ��
	if (barrierClass)
	{
		// ���� Grenade ��ġ���� �������Ʈ ����
		GetWorld()->SpawnActor<AActor>(barrierClass, GetActorLocation(), FRotator::ZeroRotator);
	}

	// Grenade ����
	Destroy();
}

