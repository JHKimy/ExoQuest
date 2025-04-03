// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy1FSM.generated.h"


class Enemy1IdleState : public EnemyIdleState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy1PatrolState : public EnemyPatrolState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};

class Enemy1MoveState : public EnemyMoveState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy1AttackState : public EnemyAttackState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy1DamageState : public EnemyDamageState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy1DeathState : public EnemyDeathState {
public:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

UCLASS()
class EXOQUEST_API UEnemy1FSM : public UEnemyFSM
{
	GENERATED_BODY()

public:
	UEnemy1FSM();
	Enemy1IdleState   Enemy1IdleState;
	Enemy1PatrolState Enemy1PatrolState;
	Enemy1MoveState   Enemy1MoveState;
	Enemy1AttackState Enemy1AttackState;
	Enemy1DamageState Enemy1DamageState;
	Enemy1DeathState  Enemy1DeathState;

protected:
	virtual void BeginPlay() override;

	//FORCEINLINE float GetDistanceToTarget() override
	//{
	//	return FVector::Dist(target->GetActorLocation(), enemy1->GetActorLocation());
	//};
};


