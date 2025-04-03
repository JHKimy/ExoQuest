#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h"
#include "Enemy1.generated.h"

UCLASS()
class EXOQUEST_API AEnemy1 : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 손에 충돌체
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* AttackCollision;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UEnemy1AnimInstance* anim;

	UFUNCTION()
	void SetAnimState(EEnemyState NewState);

	UFUNCTION()
	void SetAttackPlay(bool b);

	// 손에 맞으면 실행
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = Damage)
	bool bCanDamage = true;


	void Death() override;

	void MoveToTarget() override;

	UFUNCTION()
	void MoveToPatrolPoint();
	
	// 순찰용 경로 리스트
	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	int32 CurrentPatrolIndex = 0;

	UPROPERTY(EditAnywhere, Category = Detect)
	float detectRange = 1000.f;
	FORCEINLINE float GetDetectRange() {
		return detectRange;
	}

	UPROPERTY(EditAnywhere, Category = Detect)
	float attackRange = 150.f;
	FORCEINLINE float GetAttackRange() {
		return attackRange;
	}

	UPROPERTY(EditAnywhere, Category = "Detect")
	float ViewAngle = 90.f; // 시야각 90도 (좌우 45도씩)
	UFUNCTION()
	bool IsPlayerInSight();
	//UPROPERTY(EditAnywhere, Category = "Patrol")
	//float PatrolMoveSpeed = 150.f;  // 기본 속도 (예: 150)

	// 되돌아가기
	UPROPERTY(VisibleAnywhere, Category = "Return")
	bool bReturningToOrigin = false;

	UPROPERTY(VisibleAnywhere, Category = "Return")
	FVector InitialPosition;
	UFUNCTION()
	void ReturnToPatrolLocation();


	class AAIController* AI;

};
