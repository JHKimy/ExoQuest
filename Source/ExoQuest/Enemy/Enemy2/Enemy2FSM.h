#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy2FSM.generated.h"

class Enemy2IdleState : public EnemyIdleState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy2PatrolState : public EnemyPatrolState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};

class Enemy2MoveState : public EnemyMoveState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy2AttackState : public EnemyAttackState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy2DamageState : public EnemyDamageState {
public:
	void Enter(UEnemyFSM* FSM) override;

	void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};

class Enemy2DeathState : public EnemyDeathState {
public:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	void Exit(UEnemyFSM* FSM) override;
};
UCLASS()
class EXOQUEST_API UEnemy2FSM : public UEnemyFSM
{
	GENERATED_BODY()

public:
	UEnemy2FSM();
	Enemy2IdleState   Enemy2IdleState;
	Enemy2PatrolState Enemy2PatrolState;
	Enemy2MoveState   Enemy2MoveState;
	Enemy2AttackState Enemy2AttackState;
	Enemy2DamageState Enemy2DamageState;
	Enemy2DeathState  Enemy2DeathState;

protected:
	virtual void BeginPlay() override;

	float idleTime = 5.f;
};
