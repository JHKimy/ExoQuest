#include "Enemy/Enemy2/BTTasks/Task_RandomIdleMotion.h"
#include "BehaviorTree/BlackboardComponent.h"

UTask_RandomIdleMotion::UTask_RandomIdleMotion()
{
	NodeName = TEXT("Random Idle Index");
}

void UTask_RandomIdleMotion::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
	{
		int32 RandomIndex = FMath::RandRange(0, MaxIndex);
		BB->SetValueAsInt(GetSelectedBlackboardKey(), RandomIndex);
	}
}
