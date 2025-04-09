// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy2.generated.h"


UCLASS()
class EXOQUEST_API AEnemy2 : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy2();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UEnemy2AnimInstance* anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AAIController* AI;
	




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsPlayerDetected = false;

	//// Enemy2.h
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	//APawn* DetectedPawn = nullptr;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);





	//  Enemy2의 라이플 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle")
	TObjectPtr<class AEnemyRifle> EnemyRifle;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class AEnemyRifle> RifleClass_BP;


	// IK 사용
	UPROPERTY(BlueprintReadOnly)
	FVector LeftHandIKPosition;

	UPROPERTY(BlueprintReadOnly)
	FRotator LeftHandIKRotation;

	UFUNCTION(BlueprintCallable, Category = "IK")
	void UpdateLeftHandIK();

	UFUNCTION(BlueprintCallable, Category = "Rotate")
	void RotateToTarget();

	// 총을 부착할 소켓 이름 (스켈레탈 메시 본 기준)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle")
	FName RifleSocketName = "RightHandSocket";


	 UFUNCTION(BlueprintCallable, Category = "Animation")
	 void SetAnimState(EEnemyState NewState);

	 UFUNCTION(BlueprintCallable, Category = "Fire")
	 void FireRifle();

	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void Death() override;

	void SetTarget(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle")
	float idleTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	float patrolTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
	float fireDistance = 700.f;

	float bDamaged = false;
};
