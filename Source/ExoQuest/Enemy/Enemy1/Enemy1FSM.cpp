#include "Enemy/Enemy1/Enemy1FSM.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy/Enemy1/Enemy1.h"
#include "Enemy/Enemy1/Enemy1AnimInstance.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>

UEnemy1FSM::UEnemy1FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemy1FSM::BeginPlay()
{
	Super::BeginPlay();

	StateMap.Add(EEnemyState::Idle, &Enemy1IdleState);
	StateMap.Add(EEnemyState::Patrol, &Enemy1PatrolState);
	StateMap.Add(EEnemyState::Move, &Enemy1MoveState);
	StateMap.Add(EEnemyState::Attack, &Enemy1AttackState);
	StateMap.Add(EEnemyState::Damage, &Enemy1DamageState);
	StateMap.Add(EEnemyState::Death, &Enemy1DeathState);

	ChangeState(EEnemyState::Idle);
}




void Enemy1IdleState::Enter(UEnemyFSM* FSM)
{
	EnemyIdleState::Enter(FSM);
	FSM->enemy1->SetAnimState(EEnemyState::Idle);
	FSM->ResetTimer();
}

void Enemy1IdleState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyIdleState::Update(FSM, DeltaTime);

	//FSM->AddTimer(DeltaTime);

	// 실제 움직임은 enemy1 안에서 처리됨 (FSM->enemy1->MoveToTarget())
	//float distance = FVector::Dist(FSM->target->GetActorLocation(), FSM->enemy1->GetActorLocation());
	if (FSM->GetDistanceToTarget() < FSM->enemy1->GetAttackRange())
	{
		FSM->ChangeState(EEnemyState::Attack);
	}
	else if (FSM->GetDistanceToTarget() < FSM->enemy1->GetDetectRange())
	{
		FSM->ChangeState(EEnemyState::Move);
	}
	else
	{
		FSM->ChangeState(EEnemyState::Patrol); // 일정 시간 후 순찰 시작해도 됨
	}
}


void Enemy1IdleState::Exit(UEnemyFSM* FSM)
{
	EnemyIdleState::Exit(FSM);
	//FSM->ResetTimer();
}
void Enemy1PatrolState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy1->SetAnimState(EEnemyState::Patrol);
}

void Enemy1PatrolState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	FSM->enemy1->MoveToPatrolPoint();

	bool inSight = FSM->enemy1->IsPlayerInSight();

	// 플레이어 감지 시 바로 추적
	if (FSM->GetDistanceToTarget() < FSM->enemy1->GetDetectRange() && inSight)
	{
		FSM->ChangeState(EEnemyState::Move);
	}
}

void Enemy1PatrolState::Exit(UEnemyFSM* FSM)
{
}



void Enemy1MoveState::Enter(UEnemyFSM* FSM)
{
	EnemyMoveState::Enter(FSM);

	FSM->enemy1->SetAnimState(EEnemyState::Move);

}

void Enemy1MoveState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyMoveState::Update(FSM, DeltaTime);

	FSM->enemy1->MoveToTarget();

	// 실제 움직임은 enemy1 안에서 처리됨 (FSM->enemy1->MoveToTarget())
	if (FSM->GetDistanceToTarget() < FSM->enemy1->GetAttackRange())
	{
		FSM->ChangeState(EEnemyState::Attack);
	}
	else if (FSM->GetDistanceToTarget() >= FSM->enemy1->GetDetectRange())
	{
		//// 탐지 범위 밖이면 복귀 시작
		//if (!FSM->enemy1->bReturningToOrigin)
		//{
			FSM->enemy1->ReturnToPatrolLocation();
			FSM->ChangeState(EEnemyState::Patrol);
		//}

		//float distanceToOrigin = FVector::Dist(FSM->enemy1->GetActorLocation(), FSM->enemy1->InitialPosition);
		//if (distanceToOrigin < 100.f)
		//{
		//	FSM->enemy1->bReturningToOrigin = false;
		//	FSM->ChangeState(EEnemyState::Patrol);
		//}
	}
	//if (FSM->GetDistanceToTarget() >= FSM->enemy1->GetDetectRange())
	//{
	//	FSM->ChangeState(EEnemyState::Idle);
	//}
}

void Enemy1MoveState::Exit(UEnemyFSM* FSM)
{
	EnemyMoveState::Exit(FSM);

}



void Enemy1AttackState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy1->SetAnimState(EEnemyState::Attack);
	FSM->enemy1->SetAttackPlay(true);
	//FSM->enemy1->bDidAttackHit = false; // <- 공격 히트 여부 초기화

	// FSM->ResetTimer();

	// EnemyAttackState::Enter(FSM);

}

void Enemy1AttackState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	//EnemyAttackState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 2.5f)
		FSM->ChangeState(EEnemyState::Idle);
}

void Enemy1AttackState::Exit(UEnemyFSM* FSM)
{
	// EnemyAttackState::Exit(FSM);
	FSM->enemy1->SetAttackPlay(false);
	//FSM->enemy1->bDidAttackHit = false; // <- 상태 초기화
}

void Enemy1DamageState::Enter(UEnemyFSM* FSM)
{
	FSM->enemy1->SetAnimState(EEnemyState::Damage);
	FSM->ResetTimer();

	// EnemyDamageState::Enter(FSM);

}

void Enemy1DamageState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	// EnemyDamageState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime);
	if (FSM->GetTimer() > 1.0f)
		FSM->ChangeState(EEnemyState::Idle);
}

void Enemy1DamageState::Exit(UEnemyFSM* FSM)
{
	EnemyDamageState::Exit(FSM);

}

void Enemy1DeathState::Enter(UEnemyFSM* FSM)
{
	EnemyDeathState::Enter(FSM);
	FSM->enemy1->SetAnimState(EEnemyState::Death);

	FSM->ResetTimer(); // 타이머 초기화
}

void Enemy1DeathState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyDeathState::Update(FSM, DeltaTime);
	FSM->AddTimer(DeltaTime); // 시간 누적
	if (FSM->GetTimer() > 3.0f)
	{
		FSM->enemy1->Death();
	}
}

void Enemy1DeathState::Exit(UEnemyFSM* FSM)
{
	EnemyDeathState::Exit(FSM);

}