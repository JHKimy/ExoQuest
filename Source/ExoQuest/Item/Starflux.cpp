// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Starflux.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"
#include "UI/InventoryUI.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/HUD.h" // AHUD ����




AStarflux::AStarflux()
{
	PrimaryActorTick.bCanEverTick = true;

	// ĸ��, �޽�����
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

	// �浹 ����
	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AStarflux::OnBeginOverlap);
	
	// floating ���� ����
	Amplitude = 15.f;
	RotationSpeed = 1.f;
	// �˾� �ִϸ��̼� ����
	ElapsedTime = 0.f; // �ִϸ��̼� ���� �ð� �ʱ�ȭ
	bIsPopping = true; // �˾� ���� Ȱ��ȭ


	// ItemDataBase ���� ����
	ItemName = TEXT("StarFlux");
	ItemNum = 1;

	//character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void AStarflux::BeginPlay()
{
	Super::BeginPlay();


	// ĳ���� �ʱ�ȭ
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


	//// ĳ���Ϳ��� �Ÿ��� ��ó ���� �Ǵ�
	//if (NearbyCharacter && bCanAbsorb)
	//{
		//float Distance = FVector::Dist(GetActorLocation(), NearbyCharacter->GetActorLocation());
		//IsCharacterNearby = (Distance < 200.f); // 200.f�� ��ó �Ÿ� ����

		//if (IsCharacterNearby)
		//{
		//	AbsorbToCharacter(DeltaTime);
		//}
	//}
	
}

void AStarflux::Pop(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// �˾� �ִϸ��̼� ���� �ð� (��)
	const float PopDuration = 2.f;

	// �ִϸ��̼� ���� ���� (0 ~ 1)
	float Alpha = FMath::Clamp(ElapsedTime / PopDuration, 0.f, 1.f);

	// Ease-In-Out ���� ����
	float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 3.f); // 2.f�� ��� ������


	StartBelowLocation = StartPos;

	StartBelowLocation.Z -= 150.f; // �Ʒ��� 100 ����
	SetActorLocation(StartBelowLocation);


	// ��ġ ���
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = FMath::Lerp(StartBelowLocation.Z, StartPos.Z, EasedAlpha);
	SetActorLocation(CurrentLocation);

	if (ElapsedTime >= PopDuration)
	{
		CurrentState = EStarfluxState::Floating; // ���� ��ȯ
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

	// 3�� �� ��� ���·� ��ȯ
	TimeSinceStart += DeltaTime;
	if (TimeSinceStart >= 3.0f)
	{
		CurrentState = EStarfluxState::Absorbing; // ���� ��ȯ
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Ready to Absorb!"));
	}
}

void AStarflux::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// ĳ���Ͱ� Overlap�� ���Դ��� Ȯ��
	if (ACharacterBase* EquippedCharacter = Cast<ACharacterBase>(OtherActor))
	{
		
		EquippedCharacter->ItemDataBase->AddItem(ItemName, ItemNum);

		EquippedCharacter->InventoryUI->UpdateInventory();

		// �������� �ʿ��� ����
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

	// ĳ���Ϳ��� �Ÿ� ���
	float DistanceToCharacter = FVector::Dist(GetActorLocation(), character->GetActorLocation());

	// ���� �Ÿ� �̳��� ���� ���� ��� ����
	if (DistanceToCharacter < AbsorptionDistance) {

		FVector CurrentLocation = GetActorLocation();

		FVector CharacterLocation = character->GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, CharacterLocation, DeltaTime, MoveToCharacterSpeed);
		SetActorLocation(NewLocation);
	}
	
}