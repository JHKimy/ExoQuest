// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Starflux.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"
#include "UI/InventoryUI.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/HUD.h" // AHUD 포함




AStarflux::AStarflux()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐, 메쉬설정
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	RootComponent = capsuleComp;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/Starflux/SM_Starflux.SM_Starflux'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 충돌 연동
	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AStarflux::OnBeginOverlap);
	
	// floating 관련 변수
	Amplitude = 15.f;
	RotationSpeed = 1.f;
	// 팝업 애니메이션 시작
	ElapsedTime = 0.f; // 애니메이션 진행 시간 초기화
	bIsPopping = true; // 팝업 상태 활성화


	// ItemDataBase 변수 설정
	ItemName = TEXT("StarFlux");
	ItemNum = 1;

	//character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void AStarflux::BeginPlay()
{
	Super::BeginPlay();


	// 캐릭터 초기화
	character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!character)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NODetected")));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Detected Character")));
	}

	// 
	StartPos = GetActorLocation();



	GetWorldTimerManager().SetTimer(
		DistanceCheckTimerHandle,
		this,
		&AStarflux::CheckForNearbyCharacters,
		CheckInterval,
		true
	);

}

void AStarflux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case EStarfluxState::Pop:
		Pop(DeltaTime);
		break;

	case EStarfluxState::Floating:
		Floating(DeltaTime);
		break;

	case EStarfluxState::Absorbing:
		AbsorbToCharacter(DeltaTime);
		break;

	default:
		break;
	}


	//// 캐릭터와의 거리로 근처 여부 판단
	//if (NearbyCharacter && bCanAbsorb)
	//{
		//float Distance = FVector::Dist(GetActorLocation(), NearbyCharacter->GetActorLocation());
		//IsCharacterNearby = (Distance < 200.f); // 200.f는 근처 거리 기준

		//if (IsCharacterNearby)
		//{
		//	AbsorbToCharacter(DeltaTime);
		//}
	//}
	
}

void AStarflux::Pop(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// 팝업 애니메이션 지속 시간 (초)
	const float PopDuration = 2.f;

	// 애니메이션 진행 비율 (0 ~ 1)
	float Alpha = FMath::Clamp(ElapsedTime / PopDuration, 0.f, 1.f);

	// Ease-In-Out 보간 적용
	float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 3.f); // 2.f는 곡률 조정값


	StartBelowLocation = StartPos;

	StartBelowLocation.Z -= 150.f; // 아래로 100 유닛
	SetActorLocation(StartBelowLocation);


	// 위치 계산
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = FMath::Lerp(StartBelowLocation.Z, StartPos.Z, EasedAlpha);
	SetActorLocation(CurrentLocation);

	if (ElapsedTime >= PopDuration)
	{
		CurrentState = EStarfluxState::Floating; // 상태 전환
		ElapsedTime = 0.f;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Popup Complete, Floating Start"));
	}
}

void AStarflux::Floating(float DeltaTime)
{
	FVector NewLocation = StartPos;
	float Time = GetWorld()->GetTimeSeconds();

	NewLocation.Z += FMath::Sin(Time) * Amplitude;

	SetActorLocation(NewLocation);
	AddActorLocalRotation(FRotator(0, RotationSpeed, 0));

	// 3초 후 흡수 상태로 전환
	TimeSinceStart += DeltaTime;
	if (TimeSinceStart >= 3.0f)
	{
		CurrentState = EStarfluxState::Absorbing; // 상태 전환
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Ready to Absorb!"));
	}
}

void AStarflux::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// 캐릭터가 Overlap에 들어왔는지 확인
	if (ACharacterBase* EquippedCharacter = Cast<ACharacterBase>(OtherActor))
	{
		
		EquippedCharacter->ItemDataBase->AddItem(ItemName, ItemNum);

		EquippedCharacter->InventoryUI->UpdateInventory();

		// 아이템을 맵에서 제거
		Destroy();
	}
}


void AStarflux::CheckForNearbyCharacters()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), FoundActors);

	IsCharacterNearby = false;
	character = nullptr;

	FVector CurrentLocation = GetActorLocation();
	float ClosestDistance = MAX_FLT;

	for (AActor* Actor : FoundActors)
	{
		ACharacterBase* DetectedCharacter = Cast<ACharacterBase>(Actor);
		if (DetectedCharacter)
		{
			float Distance = FVector::Dist(CurrentLocation, DetectedCharacter->GetActorLocation());
			if (Distance < 100.0f && Distance < ClosestDistance)
			{
				IsCharacterNearby = true;
				character = DetectedCharacter;
				ClosestDistance = Distance;
			}
		}
	}

}


void AStarflux::AbsorbToCharacter(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0, RotationSpeed, 0));


	if (!character)
	{
		character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No Character Found"));
			return;
		}
	}

	// 캐릭터와의 거리 계산
	float DistanceToCharacter = FVector::Dist(GetActorLocation(), character->GetActorLocation());

	// 일정 거리 이내에 있을 때만 흡수 실행
	if (DistanceToCharacter < AbsorptionDistance) {

		FVector CurrentLocation = GetActorLocation();

		FVector CharacterLocation = character->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, CharacterLocation, DeltaTime, MoveToCharacterSpeed);
		SetActorLocation(NewLocation);
	}
	
}