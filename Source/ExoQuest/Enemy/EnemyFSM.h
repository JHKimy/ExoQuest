#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Patrol,
	Move,
	Attack,
	Damage,
	Death
};

class UEnemyFSM;

class IEnemyState {
public:
	virtual void Enter(UEnemyFSM* FSM) = 0;
	virtual void Update(UEnemyFSM* FSM, float DeltaTime) = 0;
	virtual void Exit(UEnemyFSM* FSM) = 0;
};

class EnemyIdleState : public IEnemyState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};
class EnemyPatrolState : public IEnemyState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};
class EnemyMoveState : public IEnemyState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};
class EnemyAttackState : public IEnemyState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};
class EnemyDamageState : public IEnemyState {
protected:
	virtual void Enter(UEnemyFSM* FSM) override;

	virtual void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};
class EnemyDeathState : public IEnemyState {
protected:
	virtual  void Enter(UEnemyFSM* FSM) override;

	virtual  void Update(UEnemyFSM* FSM, float DeltaTime) override;

	virtual void Exit(UEnemyFSM* FSM) override;
};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXOQUEST_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyFSM();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ChangeState(EEnemyState NewState);

public:

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ACharacterBase* target;

	UPROPERTY()
	class AEnemyBase* enemy;

	UPROPERTY()
	class AEnemy1* enemy1;
	
	UPROPERTY()
	class AEnemy2* enemy2;

	UPROPERTY()
	class AEnemy2AIController* enemy2AIController;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UEnemy1AnimInstance* anim;
	
	virtual float GetDistanceToTarget();

	//// 플레이어 감지 시 FSM이 상태 전이 처리
	//virtual void OnPlayerDetected(APawn* DetectedPawn);

	//// Target 저장 함수
	//void SetTarget(APawn* NewTarget);

	//APawn* GetTarget() const;

protected:

	IEnemyState* CurrentState = nullptr;
	EEnemyState CurrentStateType;
	TMap<EEnemyState, IEnemyState*> StateMap;

	// 상태 객체 미리 생성
	EnemyIdleState IdleState;
	EnemyPatrolState PatrolState;
	EnemyMoveState MoveState;
	EnemyAttackState AttackState;
	EnemyDamageState DamageState;
	EnemyDeathState DeathState;

	float currentTime = 0.f;

public:
	void ResetTimer();
	void AddTimer(float DeltaTime);
	float GetTimer() const;
};
