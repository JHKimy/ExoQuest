#include "Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyFSM.h"	// AI
#include "GameFramework/CharacterMovementComponent.h" // ������
#include "Kismet/KismetSystemLibrary.h"		// ȭ�� �ؽ��� ��� 
#include "Item/Starflux.h"

#include "PaperSpriteComponent.h"

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

	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass�� ����Ͽ� �ִϸ��̼� �������Ʈ Ŭ���� �ε�
	AnimBP = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));
	
	if (AnimBP) {
		UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	}

	localMesh->SetAnimInstanceClass(AnimBP);

	if (enemyPosition) {
		enemyPosition->bVisibleInSceneCaptureOnly = true;
	}
	//enemyPosition->bRenderCustomDepth = true;


	// ���� ī�޶󿡼� PaperSprite �����
	//enemyPosition->SetVisibility(false);

	// �̴ϸ� ī�޶󿡼� PaperSprite ���̰� �ϱ�
	//enemyPosition->SetVisibility(true);




	// �̴ϸ� ī�޶󿡼� PaperSprite ���̰� �ϱ�
	//enemyPosition->SetVisibility(true);
	//enemyPosition->SetHiddenInGame(true);








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



// ## ���ö��� ����ũ ������ ����� ���� �; ������ �ý��۸� �̰ɷ� �ۼ� ##
// Į�� �Ҷ� ������
float AEnemyBase::TakeDamage(
	float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	// FSM ��������
	auto fsmForDamage = Cast<UEnemyFSM>(GetComponentByClass(UEnemyFSM::StaticClass()));

	// ü�� 
	health -= DamageAmount;

	// ���� ü�� ���
	//FString HealthString = FString::Printf(TEXT("Current Health: %f"), health);
	//UKismetSystemLibrary::PrintString(GetWorld(), HealthString, true, false, FLinearColor::Blue, 2.f);

	// ü���� 0 ���ϰ� �Ǹ� ��� ó��
	if (health <= 0.0f)
	{
		// FSM�� nullptr���� Ȯ��
		if (fsmForDamage)
		{
			fsmForDamage->EState = EEnemyState::Die;  // FSM ���� ��ȯ
			// óġ�� �浹ü �����
			fsmForDamage->enemy->GetCapsuleComponent()->
				SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//UKismetSystemLibrary::PrintString
			//(GetWorld(), TEXT("oops!!"), true, false, FLinearColor::Green, 2.f);
		}
		else
		{
			// FSM�� nullptr�� ��� ���� ó��
			//UKismetSystemLibrary::PrintString
			//(GetWorld(), TEXT("NULL!!"), true, false, FLinearColor::Red, 2.f);
		}
	}

	return DamageAmount;
}