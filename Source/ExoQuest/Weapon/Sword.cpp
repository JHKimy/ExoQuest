#include "Weapon/Sword.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"


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
	damage = 10.f;
	// 쿨타임
	coolTime = 0.45f;
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 공격 중일 때 경로를 따라 박스를 그려줌
	if (bIsAttacking && attackCollision)
	{
		FVector BoxLocation = attackCollision->GetComponentLocation();
		FVector BoxExtent = attackCollision->GetScaledBoxExtent();
		FRotator BoxRotation = attackCollision->GetComponentRotation();

		// DrawDebugBox 함수로 디버그 시각화 (3초 동안 유지)
		DrawDebugBox(GetWorld(), BoxLocation, BoxExtent, BoxRotation.Quaternion(), FColor::Red, false, 0.5f, 0, 1.0f);
	}
}

void ASword::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking) return; // 공격 중이 아닐 경우 데미지를 입히지 않음

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

void ASword::Slash()
{

	if (bIsAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slash()가 이미 진행 중입니다."));
		return;
	}

	bIsAttacking = true;
	UE_LOG(LogTemp, Log, TEXT("Slash()가 시작되었습니다."));

	if (GetWorld())
	{
		UE_LOG(LogTemp, Log, TEXT("GetWorld()가 성공적으로 호출되었습니다."));
		GetWorld()->GetTimerManager().SetTimer(
			SalshTimer, this, &ASword::ResetSlash, coolTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld()가 null입니다."));
	}
}

void ASword::ResetSlash()
{
	bIsAttacking = false;
}

