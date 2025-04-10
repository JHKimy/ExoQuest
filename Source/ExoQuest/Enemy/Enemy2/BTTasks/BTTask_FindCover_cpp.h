// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindCover_cpp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (DisplayName = "Find Cover and Peek Position"))
class EXOQUEST_API UBTTask_FindCover_cpp : public UBTTaskNode
{
    GENERATED_BODY() // 클래스 선언 내부의 제일 위에 있어야 함

public:
    UBTTask_FindCover_cpp();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
    /* 플레이어가 피크 위치에서 보이는지 확인 */
    bool CanSensePlayerFromLocation(class UPawnSensingComponent* Sensing, const FVector& FromLocation, AActor* Player);
    //FVector GetMuzzleLocation(APawn* AIPawn);
    //bool CanSeePlayerFromMuzzle(UWorld* World, const FVector& MuzzleLocation, AActor* Player, AActor* IgnoredActor);
};
