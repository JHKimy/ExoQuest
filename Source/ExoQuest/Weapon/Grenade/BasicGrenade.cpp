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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectAsset(TEXT("/Game/Asset/Effect/P_GrenadeExplosion.P_GrenadeExplosion"));
	if (ExplosionEffectAsset.Succeeded())
	{
		explosionEffect = ExplosionEffectAsset.Object;
	}
}

void ABasicGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComponent->BodyInstance.SetMassOverride(1.0f); // 수류탄의 질량 설정 (예: 1kg)
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

void ABasicGrenade::PredictGrenadePath(FVector LaunchVelocity, float MaxSimTime)
{
	// 결과를 저장할 구조체
	FPredictProjectilePathResult PredictResult;

	// 경로 계산 요청 파라미터
	FPredictProjectilePathParams Params;
	Params.StartLocation = GetActorLocation(); // 현재 수류탄의 위치
	Params.LaunchVelocity = LaunchVelocity;   // 던질 때의 속도
	Params.ProjectileRadius = ProjectileRadius;
	Params.SimFrequency = SimFrequency;
	Params.MaxSimTime = MaxSimTime;
	Params.OverrideGravityZ = GravityZ;
	Params.TraceChannel = TraceChannel;
	Params.bTraceWithCollision = true;        // 충돌 확인


	// 경로 예측
	if (UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult))
	{
		// 경로 점들 시각화
		for (const FPredictProjectilePathPointData& PointData : PredictResult.PathData)
		{
			DrawDebugSphere(GetWorld(), PointData.Location, 5.0f, 12, FColor::Green, false, 1.0f);
		}

		// 충돌 지점 시각화
		if (PredictResult.HitResult.IsValidBlockingHit())
		{
			DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location, 10.0f, 12, FColor::Red, false, 1.0f);
		}
	}
}