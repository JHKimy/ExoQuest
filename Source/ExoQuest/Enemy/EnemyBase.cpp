#include "Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyFSM.h"	// AI
#include "GameFramework/CharacterMovementComponent.h" // ������
#include "Kismet/KismetSystemLibrary.h"		// ȭ�� �ؽ��� ��� 


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(2.5f, 2.5f, 1.5f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy1/SM_Enemy1.SM_Enemy1'"));

	if (TempMesh.Succeeded())
	{
		// ���̷�Ż �޽� ����
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// ��ġ ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// FSM�� �������� ����
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));


}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ����
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
//	// ���� ü�� ���
//	FString HealthString = FString::Printf(TEXT("Current Health: %f"), health);
//	UKismetSystemLibrary::PrintString(GetWorld(), HealthString, true, false, FLinearColor::Blue, 2.f);
//
//	//fsm2->mState = EEnemyState::Damage;
//
//
//
//	// ü���� 0 ���ϰ� �Ǹ� ��� ó��
//	if (health <= 0.0f)
//	{
//		// FSM�� nullptr���� Ȯ��
//		if (fsm2)
//		{
//			fsm2->mState = EEnemyState::Die;  // FSM ���� ��ȯ
//			fsm2->me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//			UKismetSystemLibrary::PrintString
//			(GetWorld(), TEXT("oops!!"), true, false, FLinearColor::Green, 2.f);
//		}
//		else
//		{
//			// FSM�� nullptr�� ��� ���� ó��
//			UKismetSystemLibrary::PrintString
//			(GetWorld(), TEXT("NULL!!"), true, false, FLinearColor::Red, 2.f);
//		}
//	}
//
//	return DamageAmount;
//}
