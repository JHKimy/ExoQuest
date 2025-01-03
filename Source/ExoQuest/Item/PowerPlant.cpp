#include "Item/PowerPlant.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/EQPlayerController.h"
#include <Blueprint/UserWidget.h>	// ����
#include "Character/CharacterBase.h"

// Sets default values
APowerPlant::APowerPlant()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionSphere->InitSphereRadius(3000.f);
	CollisionSphere->SetupAttachment(RootComponent);

	// �浹 ����
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic); // �⺻ WorldDynamic ���
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // ��� ä�� ����
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn�� Overlap

}

// Called when the game starts or when spawned
void APowerPlant::BeginPlay()
{
	Super::BeginPlay();

	// ** �浹 ����
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APowerPlant::OnBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APowerPlant::OnEndOverlap);


	PlayerController = GetWorld()->GetFirstPlayerController();
	UI = CreateWidget<UUserWidget>(PlayerController, UIClass);
	
	PlayerCharacter = Cast<ACharacterBase>(PlayerController->GetPawn());
}

// Called every frame
void APowerPlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerPlant::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		if (UI && !UI->IsInViewport())
		{
			// UI�� ����Ʈ�� �߰�
			UI->AddToViewport();
			
			PlayerController->bShowMouseCursor = true;
			//PlayerCharacter->bIsInventoryOpen = false;

			// ĳ���� �Է� �� ȸ�� �� ���� �߻� ����
			if (PlayerCharacter)
			{
				//PlayerCharacter->SetInputRestrictions(true); // ����� ���� �Լ� ȣ��
				PlayerCharacter->bIsInventoryOpen = true; // ����� ���� �Լ� ȣ��

			}


			UE_LOG(LogTemp, Warning, TEXT("UI added to viewport."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UI is already in viewport or UI is null."));
		}
	}
}




// ?????????? => �������Ʈ
void APowerPlant::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		if (PlayerCharacter && OtherActor == PlayerCharacter && UI && UI->IsInViewport())
		{
			UI->RemoveFromParent();
			PlayerController->bShowMouseCursor = false; // ���콺 Ŀ�� ��Ȱ��ȭ
			if (PlayerCharacter)
			{
				PlayerCharacter->bIsInventoryOpen = false; // ����� ���� �Լ� ȣ��
			}

			UE_LOG(LogTemp, Warning, TEXT("UI removed from viewport."));
		}
	}
}

void APowerPlant::SpawnItemByIndex(int32 ItemIndex)
{
	if (ItemIndex < 0 || ItemIndex >= ItemClasses.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item index: %d"), ItemIndex);
		return;
	}

	// ������ ������ Ŭ����
	TSubclassOf<AActor> ItemClass = ItemClasses[ItemIndex];
	if (!ItemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item class at index %d is null"), ItemIndex);
		return;
	}

	// ���� ��ġ ���� (PowerPlant ���� ����)
	FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// ������ ����
	AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation);
	if (SpawnedItem)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully spawned item at index: %d"), ItemIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn item at index: %d"), ItemIndex);
	}
}
