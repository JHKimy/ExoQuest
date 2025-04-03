// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy1FSM.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy/Enemy1.h"
#include "Enemy/Enemy1AnimInstance.h"
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
	StateMap.Add(EEnemyState::Move, &Enemy1MoveState);
	StateMap.Add(EEnemyState::Attack, &Enemy1AttackState);
	StateMap.Add(EEnemyState::Damage, &Enemy1DamageState);
	StateMap.Add(EEnemyState::Death, &Enemy1DeathState);

	ChangeState(EEnemyState::Idle);
}

void UEnemy1FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UEnemyFSM::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FString StateName;
	//switch (CurrentStateType)
	//{
	//case EEnemyState::Idle:   StateName = TEXT("Idle"); break;
	//case EEnemyState::Move:   StateName = TEXT("Move"); break;
	//case EEnemyState::Attack: StateName = TEXT("Attack"); break;
	//case EEnemyState::Damage: StateName = TEXT("Damage"); break;
	//case EEnemyState::Death:  StateName = TEXT("Death"); break;
	//default: StateName = TEXT("Unknown"); break;
	//}
	//UKismetSystemLibrary::PrintString(this, StateName);

	//if (CurrentState)
	//{
	//	CurrentState->Update(this, DeltaTime);
	//}
}

void UEnemy1FSM::ChangeState(EEnemyState NewState)
{
	UEnemyFSM::ChangeState(NewState);
	//if (CurrentState)
	//	CurrentState->Exit(this);

	//if (StateMap.Contains(NewState))
	//{
	//	CurrentState = StateMap[NewState];
	//	CurrentStateType = NewState;
	//	CurrentState->Enter(this);
	//}
}



void Enemy1IdleState::Enter(UEnemyFSM* FSM)
{
	UKismetSystemLibrary::PrintString(FSM, TEXT("Entered Idle State"), true, false, FLinearColor::Yellow, 1.5f);

	EnemyIdleState::Enter(FSM);
	FSM->enemy1->SetAnimState(EEnemyState::Idle);
	FSM->ResetTimer();
}

void Enemy1IdleState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyIdleState::Update(FSM, DeltaTime);

	FSM->AddTimer(DeltaTime);

	// 화면에 시간 출력
	UKismetSystemLibrary::PrintString(
		FSM,
		FString::Printf(TEXT("Idle Timer: %.2f초"), FSM->GetTimer()),
		true,         // 화면에 표시
		false,        // 로그에는 안 찍음
		FLinearColor::Green,
		0.f           // 지속 시간 (0이면 한 프레임)
	);

	if (FSM->GetTimer() > 2.f)
	{
		FSM->ChangeState(EEnemyState::Move);
	}
}


void Enemy1IdleState::Exit(UEnemyFSM* FSM)
{
	EnemyIdleState::Exit(FSM);
	FSM->ResetTimer();
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
	float distance = FVector::Dist(FSM->target->GetActorLocation(), FSM->enemy1->GetActorLocation());
	if (distance < 150.f)
	{
		FSM->ChangeState(EEnemyState::Attack);
	}
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
	if (FSM->GetTimer() > 3.f)
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
}

void Enemy1DeathState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	EnemyDeathState::Update(FSM, DeltaTime);

	FSM->enemy1->Death();
}

void Enemy1DeathState::Exit(UEnemyFSM* FSM)
{
	EnemyDeathState::Exit(FSM);

}