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
    bNotifyTick = false;
    bNotifyTaskFinished = true;
}
EBTNodeResult::Type UBTTask_CoverShot_cpp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AAIController* AIController = OwnerComp.GetAIOwner();

    if (!AIPawn || !BB || !AIController) return EBTNodeResult::Failed;

    const FVector CoverLocation = BB->GetValueAsVector("Cover");
    const FVector PeekLocation = BB->GetValueAsVector("PeekPoint");


    // FHitResult Hit;
    // FCollisionQueryParams Params;
    // Params.AddIgnoredActor(AIPawn);
    // bool bHit = World->LineTraceSingleByChannel(
    //     Hit,
    //     GetMuzzleLocation(AIPawn),
    //     Player->GetActorLocation(),
    //     ECC_Visibility,
    //     Params
    // );
    // DrawDebugLine(World, GetMuzzleLocation(AIPawn), Player->GetActorLocation(), bHit ? FColor::Red : FColor::Green, false, 1.f, 0, 2.f);



    MoveTo(AIController, CoverLocation);

    return EBTNodeResult::InProgress;

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
