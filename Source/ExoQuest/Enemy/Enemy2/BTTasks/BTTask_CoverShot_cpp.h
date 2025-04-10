#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CoverShot_cpp.generated.h"
UENUM()
enum class ECoverMoveState : uint8
{
    ToCover,
    ToPeek
};
UCLASS(Blueprintable, meta = (DisplayName = "Cover Shot"))
class EXOQUEST_API UBTTask_CoverShot_cpp : public UBTTaskNode
{
	GENERATED_BODY()
public:
    UBTTask_CoverShot_cpp();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void MoveTo(class AAIController* AIController, const FVector& TargetLocation);
	void Fire(APawn* AIPawn);

	// 상태를 저장할 용도
	ECoverMoveState MoveState;
	bool bArrived = false;

	FTimerHandle MoveTimerHandle;
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};
