#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"



// 겹치는 이름을 막기 위해 class 선언
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle		UMETA(DisplayName = "Idle State"),
	Move		UMETA(DisplayName = "Move State"),
	Attack		UMETA(DisplayName = "Attack State"),
	Damage		UMETA(DisplayName = "Damage State"),
	Die			UMETA(DisplayName = "Die State")
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


	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState EState = EEnemyState::Idle;



	// 대기 상태
	void IdleState();

	// 이동 상태
	void MoveState();

	// 공격 상태
	void AttackState();

	// 피격 상태
	void DamageState();

	// 죽음 상태
	void DieState();

	// 피격 알림 이벤트
	void OnDamageProcess();


	// 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float idleDelayTIme = 2;

	// 경과 시간
	float currentTime = 0;

	// 타겟
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ACharacterBase* target;

	// 적
	UPROPERTY()
	class AEnemyBase* enemy;

	// 공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.f;

	// 공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;

	// 피격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;

	// 아래로 사라짐
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;



	// 무기 인스턴스 
	class ARifle* rifleInstance;
	class AShotgun* shotgunInstance;
	//class ARocketLauncher* rocketLauncherInstance;
	class ARocketProjectile* rocketLauncherInstance;
	class ASword* swordInstance;

	// 무기별 데미지
	UPROPERTY(EditAnywhere, Category = Damage)
	float rifleDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float shotgunDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float rocketLaunchetDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float swordDamage;


	// 무기 데미지 업데이트 함수
	void UpdateWeaponDamage();


};
