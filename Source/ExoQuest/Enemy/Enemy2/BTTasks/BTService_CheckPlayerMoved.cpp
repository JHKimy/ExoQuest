// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy2/BTTasks/BTService_CheckPlayerMoved.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckPlayerMoved::UBTService_CheckPlayerMoved()
{
	NodeName = TEXT("Check Player Moved");
	this->Interval = 0.5f;
	this->RandomDeviation = 0.f;
}

void UBTService_CheckPlayerMoved::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	if (!TargetActor) return;

	FVector CurrentLocation = TargetActor->GetActorLocation();
	FVector LastSeen = BB->GetValueAsVector("LastSeenLocation");

	if (FVector::DistSquared(CurrentLocation, LastSeen) > FMath::Square(MovementThreshold))
	{
		BB->SetValueAsVector("LastSeenLocation", CurrentLocation);
		BB->ClearValue("Cover");
		BB->ClearValue("PeekPoint");
		// BB->SetValueAsInt("CoverValid", 0); // Cover ∫Ò¿Ø»ø
	}
}
