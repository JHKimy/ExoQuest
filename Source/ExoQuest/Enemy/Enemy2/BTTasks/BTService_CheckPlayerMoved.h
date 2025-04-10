// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerMoved.generated.h"

UCLASS(Blueprintable) // 이거 반드시 추가
class EXOQUEST_API UBTService_CheckPlayerMoved : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckPlayerMoved();

protected:
	virtual void TickNode(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	const float MovementThreshold = 200.f;
	
};
