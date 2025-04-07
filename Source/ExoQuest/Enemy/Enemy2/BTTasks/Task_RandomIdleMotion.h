// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Task_RandomIdleMotion.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API UTask_RandomIdleMotion : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UTask_RandomIdleMotion();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Random")
	int32 MaxIndex = 2; // 0 ~ MaxIndex 사이에서 선택됨
	
};
