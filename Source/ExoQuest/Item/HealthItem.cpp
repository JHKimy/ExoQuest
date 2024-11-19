#include "Item/HealthItem.h"
#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"

AHealthItem::AHealthItem()
{
	PrimaryActorTick.bCanEverTick = true;

	capsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	

	//capsuleCollision->SetRelativeLocationAndRotation(
	//	FVector(6.7f, 0.f, 40.f), FRotator(0.f, -2.5f, 0.f));
	//capsuleCollision->SetRelativeScale3D(FVector(0.65f, 0.25f, 1.9f));

	capsuleCollision->SetupAttachment(RootComponent);

	capsuleCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));  // 충돌 프로파일 설정
	capsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealthItem::OnBeginOverlap);

	RootComponent = capsuleCollision;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/HealthItem/HealthItem.HealthItem'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	meshComp->SetupAttachment(RootComponent);


}

void AHealthItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	// ACharacterBase로 캐스팅
	ACharacterBase* tempCharacter = Cast<ACharacterBase>(OtherActor);

	// 캐스팅 성공 여부 확인
	if (tempCharacter)
	{
		tempCharacter->health += 10;  // 캐스팅이 성공했을 때만 health 접근
		
	}
	Destroy();
	
}


