#include "Enemy/Enemy2/BTTasks/BTTask_FindCover_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Perception/PawnSensingComponent.h"

UBTTask_FindCover_cpp::UBTTask_FindCover_cpp()
{
	NodeName = TEXT("Find Cover and Peek Position");
}

EBTNodeResult::Type UBTTask_FindCover_cpp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UWorld* World = AIPawn->GetWorld();
	AActor* Player = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
	AEnemy2* Enemy = Cast<AEnemy2>(AIPawn);

	if (!Enemy || !Player) return EBTNodeResult::Failed;

	UPawnSensingComponent* Sensing = Enemy->FindComponentByClass<UPawnSensingComponent>();
	if (!Sensing) return EBTNodeResult::Failed;

	// 플레이어와 적 사이에 장애물이 있는지 확인 (하나만)
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Enemy);
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		AIPawn->GetActorLocation(),
		Player->GetActorLocation(),
		ECC_Visibility,
		Params
	);

	if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Cover"))
	{
		AActor* Wall = HitResult.GetActor();

		// 벽에서 적 방향으로 100만큼 떨어진 Cover 위치 계산
		FVector ToEnemy = AIPawn->GetActorLocation() - Wall->GetActorLocation();
		FVector CoverLocation = Wall->GetActorLocation() + ToEnemy.GetSafeNormal() * 100.f;

		BB->SetValueAsVector("Cover", CoverLocation);
		BB->SetValueAsInt("CoverValid", 1); // Cover 유효

		// Peek 위치 계산 (Cover 기준 양옆)
		FVector ToPlayer = Player->GetActorLocation() - CoverLocation;
		FVector Right = FVector::CrossProduct(ToPlayer.GetSafeNormal(), FVector::UpVector);

		FVector PeekRight = CoverLocation + Right * 200.f;
		FVector PeekLeft = CoverLocation - Right * 200.f;

		// 감지 여부 확인
		bool bCanSenseRight = CanSensePlayerFromLocation(Sensing, PeekRight, Player);
		bool bCanSenseLeft = CanSensePlayerFromLocation(Sensing, PeekLeft, Player);

		// Peek 방향 선택
		static bool bPeekToggle = false;
		FVector ChosenPeek = FVector::ZeroVector;

		if (bCanSenseRight && bCanSenseLeft)
		{
			ChosenPeek = bPeekToggle ? PeekRight : PeekLeft;
			bPeekToggle = !bPeekToggle;
		}
		else if (bCanSenseRight)
		{
			ChosenPeek = PeekRight;
		}
		else if (bCanSenseLeft)
		{
			ChosenPeek = PeekLeft;
		}

		BB->SetValueAsVector("PeekPoint", ChosenPeek);

		return EBTNodeResult::Succeeded;
	}

	// 적절한 Cover가 없으면 초기화
	BB->ClearValue("Cover");
	BB->ClearValue("PeekPoint");
	BB->SetValueAsInt("CoverValid", 0); // Cover 비유효
	return EBTNodeResult::Succeeded;
}

bool UBTTask_FindCover_cpp::CanSensePlayerFromLocation(UPawnSensingComponent* Sensing, const FVector& FromLocation, AActor* Player)
{
	if (!Sensing || !Player) return false;

	FVector ToPlayer = Player->GetActorLocation() - FromLocation;
	float Distance = ToPlayer.Size();
	if (Distance > Sensing->SightRadius) return false;

	FVector Direction = ToPlayer.GetSafeNormal();
	FVector Forward = Sensing->GetOwner()->GetActorForwardVector();
	float Angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(Forward, Direction)));
	return Angle <= Sensing->GetPeripheralVisionAngle();
}
