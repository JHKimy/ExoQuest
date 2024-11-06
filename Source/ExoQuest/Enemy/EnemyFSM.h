#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"



// ��ġ�� �̸��� ���� ���� class ����
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


	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState EState = EEnemyState::Idle;



	// ��� ����
	void IdleState();

	// �̵� ����
	void MoveState();

	// ���� ����
	void AttackState();

	// �ǰ� ����
	void DamageState();

	// ���� ����
	void DieState();

	// �ǰ� �˸� �̺�Ʈ
	void OnDamageProcess();


	// ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float idleDelayTIme = 2;

	// ��� �ð�
	float currentTime = 0;

	// Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ACharacterBase* target;

	// ��
	UPROPERTY()
	class AEnemyBase* enemy;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.f;

	// ���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackDelayTime = 2.0f;

	// �ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;

	// �Ʒ��� �����
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;



	// ���� �ν��Ͻ� 
	class ARifle* rifleInstance;
	class AShotgun* shotgunInstance;
	//class ARocketLauncher* rocketLauncherInstance;
	class ARocketProjectile* rocketLauncherInstance;
	class ASword* swordInstance;

	// ���⺰ ������
	UPROPERTY(EditAnywhere, Category = Damage)
	float rifleDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float shotgunDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float rocketLaunchetDamage;

	UPROPERTY(EditAnywhere, Category = Damage)
	float swordDamage;


	// ���� ������ ������Ʈ �Լ�
	void UpdateWeaponDamage();


};
