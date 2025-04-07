// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskIdleRandomMotion_E2.generated.h"

UCLASS(BlueprintType)
class EXOQUEST_API UTaskIdleRandomMotion_E2 : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTaskIdleRandomMotion_E2();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Idle")
	int32 MaxIndex = 2; // 무작위 범위 설정 (0~2)

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector IdleIndexKey;
};
