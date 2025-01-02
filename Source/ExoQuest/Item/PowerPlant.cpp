#include "Item/PowerPlant.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/EQPlayerController.h"
#include <Blueprint/UserWidget.h>	// 위젯
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

	// 충돌 설정
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic); // 기본 WorldDynamic 사용
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널 무시
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn과 Overlap

}

// Called when the game starts or when spawned
void APowerPlant::BeginPlay()
{
	Super::BeginPlay();

	// ** 충돌 연동
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
			// UI를 뷰포트에 추가
			UI->AddToViewport();
			
			PlayerController->bShowMouseCursor = true;
			//PlayerCharacter->bIsInventoryOpen = false;

			UE_LOG(LogTemp, Warning, TEXT("UI added to viewport."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UI is already in viewport or UI is null."));
		}
	}
}




// ?????????? => 블루프린트
void APowerPlant::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacterBase::StaticClass()))
	{
		if (PlayerCharacter && OtherActor == PlayerCharacter && UI && UI->IsInViewport())
		{
			UI->RemoveFromParent();
			PlayerController->bShowMouseCursor = false; // 마우스 커서 비활성화
			UE_LOG(LogTemp, Warning, TEXT("UI removed from viewport."));
		}
	}
}
