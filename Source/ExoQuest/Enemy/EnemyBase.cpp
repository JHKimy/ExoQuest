#include "Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyFSM.h"	// AI
#include "GameFramework/CharacterMovementComponent.h" // 움직임
#include "Kismet/KismetSystemLibrary.h"		// 화면 텍스츠 출력 


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(2.5f, 2.5f, 1.5f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy1/SM_Enemy1.SM_Enemy1'"));

	if (TempMesh.Succeeded())
	{
		// 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// FSM의 주인으로 설정
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));


}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 스탯
	health = 100.f;
	attackPower = 10.f;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	return 0.0f;
//}

//float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//
//	auto fsm2 = Cast<AEnemyBase>(GetComponentByClass(AEnemyBase::StaticClass()));
//
//
//	health -= DamageAmount;
//
//	// 현재 체력 출력
//	FString HealthString = FString::Printf(TEXT("Current Health: %f"), health);
//	UKismetSystemLibrary::PrintString(GetWorld(), HealthString, true, false, FLinearColor::Blue, 2.f);
//
//	//fsm2->mState = EEnemyState::Damage;
//
//
//
//	// 체력이 0 이하가 되면 사망 처리
//	if (health <= 0.0f)
//	{
//		// FSM이 nullptr인지 확인
//		if (fsm2)
//		{
//			fsm2->mState = EEnemyState::Die;  // FSM 상태 전환
//			fsm2->me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//			UKismetSystemLibrary::PrintString
//			(GetWorld(), TEXT("oops!!"), true, false, FLinearColor::Green, 2.f);
//		}
//		else
//		{
//			// FSM이 nullptr인 경우 에러 처리
//			UKismetSystemLibrary::PrintString
//			(GetWorld(), TEXT("NULL!!"), true, false, FLinearColor::Red, 2.f);
//		}
//	}
//
//	return DamageAmount;
//}
