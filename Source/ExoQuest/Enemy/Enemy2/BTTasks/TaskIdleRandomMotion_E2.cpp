#include "Enemy/Enemy2/BTTasks/TaskIdleRandomMotion_E2.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UTaskIdleRandomMotion_E2::UTaskIdleRandomMotion_E2()
{
	NodeName = TEXT("Random Idle Motion");

	// Blackboard에서 사용할 키 초기화 가능
	IdleIndexKey.SelectedKeyName = "IdleIndex";
}

EBTNodeResult::Type UTaskIdleRandomMotion_E2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	int32 RandomIndex = FMath::RandRange(0, MaxIndex); // 0 ~ MaxIndex
	BB->SetValueAsInt(IdleIndexKey.SelectedKeyName, RandomIndex);

	// 로그 확인용
	UE_LOG(LogTemp, Warning, TEXT("Random Idle Index selected: %d"), RandomIndex);

	return EBTNodeResult::Succeeded;
}
