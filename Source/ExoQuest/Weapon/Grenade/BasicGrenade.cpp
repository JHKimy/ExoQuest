#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"


#include "Enemy/EnemyFSM.h" // 데미지 처리
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Player/EQPlayerController.h"
#include "EngineUtils.h" // TActorIterator 사용을 위한 헤더


ABasicGrenade::ABasicGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	collisionComponent->InitSphereRadius(10.0f);
	collisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
	collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	collisionComponent->SetLinearDamping(0.5f);  // 선형 감속 증가 (마찰력 비슷한 효과)
	collisionComponent->SetAngularDamping(1.0f); // 회전 감속 증가 (바운스 시 회전 멈춤)
	//collisionComponent->SetSimulatePhysics(true);

	collisionComponent->SetNotifyRigidBodyCollision(true);
	collisionComponent->SetGenerateOverlapEvents(true);
	//collisionComponent->BodyInstance.SetMassOverride(0.f);

	RootComponent = collisionComponent;

	// 충돌 이벤트 연결
	collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/BasicGrenade/SM_Grenade.SM_Grenade'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	//meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	//meshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 비활성화
	meshComponent->SetupAttachment(RootComponent);

	// 이동 컴포넌트 추가
	//movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	//movementComponent->SetUpdatedComponent(collisionComponent);






	Speed = 3500.f; // 초기 속도를 설정
	maxSpeed = 3500.f;
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

	// 바운스 관련 설정 추가
	movementComponent->bShouldBounce = true;  // 바운스 활성화
	movementComponent->Bounciness = 0.2f; // 바운스 강도 (기본값: 0.6~1.0)
	movementComponent->Friction = 0.5f; // 마찰력 증가 (기본값: 0.2~0.3)
	movementComponent->BounceVelocityStopSimulatingThreshold = 5.0f; // 특정 속도 이하에서는 바운스 멈춤

	// 로켓 발사 방향을 Y축 방향으로 설정
	// FVector LaunchDirection = FVector(Speed, 0.0f, 0.0f); // Y축을 따라 발사하도록 설정
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



	// collisionComponent->BodyInstance.SetMassOverride(1.0f); // 수류탄의 질량 설정 (예: 1kg)
}

void ABasicGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 던지기 속도가 설정되었을 경우 예상 경로 업데이트
	//PredictGrenadePath(LaunchVelocity);
	
	//// 월드에서 플레이어 캐릭터를 가져오기
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
	// 소유자와의 충돌 무시
	if (OtherActor == GetOwner())
	{
		return;
	}

	collisionComponent->SetSimulatePhysics(true);


	// 지연 작업을 위한 FLatentActionInfo 설정
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // 지연이 완료되면 호출할 객체
	LatentInfo.ExecutionFunction = FName(TEXT("Explosion")); // 실행할 함수 이름
	LatentInfo.Linkage = 0; // 연결 ID (주로 0 사용)
	LatentInfo.UUID = __LINE__; // 고유 ID (주로 코드 라인 사용)

	//// 디버그 드로잉으로 충돌 확인
	//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.0f, 12, FColor::Red, false, 1.0f);

	UKismetSystemLibrary::Delay(this, 3.f, LatentInfo);



}

void ABasicGrenade::Explosion()
{
	// 폭발 효과 표시
	if (explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	}

	Destroy();
}

//void ABasicGrenade::PredictGrenadePath(FVector LaunchVelocity, float MaxSimTime)
//{
//	// 결과를 저장할 구조체
//	FPredictProjectilePathResult PredictResult;
//
//	// 경로 계산 요청 파라미터
//	FPredictProjectilePathParams Params;
//	Params.StartLocation = GetActorLocation(); // 현재 수류탄의 위치
//	Params.LaunchVelocity = LaunchVelocity;   // 던질 때의 속도
//	Params.ProjectileRadius = ProjectileRadius;
//	Params.SimFrequency = SimFrequency;
//	Params.MaxSimTime = MaxSimTime;
//	Params.OverrideGravityZ = GravityZ;
//	Params.TraceChannel = TraceChannel;
//	Params.bTraceWithCollision = true;        // 충돌 확인
//
//
//	// 경로 예측
//	if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult))
//	{
//		// 경로 점들 시각화
//		for (const FPredictProjectilePathPointData& PointData : PredictResult.PathData)
//		{
//			DrawDebugSphere(GetWorld(), PointData.Location, 5.0f, 12, FColor::Green, false, 1.0f);
//		}
//
//		// 충돌 지점 시각화
//		if (PredictResult.HitResult.IsValidBlockingHit())
//		{
//			DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location, 10.0f, 12, FColor::Red, false, 1.0f);
//		}
//	}
//}