#include "Weapon/Grenade/SolidGrenade.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


ASolidGrenade::ASolidGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	//collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	collisionComponent->SetLinearDamping(0.5f);  // 선형 감속 증가 (마찰력 비슷한 효과)
	collisionComponent->SetAngularDamping(1.0f); // 회전 감속 증가 (바운스 시 회전 멈춤)
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


	// 이동 컴포넌트 추가
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);

	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInitialVelocityInLocalSpace = true;
	movementComponent->ProjectileGravityScale = gravity;

	// 바운스 관련 설정 추가
	movementComponent->bShouldBounce = true;  // 바운스 활성화
	movementComponent->Bounciness = 0.2f; // 바운스 강도 (기본값: 0.6~1.0)
	movementComponent->Friction = 0.5f; // 마찰력 증가 (기본값: 0.2~0.3)
	movementComponent->BounceVelocityStopSimulatingThreshold = 5.0f; // 특정 속도 이하에서는 바운스 멈춤

}

void ASolidGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	// 카메라의 방향을 가져와 초기 속도 설정
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 발사 방향 설정 (카메라가 바라보는 방향)
		FVector LaunchDirection = CameraRotation.Vector(); // 카메라가 바라보는 방향의 단위 벡터
		movementComponent->Velocity = LaunchDirection * Speed; // 발사 속도 설정
	}

	collisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ASolidGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ASolidGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 블루프린트가 설정되어 있는지 확인
	if (barrierClass)
	{
		// 현재 Grenade 위치에서 블루프린트 스폰
		GetWorld()->SpawnActor<AActor>(barrierClass, GetActorLocation(), FRotator::ZeroRotator);
	}

	// Grenade 제거
	Destroy();
}

