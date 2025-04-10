// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CoverShot_cpp.generated.h"

UCLASS(Blueprintable, meta = (DisplayName = "Cover Shot"))
class EXOQUEST_API UBTTask_CoverShot_cpp : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_CoverShot_cpp();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    void Fire(APawn* AIPawn);
    void MoveTo(class AAIController* AIController, const FVector& TargetLocation);
};
