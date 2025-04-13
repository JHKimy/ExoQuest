#include "Enemy/Enemy2/BTTasks/BTTask_FindCover_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Perception/PawnSensingComponent.h"

UBTTask_FindCover_cpp::UBTTask_FindCover_cpp()
{
	NodeName = TEXT("Find Cover and Peek Position");
	bNotifyTick = true; // Tick 활성화
}

EBTNodeResult::Type UBTTask_FindCover_cpp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UWorld* World = AIPawn->GetWorld();
	AActor* Player = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	AEnemy2* Enemy = Cast<AEnemy2>(AIPawn);

	// 주변 원 범위 안에 Cover Tag 액터 찾기
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AIPawn); // 자기 자신 제외

	float SearchRadius = 1000.f;

	// Sphere Overlap
	bool bFound = World->OverlapMultiByChannel(
		Overlaps,
		AIPawn->GetActorLocation(),
		FQuat::Identity,
		ECC_WorldStatic, // ECC_WorldStatic
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams
	);

	AActor* NearestCover = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	if (bFound)
	{
		for (const FOverlapResult& Result : Overlaps)
		{
			AActor* Actor = Result.GetActor();
			if (Actor && Actor->ActorHasTag("Cover"))
			{
				// 가장 가까운 액터 고르기
				float Dist = FVector::DistSquared(AIPawn->GetActorLocation(), Actor->GetActorLocation());
				if (Dist < MinDistance)
				{
					MinDistance = Dist;
					NearestCover = Actor;
				}
			}
		}
	}

	if (NearestCover)
	{
		// 플레이어와 적 사이의 정중앙 위치 계산
		//FVector MidPoint = (AIPawn->GetActorLocation() + Player->GetActorLocation()) * 0.5f;
		FVector MidPoint = NearestCover->GetActorLocation();

		FVector PlayerToCover = MidPoint - Player->GetActorLocation(); // 플레이어 → Cover
		FVector OppositeDirection = PlayerToCover.GetSafeNormal();
		FVector CoverLocation = MidPoint + OppositeDirection * 300.f;



		// PeekPoint 계산 시작
		FVector RightVector = FVector::CrossProduct(OppositeDirection, FVector::UpVector); // Cover 기준 오른쪽
		//FVector RightVector = AIPawn->GetActorRightVector(); // Cover 기준 오른쪽
		FVector PeekRight = CoverLocation + RightVector * 700.f;
		FVector PeekLeft = CoverLocation - RightVector * 700.f;

		UPawnSensingComponent* Sensing = Enemy->FindComponentByClass<UPawnSensingComponent>();
		FVector ChosenPeek = FVector::ZeroVector;

		if (Sensing)
		{
			bool bCanSeeRight = CanSensePlayerFromLocation(Sensing, PeekRight, Player);
			bool bCanSeeLeft = CanSensePlayerFromLocation(Sensing, PeekLeft, Player);

			if (bCanSeeRight && bCanSeeLeft)
			{
				static bool bToggle = false;
				ChosenPeek = bToggle ? PeekRight : PeekLeft;
				bToggle = !bToggle;
			}
			else if (bCanSeeRight)
			{
				ChosenPeek = PeekRight;
				CurrentPeekDirection = EPeekDirection::PeekRight;
			}
			else if (bCanSeeLeft)
			{
				ChosenPeek = PeekLeft;
				CurrentPeekDirection = EPeekDirection::PeekLeft;
			}
		}
		if (ChosenPeek == PeekRight) {
			CurrentPeekDirection = EPeekDirection::PeekRight;
		}
		else if (ChosenPeek == PeekLeft) {
			CurrentPeekDirection = EPeekDirection::PeekLeft;
		}


		// 블랙보드 저장
		BB->SetValueAsVector("Cover", CoverLocation);
		BB->SetValueAsVector("PeekPoint", ChosenPeek);
		BB->SetValueAsInt("CoverValid", 1);
		BB->SetValueAsInt("PeekDirection", static_cast<int>(CurrentPeekDirection));

		// 디버그 시각화
		DrawDebugSphere(World, CoverLocation, 20.f, 12, FColor::Green, false, 1.f);
		DrawDebugSphere(World, PeekRight, 15.f, 8, FColor::Red, false, 1.f);
		DrawDebugSphere(World, PeekLeft, 15.f, 8, FColor::Blue, false, 1.f);
		DrawDebugSphere(World, ChosenPeek, 20.f, 10, FColor::Yellow, false, 1.f);

		return EBTNodeResult::Succeeded;
	}
	else
	{
		//BB->ClearValue("Cover");
		//BB->ClearValue("PeekPoint");
		BB->SetValueAsInt("CoverValid", 0);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Succeeded;




	//// 플레이어 방향으로 레이캐스트 → Cover 확인
	//FVector ToPlayer = Player->GetActorLocation() - AIPawn->GetActorLocation();
	//FVector DirToPlayer = ToPlayer.GetSafeNormal();

	//FHitResult HitResult;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(Enemy);

	//bool bHit = World->LineTraceSingleByChannel(
	//	HitResult,
	//	AIPawn->GetActorLocation(),
	//	AIPawn->GetActorLocation() + DirToPlayer * 2000.f,
	//	ECC_Visibility,
	//	Params
	//);





	//if (HitResult.GetActor()->ActorHasTag("Cover"))
	//{
	//	AActor* CoverActor = HitResult.GetActor();
	//	FVector CoverLocation = CoverActor->GetActorLocation();

	//	// 플레이어 → Cover 방향의 반대 방향으로 적 이동 위치 설정
	//	FVector ToCoverFromPlayer = CoverLocation - Player->GetActorLocation();
	//	FVector OppositeDirection = ToCoverFromPlayer.GetSafeNormal();
	//	FVector DesiredEnemyPos = CoverLocation + OppositeDirection * 300.f;

	//	AIPawn->SetActorLocation(FMath::VInterpTo(AIPawn->GetActorLocation(), DesiredEnemyPos, DeltaSeconds, 5.f));

	//	BB->SetValueAsVector("Cover", CoverLocation);
	//	BB->SetValueAsInt("CoverValid", 1);
	//}





	//return EBTNodeResult::Type();
}

