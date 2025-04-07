#include "Enemy/Enemy2/Enemy2FSM.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Enemy/Enemy2/Enemy2AIController.h"

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


	//if (FSM->GetTimer() > 3.0f)
	//{
	//	FSM->ChangeState(EEnemyState::Patrol);
	//}


}

void Enemy2IdleState::Exit(UEnemyFSM* FSM)
{
	EnemyIdleState::Exit(FSM);
	//FSM->ResetTimer();
}





void Enemy2PatrolState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy2->SetAnimState(EEnemyState::Patrol);
	
	if (FSM->enemy2AIController) {
		FSM->enemy2AIController->RunPatrolBT();
	}


	FSM->ResetTimer();  // 타이머 초기화
}

void Enemy2PatrolState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->AddTimer(DeltaTime);  // 시간 누적
	if (FSM->GetTimer() > 10.0f)
	{
		FSM->ChangeState(EEnemyState::Idle);
	}
	//FSM->enemy2->MoveToPatrolPoint();

	//bool inSight = FSM->enemy1->IsPlayerInSight();

	//// 플레이어 감지 시 바로 추적
	//if (FSM->GetDistanceToTarget() < FSM->enemy1->GetDetectRange() && inSight)
	//{
	//	FSM->ChangeState(EEnemyState::Move);
	//}
}

void Enemy2PatrolState::Exit(UEnemyFSM* FSM)
{
}





void Enemy2MoveState::Enter(UEnemyFSM* FSM)
{
	EnemyMoveState::Enter(FSM);

	FSM->enemy2->SetAnimState(EEnemyState::Move);

}

void Enemy2MoveState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyMoveState::Update(FSM, DeltaTime);

	//FSM->enemy1->MoveToTarget();

	//// 실제 움직임은 enemy1 안에서 처리됨 (FSM->enemy1->MoveToTarget())
	//if (FSM->GetDistanceToTarget() < FSM->enemy1->GetAttackRange())
	//{
	//	FSM->ChangeState(EEnemyState::Attack);
	//}
	//else if (FSM->GetDistanceToTarget() >= FSM->enemy1->GetDetectRange())
	//{
	//	//// 탐지 범위 밖이면 복귀 시작
	//	//if (!FSM->enemy1->bReturningToOrigin)
	//	//{
	//	FSM->enemy1->ReturnToPatrolLocation();
	//	FSM->ChangeState(EEnemyState::Patrol);
	//	//}

	//	//float distanceToOrigin = FVector::Dist(FSM->enemy1->GetActorLocation(), FSM->enemy1->InitialPosition);
	//	//if (distanceToOrigin < 100.f)
	//	//{
	//	//	FSM->enemy1->bReturningToOrigin = false;
	//	//	FSM->ChangeState(EEnemyState::Patrol);
	//	//}
	//}
	////if (FSM->GetDistanceToTarget() >= FSM->enemy1->GetDetectRange())
	////{
	////	FSM->ChangeState(EEnemyState::Idle);
	////}
}

void Enemy2MoveState::Exit(UEnemyFSM* FSM)
{
	EnemyMoveState::Exit(FSM);

}





void Enemy2AttackState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy2->SetAnimState(EEnemyState::Attack);
	// FSM->enemy2->SetAttackPlay(true);
	// FSM->enemy1->bDidAttackHit = false; // <- 공격 히트 여부 초기화

	// FSM->ResetTimer();

	// EnemyAttackState::Enter(FSM);

}

void Enemy2AttackState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	//EnemyAttackState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 2.5f)
		FSM->ChangeState(EEnemyState::Idle);
}

void Enemy2AttackState::Exit(UEnemyFSM* FSM)
{
	// EnemyAttackState::Exit(FSM);
	// FSM->enemy1->SetAttackPlay(false);
	//FSM->enemy1->bDidAttackHit = false; // <- 상태 초기화
}





void Enemy2DamageState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy2->SetAnimState(EEnemyState::Damage);
	FSM->ResetTimer();

	// EnemyDamageState::Enter(FSM);

}

void Enemy2DamageState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	// EnemyDamageState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 1.0f)
		FSM->ChangeState(EEnemyState::Idle);
}

void Enemy2DamageState::Exit(UEnemyFSM* FSM)
{
	EnemyDamageState::Exit(FSM);

}





void Enemy2DeathState::Enter(UEnemyFSM* FSM)
{
	EnemyDeathState::Enter(FSM);
	FSM->enemy2->SetAnimState(EEnemyState::Death);

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
	EnemyDeathState::Exit(FSM);

}