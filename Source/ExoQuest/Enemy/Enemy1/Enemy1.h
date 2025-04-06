#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyBase.h"
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

public:
	// 손에 충돌체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UCapsuleComponent* AttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UEnemy1AnimInstance* anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AAIController* AI;

	//==================================================================================================
	// Stats
	//==================================================================================================
	
	// 순찰용 경로 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	int32 CurrentPatrolIndex = 0;
	
	// 되돌아가기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	bool bReturningToOrigin = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	FVector InitialPosition;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect")
	float ViewAngle = 90.f; // 시야각 90도 (좌우 45도씩)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect")
	float detectRange = 1000.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackPower = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bCanDamage = true;



	//==================================================================================================
	// Method
	//==================================================================================================
public:

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetAnimState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetAttackPlay(bool attackPlay);

	// 손에 맞으면 실행
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void Death() override;

	void MoveToTarget() override;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void MoveToPatrolPoint();

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void ReturnToPatrolLocation();

	UFUNCTION(BlueprintCallable, Category = "Detect")
	bool IsPlayerInSight();


	//==================================================================================================
	// Inline
	//==================================================================================================
	FORCEINLINE float GetDetectRange() {return detectRange;}
	FORCEINLINE float GetAttackRange() {return attackRange;}

};
