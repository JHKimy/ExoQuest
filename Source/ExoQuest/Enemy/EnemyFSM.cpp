#include "Enemy/EnemyFSM.h"
#include "Enemy/EnemyBase.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>		// 맵에 있는 인스턴스 가져오기
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"
#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/RocketProjectile.h"
#include "Weapon/Sword.h"
#include "Enemy1AnimInstance.h"
#include "Item/Starflux.h"
#include "Enemy/Enemy1.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass());

	target = Cast<ACharacterBase>(actor);
	enemy = Cast<AEnemyBase>(GetOwner());
	enemy1 = Cast<AEnemy1>(GetOwner());
	anim = Cast<UEnemy1AnimInstance>(enemy->GetMesh()->GetAnimInstance());

	StateMap.Add(EEnemyState::Idle, &IdleState);
	StateMap.Add(EEnemyState::Move, &MoveState);
	StateMap.Add(EEnemyState::Attack, &AttackState);
	StateMap.Add(EEnemyState::Damage, &DamageState);
	StateMap.Add(EEnemyState::Death, &DeathState);

	ChangeState(EEnemyState::Idle);

}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Update(this, DeltaTime);
	}

}

void UEnemyFSM::ChangeState(EEnemyState NewState)
{
	if (CurrentState)
		CurrentState->Exit(this);

	if (StateMap.Contains(NewState))
	{
		CurrentState = StateMap[NewState];
		CurrentStateType = NewState;
		CurrentState->Enter(this);
	}
}

void UEnemyFSM::ResetTimer()
{
	currentTime = 0.f;
}

void UEnemyFSM::AddTimer(float DeltaTime)
{
	currentTime += DeltaTime;
}

float UEnemyFSM::GetTimer() const
{
	return currentTime;
}
// ===================================================================================================================================
// ========== EnemyIdleState ==========
void EnemyIdleState::Enter(UEnemyFSM* FSM)
{
	FSM->anim->animState = EEnemyState::Idle;
	FSM->ResetTimer();
}
void EnemyIdleState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 2.f)
		FSM->ChangeState(EEnemyState::Move);
}
void EnemyIdleState::Exit(UEnemyFSM* FSM) {}

void EnemyMoveState::Enter(UEnemyFSM* FSM)
{
	FSM->anim->animState = EEnemyState::Move;
}
void EnemyMoveState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	if (!FSM->enemy || !FSM->target) return;
	FVector dir = FSM->target->GetActorLocation() - FSM->enemy->GetActorLocation();
	FSM->enemy->AddMovementInput(dir.GetSafeNormal());
	if (dir.Size() < 150.f)
		FSM->ChangeState(EEnemyState::Attack);
}
void EnemyMoveState::Exit(UEnemyFSM* FSM) {}

void EnemyAttackState::Enter(UEnemyFSM* FSM)
{
	FSM->anim->animState = EEnemyState::Attack;
	FSM->anim->bAttackPlay = true;
	FSM->ResetTimer();
}
void EnemyAttackState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 1.5f)
		FSM->ChangeState(EEnemyState::Idle);
}
void EnemyAttackState::Exit(UEnemyFSM* FSM)
{
	FSM->anim->bAttackPlay = false;
}

void EnemyDamageState::Enter(UEnemyFSM* FSM)
{
	FSM->anim->animState = EEnemyState::Damage;
	FSM->ResetTimer();
}
void EnemyDamageState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 1.0f)
		FSM->ChangeState(EEnemyState::Idle);
}
void EnemyDamageState::Exit(UEnemyFSM* FSM) {}

void EnemyDeathState::Enter(UEnemyFSM* FSM)
{
	UE_LOG(LogTemp, Warning, TEXT("Parent"));

	FSM->anim->animState = EEnemyState::Death;

	// ****************
	FSM->enemy1->Death();
}
void EnemyDeathState::Update(UEnemyFSM* FSM, float DeltaTime) {}
void EnemyDeathState::Exit(UEnemyFSM* FSM) {}