#include "Map/ToBattlePortal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h" // ���� ��ȯ�� ���
#include "Kismet/KismetSystemLibrary.h"	


AToBattlePortal::AToBattlePortal()
{
	PrimaryActorTick.bCanEverTick = true;


	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxCollision->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
	RootComponent = boxCollision;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	//meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetupAttachment(RootComponent);



}

void AToBattlePortal::BeginPlay()
{
	Super::BeginPlay();





	// �𸮾� ��ü ���� ����
	// ( C++ ) ������ -> 
	// ��� ����Ʈ �Ӽ� -> 
	// ( C++ ) PostInitializeComponents-> 
	// ( C++ ) ����÷��� -> 
	// �������Ʈ �̺�Ʈ �׷���

	// �����ڰ� ���� ���� �����ϴµ� �ڿ� �Ӽ����� ����鼭
	// ���� �ʱ�ȭ�� ������ ������ ���� ���·� �̺�Ʈ�� ��ϵ� �� �ִ�.

	// �𸮾��� �������� ��ü �ʱ�ȭ �÷ο쿡���� 
	// BeginPlay ���Ŀ� ��ü�� ����� ������ ����� ���¶�� ����
	// PostInitializeComponents �̿�
	
	// �浹 ����
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AToBattlePortal::OnBeginOverlap);

}

void AToBattlePortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AToBattlePortal::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	ACharacterBase* myCharacter = Cast<ACharacterBase>(OtherActor);

	if (myCharacter)
	{
		// ĳ���� ���� ����
		myCharacter->SaveStateBeforeLevelChange();

		// �� ��ȯ (Seamless Travel)
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BattleFields?listen"), true);
	}
}