void UBTTask_FindCover_cpp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	//UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	//UWorld* World = AIPawn->GetWorld();
	//AActor* Player = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	//AEnemy2* Enemy = Cast<AEnemy2>(AIPawn);

	//if (!Enemy || !Player) return;

	//UPawnSensingComponent* Sensing = Enemy->FindComponentByClass<UPawnSensingComponent>();
	//if (!Sensing) return;

	//// 플레이어 방향으로 레이캐스트 → Cover 확인
	//FVector ToPlayer = Player->GetActorLocation() - AIPawn->GetActorLocation();
	//FVector DirToPlayer = ToPlayer.GetSafeNormal();

	//FHitResult HitResult;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(Enemy);

	//bool bHit = World->LineTraceSingleByChannel(
	//	HitResult,
	//	AIPawn->GetActorLocation(),
	//	AIPawn->GetActorLocation() + DirToPlayer * 2000.f,
	//	ECC_Visibility,
	//	Params
	//);

	//if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Cover"))
	//{
	//	AActor* CoverActor = HitResult.GetActor();
	//	FVector CoverLocation = CoverActor->GetActorLocation();

	//	// 플레이어 → Cover 방향의 반대 방향으로 적 이동 위치 설정
	//	FVector ToCoverFromPlayer = CoverLocation - Player->GetActorLocation();
	//	FVector OppositeDirection = ToCoverFromPlayer.GetSafeNormal();
	//	FVector DesiredEnemyPos = CoverLocation + OppositeDirection * 300.f;

	//	AIPawn->SetActorLocation(FMath::VInterpTo(AIPawn->GetActorLocation(), DesiredEnemyPos, DeltaSeconds, 5.f));

	//	BB->SetValueAsVector("Cover", CoverLocation);
	//	BB->SetValueAsInt("CoverValid", 1);

	//	// PeekPoint 계산: 적 기준 좌우
	//	FVector Right = FVector::CrossProduct(ToCoverFromPlayer.GetSafeNormal(), FVector::UpVector);
	//	FVector PeekRight = AIPawn->GetActorLocation() + Right * 200.f;
	//	FVector PeekLeft = AIPawn->GetActorLocation() - Right * 200.f;

	//	bool bCanSeeRight = CanSensePlayerFromLocation(Sensing, PeekRight, Player);
	//	bool bCanSeeLeft = CanSensePlayerFromLocation(Sensing, PeekLeft, Player);

	//	FVector ChosenPeek = FVector::ZeroVector;
	//	static bool bPeekToggle = false;

	//	if (bCanSeeRight && bCanSeeLeft)
	//	{
	//		ChosenPeek = bPeekToggle ? PeekRight : PeekLeft;
	//		bPeekToggle = !bPeekToggle;
	//	}
	//	else if (bCanSeeRight)
	//	{
	//		ChosenPeek = PeekRight;
	//	}
	//	else if (bCanSeeLeft)
	//	{
	//		ChosenPeek = PeekLeft;
	//	}

	//	BB->SetValueAsVector("PeekPoint", ChosenPeek);

	//	// 디버그 시각화
	//	DrawDebugSphere(World, CoverLocation, 30.f, 12, FColor::Green, false, 0.1f);
	//	DrawDebugSphere(World, PeekRight, 15.f, 8, FColor::Red, false, 0.1f);
	//	DrawDebugSphere(World, PeekLeft, 15.f, 8, FColor::Blue, false, 0.1f);
	//}
	//else
	//{
	//	// Cover가 더 이상 없으면 초기화
	//	BB->ClearValue("Cover");
	//	BB->ClearValue("PeekPoint");
	//	BB->SetValueAsInt("CoverValid", 0);
	//}
}

bool UBTTask_FindCover_cpp::CanSensePlayerFromLocation(UPawnSensingComponent* Sensing, const FVector& FromLocation, AActor* Player)
{
	if (!Sensing || !Player) return false;

	FVector ToPlayer = Player->GetActorLocation() - FromLocation;
	float Distance = ToPlayer.Size();
	//if (Distance > Sensing->SightRadius) return false;

	FVector Direction = ToPlayer.GetSafeNormal();
	FVector Forward = Sensing->GetOwner()->GetActorForwardVector();
	float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Forward, Direction)));

	return Angle <= Sensing->GetPeripheralVisionAngle();
}