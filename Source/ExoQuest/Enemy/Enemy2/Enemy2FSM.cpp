#include "Enemy/Enemy2/Enemy2FSM.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Enemy/Enemy2/Enemy2AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CharacterBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


UEnemy2FSM::UEnemy2FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemy2FSM::BeginPlay()
{
	Super::BeginPlay();

	StateMap.Add(EEnemyState::Idle, &Enemy2IdleState);
	StateMap.Add(EEnemyState::Patrol, &Enemy2PatrolState);
	StateMap.Add(EEnemyState::Move, &Enemy2MoveState);
	StateMap.Add(EEnemyState::Attack, &Enemy2AttackState);
	StateMap.Add(EEnemyState::Damage, &Enemy2DamageState);
	StateMap.Add(EEnemyState::Death, &Enemy2DeathState);

	ChangeState(EEnemyState::Idle);
}


void Enemy2IdleState::Enter(UEnemyFSM* FSM)
{
	// EnemyIdleState::Enter(FSM);
	
	FSM->enemy2->SetAnimState(EEnemyState::Idle);

	if (FSM->enemy2AIController) {
		FSM->enemy2AIController->RunIdleBT();
	}

	FSM->ResetTimer();
}

void Enemy2IdleState::Update(UEnemyFSM* FSM, float DeltaTime)
{
    // EnemyIdleState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);  // 시간 누적

	if (FSM->enemy2->bIsPlayerDetected || FSM->enemy2->bDamaged) {
		FSM->ChangeState(EEnemyState::Move);
	}

	if (FSM->GetTimer() > FSM->enemy2->idleTime)
	{
		FSM->ChangeState(EEnemyState::Patrol);
	}


}

void Enemy2IdleState::Exit(UEnemyFSM* FSM)
{
	EnemyIdleState::Exit(FSM);
	FSM->ResetTimer();


	//FSM->ResetTimer();
}







void Enemy2PatrolState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy2->SetAnimState(EEnemyState::Patrol);

	FSM->enemy2->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	if (FSM->enemy2AIController) {
		FSM->enemy2AIController->RunPatrolBT();
	}


	FSM->ResetTimer();  // 타이머 초기화
}

void Enemy2PatrolState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->AddTimer(DeltaTime);  // 시간 누적

	if (FSM->enemy2->bIsPlayerDetected || FSM->enemy2->bDamaged) {
		FSM->ChangeState(EEnemyState::Move);
	}

	if (FSM->GetTimer() > FSM->enemy2->patrolTime)
	{
		FSM->ChangeState(EEnemyState::Idle);
	}

	//if (FSM->enemy2->bDamaged) {
	//	FSM->ChangeState(EEnemyState::Move);
	//}
}

void Enemy2PatrolState::Exit(UEnemyFSM* FSM)
{
	FSM->enemy2AIController->StopMovement();
	FSM->ResetTimer();

}







void Enemy2MoveState::Enter(UEnemyFSM* FSM)
{

	EnemyMoveState::Enter(FSM);

	FSM->enemy2->GetCharacterMovement()->MaxWalkSpeed = 600.f;

	FSM->enemy2->SetAnimState(EEnemyState::Move);


	if (FSM->enemy2AIController) {
		FSM->enemy2AIController->RunChaseBT();
	}

}

void Enemy2MoveState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyMoveState::Update(FSM, DeltaTime);

	// 타겟과의 거리 체크
	if (FSM->target)
	{
		const float Distance = FVector::Dist(FSM->enemy2->GetActorLocation(), FSM->target->GetActorLocation());

		if (Distance <= 700.0f)
		{
			FSM->ChangeState(EEnemyState::Attack);
			return; // 상태 전환 후 더 이상 Update하지 않음
		}
	}

}

void Enemy2MoveState::Exit(UEnemyFSM* FSM)
{
	EnemyMoveState::Exit(FSM);
	FSM->enemy2AIController->StopMovement();
	FSM->ResetTimer();

}







void Enemy2AttackState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy2->SetAnimState(EEnemyState::Attack);

	if (FSM->enemy2AIController) {
		FSM->enemy2AIController->RunAttackBT();
	}


	// FSM->enemy2->SetAttackPlay(true);
	// FSM->enemy1->bDidAttackHit = false; // <- 공격 히트 여부 초기화

	// FSM->ResetTimer();

	// EnemyAttackState::Enter(FSM);

}

void Enemy2AttackState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	//EnemyAttackState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);

	FSM->enemy2->RotateToTarget();

	if (FSM->target)
	{
		const float Distance = FVector::Dist(FSM->enemy2->GetActorLocation(), FSM->target->GetActorLocation());

		if (Distance > 700.0f)
		{
			FSM->ChangeState(EEnemyState::Move);
			return; // 상태 전환 후 더 이상 Update하지 않음
		}
	}


	//if (!FSM->enemy2->bIsPlayerDetected)
	//{
		//FSM->AddUnseenTimer(DeltaTime);

		//if (FSM->GetUnseenTime() > 5.0f)
		//{
		//	FSM->enemy2->bIsPlayerDetected = false;
		//	FSM->ChangeState(EEnemyState::Patrol);
		//	return;
		//}
	//}
	//else
	//{
	//	FSM->ResetUnseenTimer();
	//}

	//if (FSM->GetTimer() > 2.5f)
	//	FSM->ChangeState(EEnemyState::Idle);
}

void Enemy2AttackState::Exit(UEnemyFSM* FSM)
{
	FSM->enemy2AIController->StopMovement();
	FSM->ResetTimer();

	// EnemyAttackState::Exit(FSM);
	// FSM->enemy1->SetAttackPlay(false);
	//FSM->enemy1->bDidAttackHit = false; // <- 상태 초기화
}







void Enemy2DamageState::Enter(UEnemyFSM* FSM)
{
	//FSM->enemy2->SetAnimState(EEnemyState::Damage);
	// FSM->ResetTimer();

	// EnemyDamageState::Enter(FSM);

}

void Enemy2DamageState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	// EnemyDamageState::Update(FSM, DeltaTime);
	//FSM->AddTimer(DeltaTime);
	//if (FSM->GetTimer() > 1.0f)
	//	FSM->ChangeState(EEnemyState::Idle);
}

void Enemy2DamageState::Exit(UEnemyFSM* FSM)
{
	//FSM->ResetTimer();
	//EnemyDamageState::Exit(FSM);

}







void Enemy2DeathState::Enter(UEnemyFSM* FSM)
{
	EnemyDeathState::Enter(FSM);
	FSM->enemy2->SetAnimState(EEnemyState::Death);

	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(FSM->enemy2AIController->BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree(EBTStopMode::Safe);
	}


	FSM->ResetTimer(); // 타이머 초기화
}

void Enemy2DeathState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyDeathState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime); // 시간 누적
	if (FSM->GetTimer() > 3.0f)
	{
		FSM->enemy2->Death();
	}
}

void Enemy2DeathState::Exit(UEnemyFSM* FSM)
{
	FSM->ResetTimer();
	EnemyDeathState::Exit(FSM);

}