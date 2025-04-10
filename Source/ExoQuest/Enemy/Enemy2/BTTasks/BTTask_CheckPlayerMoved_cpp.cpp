//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Enemy/Enemy2/BTTasks/BTTask_CheckPlayerMoved_cpp.h"
//#include "BehaviorTree/BlackboardComponent.h"
//
//UBTTask_CheckPlayerMoved_cpp::UBTTask_CheckPlayerMoved_cpp()
//{
//	NodeName = TEXT("Check Player Moved");
//	Interval = 0.5f;
//	RandomDeviation = 0.f;
//}
//
//void UBTTask_CheckPlayerMoved_cpp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	auto* BB = OwnerComp.GetBlackboardComponent();
//	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
//	if (!TargetActor) return;
//
//	FVector CurrentLocation = TargetActor->GetActorLocation();
//	FVector LastSeen = BB->GetValueAsVector("LastSeenLocation");
//
//	// 거리 차이가 일정 이상일 경우 Cover 재탐색 트리거
//	if (FVector::DistSquared(CurrentLocation, LastSeen) > FMath::Square(MovementThreshold))
//	{
//		//  이전 위치 갱신
//		BB->SetValueAsVector("LastSeenLocation", CurrentLocation);
//
//		// Cover와 PeekPoint 삭제 → 다음 Tick에서 다시 FindCover 실행됨
//		BB->ClearValue("Cover");
//		BB->ClearValue("PeekPoint");
//	}
//}
