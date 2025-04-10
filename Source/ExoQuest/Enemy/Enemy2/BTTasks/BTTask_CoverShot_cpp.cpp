// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy2/BTTasks/BTTask_CoverShot_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "TimerManager.h"
#include "NavigationSystem.h"


UBTTask_CoverShot_cpp::UBTTask_CoverShot_cpp()
{
    NodeName = TEXT("Cover Shot");
    bNotifyTick = true;
    bNotifyTaskFinished = true;
}
EBTNodeResult::Type UBTTask_CoverShot_cpp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIPawn || !BB || !AIController) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;

	// 초기 상태: Cover로 이동
	MoveState = ECoverMoveState::ToCover;
	bArrived = false;

	const FVector CoverLocation = BB->GetValueAsVector("Cover");
	MoveTo(AIController, CoverLocation);

	return EBTNodeResult::InProgress;

}

void UBTTask_CoverShot_cpp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIPawn || !BB || !AIController) return;

	const FVector CurrentLocation = AIPawn->GetActorLocation();
	const FVector TargetLocation = (MoveState == ECoverMoveState::ToCover)
		? BB->GetValueAsVector("Cover")
		: BB->GetValueAsVector("PeekPoint");

	if (FVector::DistSquared(CurrentLocation, TargetLocation) < FMath::Square(50.f) && !bArrived)
	{
		bArrived = true;

		if (MoveState == ECoverMoveState::ToPeek)
		{
			Fire(AIPawn);
		}

		// 다음 이동을 잠시 대기 후 실행
		AIPawn->GetWorldTimerManager().SetTimer(
			MoveTimerHandle,
			[this, AIController, BB]()
			{
				if (MoveState == ECoverMoveState::ToCover)
				{
					MoveState = ECoverMoveState::ToPeek;
					bArrived = false;
					MoveTo(AIController, BB->GetValueAsVector("PeekPoint"));
				}
				else
				{
					MoveState = ECoverMoveState::ToCover;
					bArrived = false;
					MoveTo(AIController, BB->GetValueAsVector("Cover"));
				}
			},
			1.5f, false
		);
	}
}
void UBTTask_CoverShot_cpp::Fire(APawn* AIPawn)
{
    if (AEnemy2* Enemy = Cast<AEnemy2>(AIPawn))
    {
        Enemy->FireRifle();
    }
}

void UBTTask_CoverShot_cpp::MoveTo(class AAIController* AIController, const FVector& TargetLocation){

    AIController->MoveToLocation(TargetLocation, 10.f, true, true, true, false, 0, true);

}
