// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Starflux.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"
#include "UI/InventoryUI.h"
#include <Kismet/GameplayStatics.h>





AStarflux::AStarflux()
{
	PrimaryActorTick.bCanEverTick = true;

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



	Amplitude = 15.f;
	RotationSpeed = 1.f;




	// 기본 값 설정
	ItemName = TEXT("StarFlux");
	ItemImage = nullptr; // 블루프린트에서 설정 가능
}

void AStarflux::BeginPlay()
{
	Super::BeginPlay();
	
	StartPos = GetActorLocation();

	
	// 팝업 애니메이션 시작
	ElapsedTime = 0.f; // 애니메이션 진행 시간 초기화
	bIsPopping = true; // 팝업 상태 활성화

}

void AStarflux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPopping)
	{
		Pop(DeltaTime); // 팝업 애니메이션
	}
	else
	{
		Floating(DeltaTime); // 부유 효과
	}
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

	// 팝업 애니메이션 종료 조건
	if (ElapsedTime >= PopDuration)
	{
		bIsPopping = false; // 팝업 종료
		ElapsedTime = 0.f;  // 시간 초기화
		// 부유 애니메이션의 시작점을 현재 위치로 설정
		StartPos = CurrentLocation - 10.f; // 부유 효과와 매끄럽게 연결
	}
}

void AStarflux::Floating(float DeltaTime)
{
	FVector NewLocation = StartPos;
	float Time = GetWorld()->GetTimeSeconds();

	NewLocation.Z += FMath::Sin(Time) * Amplitude;

	SetActorLocation(NewLocation);
	AddActorLocalRotation(FRotator(0, RotationSpeed, 0));
}

void AStarflux::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// 캐릭터가 Overlap에 들어왔는지 확인
	if (Cast<ACharacterBase>(OtherActor))
	{
		// 데이터베이스에 접근
		const UItemDataBase* ConstGlobalItemDataBase = GetDefault<UItemDataBase>();

		if (ConstGlobalItemDataBase)
		{
			// const_cast로 const 제거
			UItemDataBase* GlobalItemDataBase = const_cast<UItemDataBase*>(ConstGlobalItemDataBase);

			// 데이터베이스에 아이템 추가
			GlobalItemDataBase->AddItem(ItemName, ItemImage, nullptr, 1);

			// UI 접근
			UInventoryUI* InventoryUI = Cast<UInventoryUI>(UGameplayStatics::GetActorOfClass(GetWorld(), UInventoryUI::StaticClass()));
			if (InventoryUI)
			{
				// 데이터베이스 아이템을 UI 아이템으로 변환
				TArray<FInventorySlotData> UIItems;
				for (const FItem& Item : GlobalItemDataBase->Items)
				{
					FInventorySlotData UIItem;
					UIItem.ItemName = Item.Name;
					UIItem.ItemImage = Item.Image;
					UIItem.ItemQuantity = Item.Num;
					UIItems.Add(UIItem);
				}

				// UI 업데이트
				InventoryUI->UpdateInventory(UIItems);
			}

			// 아이템을 맵에서 제거
			Destroy();

			// 디버그 로그
			UE_LOG(LogTemp, Warning, TEXT("Item added to global database and UI updated: %s"), *ItemName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to access global item database."));
		}
	}
}
