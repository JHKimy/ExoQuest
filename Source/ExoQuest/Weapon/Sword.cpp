#include "Weapon/Sword.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"


ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Sword/SM_Sword.SM_Sword'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	attackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	attackCollision->SetRelativeLocationAndRotation(
		FVector(6.7f, 0.f, 40.f), FRotator(0.f, -2.5f, 0.f));
	attackCollision->SetRelativeScale3D(FVector(0.65f, 0.25f, 1.9f));
	attackCollision->SetupAttachment(RootComponent);
	attackCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));  // 충돌 프로파일 설정
	attackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnWeaponOverlapBegin);

	// 데미지
	damage = 30.f;
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());  // 무기 소유자 확인

	// 적 캐릭터에 대한 타격 감지
	if (OtherActor && OtherActor != this)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);

		if (Enemy && Enemy->health > 0)  // 적이 살아있는지 확인
		{
			// UGameplayStatics::ApplyDamage 사용
			UGameplayStatics::ApplyDamage(Enemy, damage, OwnerPawn->GetController(), this, UDamageType::StaticClass());
		}
	}

}

