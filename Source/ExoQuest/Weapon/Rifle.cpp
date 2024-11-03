#include "Weapon/Rifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"


// y축이 앞방향 되어있는 에셋

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RIfle/SM_Rifle.SM_Rifle'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetupAttachment(RootComponent); // 메쉬를 캡슐에 부착
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	// MuzzleLocation Scene Component 생성 및 초기화
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// 기본 위치 설정 (총구 앞부분으로 위치 설정)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 2.f)); // X, Y, Z 좌표 조정 가능

	damage = 5;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle::Fire()
{
	// 라인 트레이스
	FVector startPos = muzzleLocation->GetComponentLocation();
	FVector endPos = muzzleLocation->GetComponentLocation() + muzzleLocation->GetForwardVector() * 5000;

	// 충돌 위치
	FHitResult hitInfo;
	// 충돌 옵션 정보
	FCollisionQueryParams params;
	// 자신은 제외
	params.AddIgnoredActor(ownerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	// 디버그 라인 추가
	DrawDebugLine
	(
		GetWorld(),
		startPos,         // 시작점
		endPos,           // 끝점
		FColor::Red,    // 선 색상 (녹색)
		false,            // 지속 여부 (false면 일시적으로만 표시됨)
		1.0f,             // 지속 시간 (1초 동안 표시)
		0,                // 두께 (디버그 라인의 채널)
		1.0f              // 선의 두께
	);

	if (bHit)
	{
		// 총알 파편 효과 트랜스폼
		FTransform bulletTrans;
		// 부딪힌 위치 할당
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		// 총알 파편 효과 인스턴스 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

		// 총알에 충돌시 적인지 판단
		auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));

		if (enemy)
		{
			auto enemyFSM = Cast<UEnemyFSM>(enemy);
			enemyFSM->OnDamageProcess();
		}

	}


}

