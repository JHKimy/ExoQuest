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

	capsuleCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));  // �浹 �������� ����
	capsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnBeginOverlap);
	capsuleCollision->SetupAttachment(RootComponent);





	// �⺻ �� ����
	ItemName = TEXT("Health Kit");
	//ItemImage = nullptr; // �������Ʈ���� ���� ����
	ItemNum = 1;
}

void AHealthItem::BeginPlay()
{
	Super::BeginPlay();
	
	// �ʱ� ��ġ�� Ÿ�� ��ġ ����
	InitialLocation = GetActorLocation();
	TargetLocation = InitialLocation + FVector(0, 0, RiseHeight);

	// ���� ��ġ�� �ٴ����� ����
	SetActorLocation(InitialLocation - FVector(0, 0, RiseHeight));

}

void AHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ġ�� ���������� ����
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, RiseSpeed));

	// ��ǥ ��ġ�� ���������� Tick �ߴ�
	if (FVector::Dist(CurrentLocation, TargetLocation) < KINDA_SMALL_NUMBER)
	{
		SetActorLocation(TargetLocation);
		SetActorTickEnabled(false); // �� �̻� Tick�� �ʿ� ����
	}
}

void AHealthItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// ACharacterBase�� ĳ����
	ACharacterBase* tempCharacter = Cast<ACharacterBase>(OtherActor);

	// ĳ���� ���� ���� Ȯ��
	if (tempCharacter)
	{
		tempCharacter->health += 10;  // ĳ������ �������� ���� health ����
		
		tempCharacter->ItemDataBase->AddItem(ItemName, ItemNum);

		tempCharacter->InventoryUI->UpdateInventory();

		Destroy();
	}

	
}