#include "Enemy/Enemy2/Enemy2.h"
#include "Enemy/Weapon/EnemyRifle.h"
#include "Enemy/Enemy2/Enemy2AnimInstance.h"
#include "Enemy/Enemy2/Enemy2FSM.h"

AEnemy2::AEnemy2()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy2/SM_Enemy2.SM_Enemy2'"));

	if (TempMesh.Succeeded())
	{
		// 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// FSM1의 주인으로 설정
	FSM = CreateDefaultSubobject<UEnemy2FSM>(TEXT("FSM"));
}

void AEnemy2::BeginPlay()
{	
	Super::BeginPlay();

	EnemyRifle = GetWorld()->SpawnActor<AEnemyRifle>();

	if (EnemyRifle)
	{
		//  손에 부착 (소켓 이름은 스켈레탈 메시의 본 이름 기준으로 설정)
		EnemyRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RifleSocketName);
	}

	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBPClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy2/ABP_Enemy2.ABP_Enemy2_C"));


	// 메쉬 입히기
	// USkeletalMeshComponent* localMesh = GetMesh();
	GetMesh()->SetAnimInstanceClass(AnimBPClass);

	anim = Cast<UEnemy2AnimInstance>(GetMesh()->GetAnimInstance());

	//AI = Cast<AAIController>(GetController());
}

void AEnemy2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy2::SetAnimState(EEnemyState NewState)
{
	if (anim)
	{
		anim->SetAnimState(NewState);
	}
}

float AEnemy2::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// anim->PlayHitMontage();

	return actualDamage;
}

// void AEnemy2::SetAnimState(EEnemyState NewState)
// {
// }

void AEnemy2::Death()
{
	Super::Death();
}
