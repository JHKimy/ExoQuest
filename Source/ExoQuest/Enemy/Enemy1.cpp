#include "Enemy/Enemy1.h"
//#include "Enemy/EnemyFSM.h"	// AI
#include "Enemy/Enemy1FSM.h"	// AI
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"              // 플레이어에게 데미지 줄 때 필요
#include "GameFramework/DamageType.h"      // 이건 타입을 정할 때 필요
#include "Engine/EngineTypes.h"         // FDamageEvent 포함됨!!
#include "Enemy/Enemy1AnimInstance.h"
#include "GameFramework/Controller.h"  // AController 정의 필요 시

AEnemy1::AEnemy1()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy1/SM_Enemy1.SM_Enemy1'"));

	if (TempMesh.Succeeded())
	{
		// 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), FName("RightHandSocket")); // 본에 붙이기
	AttackCollision->SetCollisionProfileName("OverlapAll");
	AttackCollision->SetGenerateOverlapEvents(false); // 기본은 꺼둠
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy1::OnHandOverlap);

	
	// FSM1의 주인으로 설정
	FSM = CreateDefaultSubobject<UEnemy1FSM>(TEXT("FSM"));
}

void AEnemy1::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBPClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));


	localMesh->SetAnimInstanceClass(AnimBPClass);

	if (AnimBPClass) {
		UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	}

	anim = Cast<UEnemy1AnimInstance>(GetMesh()->GetAnimInstance());


}

void AEnemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy1::SetAnimState(EEnemyState newState)
{
	if (anim)
	{
		anim->animState = newState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemy1::SetAnimState 실패: anim이 nullptr입니다."));
	}
}


void AEnemy1::SetAttackPlay(bool b)
{
	anim->bAttackPlay = b;
}

void AEnemy1::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanDamage) return;
	ACharacterBase* player = Cast<ACharacterBase>(OtherActor);
	if (player)
	{
		// ApplyDamage: 데미지를 플레이어에게 적용
		UGameplayStatics::ApplyDamage(
			player,					// 데미지를 받을 대상
			10.f,					// 데미지 양
			GetController(),		// 데미지 인스티게이터 (공격자 컨트롤러)
			this,					// 데미지 유발자 (적 액터 자신)
			UDamageType::StaticClass() // 데미지 타입
		);

		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage !"));
	}
}

float AEnemy1::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	anim->PlayHitMontage();

	return actualDamage;
}

void AEnemy1::Death()
{
	Super::Death();
}

void AEnemy1::MoveToTarget()
{
	Super::MoveToTarget();
}