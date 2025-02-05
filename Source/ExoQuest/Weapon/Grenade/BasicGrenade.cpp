#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"


#include "Enemy/EnemyFSM.h" // ������ ó��
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Player/EQPlayerController.h"
#include "EngineUtils.h" // TActorIterator ����� ���� ���


ABasicGrenade::ABasicGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü ���� �� ����
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	collisionComponent->SetLinearDamping(0.5f);  // ���� ���� ���� (������ ����� ȿ��)
	collisionComponent->SetAngularDamping(1.0f); // ȸ�� ���� ���� (�ٿ �� ȸ�� ����)
	//collisionComponent->SetSimulatePhysics(true);

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	//collisionComponent->BodyInstance.SetMassOverride(0.f);

	RootComponent = collisionComponent;

	// �浹 �̺�Ʈ ����
	collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/BasicGrenade/SM_Grenade.SM_Grenade'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	//meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ��Ȱ��ȭ
	//meshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� ��Ȱ��ȭ
	meshComponent->SetupAttachment(RootComponent);

	// �̵� ������Ʈ �߰�
	//movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//movementComponent->SetUpdatedComponent(collisionComponent);






	Speed = 3500.f; // �ʱ� �ӵ��� ����
	maxSpeed = 3500.f;
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

	// �ٿ ���� ���� �߰�
	movementComponent->bShouldBounce = true;  // �ٿ Ȱ��ȭ
	movementComponent->Bounciness = 0.2f; // �ٿ ���� (�⺻��: 0.6~1.0)
	movementComponent->Friction = 0.5f; // ������ ���� (�⺻��: 0.2~0.3)
	movementComponent->BounceVelocityStopSimulatingThreshold = 5.0f; // Ư�� �ӵ� ���Ͽ����� �ٿ ����

	// ���� �߻� ������ Y�� �������� ����
	// FVector LaunchDirection = FVector(Speed, 0.0f, 0.0f); // Y���� ���� �߻��ϵ��� ����
	// 1movementComponent->Velocity = LaunchDirection;
	








	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectAsset(TEXT("/Game/Asset/Effect/P_GrenadeExplosion.P_GrenadeExplosion"));
	if (ExplosionEffectAsset.Succeeded())
	{
		explosionEffect = ExplosionEffectAsset.Object;
	}
}

void ABasicGrenade::BeginPlay()
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



	// collisionComponent->BodyInstance.SetMassOverride(1.0f); // ����ź�� ���� ���� (��: 1kg)
}

void ABasicGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ������ �ӵ��� �����Ǿ��� ��� ���� ��� ������Ʈ
	//PredictGrenadePath(LaunchVelocity);
	
	//// ���忡�� �÷��̾� ĳ���͸� ��������
	//ACharacterBase* tempCharacter = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//
	//PredictGrenadePath(tempCharacter->GrenadeLaunchVelocity);
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

	collisionComponent->SetSimulatePhysics(true);


	// ���� �۾��� ���� FLatentActionInfo ����
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // ������ �Ϸ�Ǹ� ȣ���� ��ü
	LatentInfo.ExecutionFunction = FName(TEXT("Explosion")); // ������ �Լ� �̸�
	LatentInfo.Linkage = 0; // ���� ID (�ַ� 0 ���)
	LatentInfo.UUID = __LINE__; // ���� ID (�ַ� �ڵ� ���� ���)

	//// ����� ��������� �浹 Ȯ��
	//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.0f, 12, FColor::Red, false, 1.0f);

	UKismetSystemLibrary::Delay(this, 3.f, LatentInfo);



}

void ABasicGrenade::Explosion()
{
	// ���� ȿ�� ǥ��
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	Destroy();
}

//void ABasicGrenade::PredictGrenadePath(FVector LaunchVelocity, float MaxSimTime)
//{
//	// ����� ������ ����ü
//	FPredictProjectilePathResult PredictResult;
//
//	// ��� ��� ��û �Ķ����
//	FPredictProjectilePathParams Params;
//	Params.StartLocation = GetActorLocation(); // ���� ����ź�� ��ġ
//	Params.LaunchVelocity = LaunchVelocity;   // ���� ���� �ӵ�
//	Params.ProjectileRadius = ProjectileRadius;
//	Params.SimFrequency = SimFrequency;
//	Params.MaxSimTime = MaxSimTime;
//	Params.OverrideGravityZ = GravityZ;
//	Params.TraceChannel = TraceChannel;
//	Params.bTraceWithCollision = true;        // �浹 Ȯ��
//
//
//	// ��� ����
//	if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult))
//	{
//		// ��� ���� �ð�ȭ
//		for (const FPredictProjectilePathPointData& PointData : PredictResult.PathData)
//		{
//			DrawDebugSphere(GetWorld(), PointData.Location, 5.0f, 12, FColor::Green, false, 1.0f);
//		}
//
//		// �浹 ���� �ð�ȭ
//		if (PredictResult.HitResult.IsValidBlockingHit())
//		{
//			DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location, 10.0f, 12, FColor::Red, false, 1.0f);
//		}
//	}
//}