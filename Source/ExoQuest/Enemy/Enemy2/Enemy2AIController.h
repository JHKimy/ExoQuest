// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy2AIController.generated.h"

UCLASS()
class EXOQUEST_API AEnemy2AIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemy2AIController();

	class UBlackboardComponent* BBComponent;

	UFUNCTION(BlueprintCallable)
	void RunIdleBT();

	UFUNCTION(BlueprintCallable)
	void RunPatrolBT();

	UFUNCTION(BlueprintCallable)
	void RunChaseBT();

	UFUNCTION(BlueprintCallable)
	void RunAttackBT();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	class UBehaviorTree* BT_Enemy2Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT_Enemy2Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT_Enemy2Chase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
	UBehaviorTree* BT_Enemy2Attack;
};
