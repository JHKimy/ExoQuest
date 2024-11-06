#include "Weapon/RocketProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"

#include "EngineUtils.h" // TActorIterator 사용을 위한 헤더

#include "Enemy/EnemyFSM.h" // 데미지 처리

// ## 총의 Muzzle이 y축을 바라보고 있으므로 변경 ##

ARocketProjectile::ARocketProjectile()
{
	PrimaryActorTick.bCanEverTick = true;


	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(15.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	RootComponent = collisionComponent;

	// 충돌 이벤트 연결
	collisionComponent->OnComponentHit.AddDynamic(this, &ARocketProjectile::OnHit);


	// 메쉬 컴포넌트 추가
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RocketLauncher/RocketProjectile/SM_RocketProjectile.SM_RocketProjectile'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	meshComponent->SetRelativeLocation(FVector(0.f, 0.f, -12.f));
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	meshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 비활성화
	meshComponent->SetupAttachment(RootComponent);


	Speed = 4000.f; // 초기 속도를 설정
	maxSpeed = 5000.f;
	gravity = 2.f;
	damage = 50.f;

	// 이동 컴포넌트 추가
	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	movementComponent->SetUpdatedComponent(collisionComponent);

	movementComponent->InitialSpeed = Speed;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->bRotationFollowsVelocity = true;
	movementComponent->bInitialVelocityInLocalSpace = true;
	movementComponent->ProjectileGravityScale = gravity;



	// 로켓 발사 방향을 Y축 방향으로 설정
	FVector LaunchDirection = FVector(0.0f, Speed, 0.0f); // Y축을 따라 발사하도록 설정
	movementComponent->Velocity = LaunchDirection;
}

void ARocketProjectile::BeginPlay()
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



	// 월드에 있는 모든 ACharacterBase 인스턴스와의 충돌을 무시하도록 설정
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
	// 10초 지나면 알아서 삭제
	
}

void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// 소유자와의 충돌 무시
	if (OtherActor == GetOwner())
	{
		return;
	}

	// 폭발 효과 표시
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	// 데미지 처리
	UGameplayStatics::ApplyRadialDamage(GetWorld(), damage, GetActorLocation(), 200.0f, nullptr, TArray<AActor*>(), this);

	// 로켓 제거
	Destroy();



	if (UEnemyFSM* EnemyFSM = Cast<UEnemyFSM>(OtherActor->GetComponentByClass(UEnemyFSM::StaticClass() ) ))
	{
		EnemyFSM->OnDamageProcess();
	}

}