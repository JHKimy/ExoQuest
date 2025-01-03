#include "Item/HealthItem.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"
#include "UI/InventoryUI.h"
#include "Components/CapsuleComponent.h"

AHealthItem::AHealthItem()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/HealthItem/HealthItem.HealthItem'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;



	capsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	

	//capsuleCollision->SetRelativeLocationAndRotation(
	//	FVector(6.7f, 0.f, 40.f), FRotator(0.f, -2.5f, 0.f));
	//capsuleCollision->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	capsuleCollision->SetCapsuleRadius(50.f);
	capsuleCollision->SetCapsuleHalfHeight(80.f);
	capsuleCollision->SetupAttachment(RootComponent);

	capsuleCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));  // 충돌 프로파일 설정
	capsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnBeginOverlap);
	capsuleCollision->SetupAttachment(RootComponent);





	// 기본 값 설정
	ItemName = TEXT("Health Kit");
	//ItemImage = nullptr; // 블루프린트에서 설정 가능
	ItemNum = 1;
}

void AHealthItem::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 위치와 타겟 위치 설정
	InitialLocation = GetActorLocation();
	TargetLocation = InitialLocation + FVector(0, 0, RiseHeight);

	// 시작 위치를 바닥으로 조정
	SetActorLocation(InitialLocation - FVector(0, 0, RiseHeight));

}

void AHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 위치를 점진적으로 보간
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, RiseSpeed));

	// 목표 위치에 도달했으면 Tick 중단
	if (FVector::Dist(CurrentLocation, TargetLocation) < KINDA_SMALL_NUMBER)
	{
		SetActorLocation(TargetLocation);
		SetActorTickEnabled(false); // 더 이상 Tick이 필요 없음
	}
}

void AHealthItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// ACharacterBase로 캐스팅
	ACharacterBase* tempCharacter = Cast<ACharacterBase>(OtherActor);

	// 캐스팅 성공 여부 확인
	if (tempCharacter)
	{
		tempCharacter->health += 10;  // 캐스팅이 성공했을 때만 health 접근
		
		tempCharacter->ItemDataBase->AddItem(ItemName, ItemNum);

		tempCharacter->InventoryUI->UpdateInventory();

		Destroy();
	}

	
}