#include "Map/ToBattlePortal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h" // 레벨 전환에 사용
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





	// 언리얼 객체 생성 순서
	// ( C++ ) 생성자 -> 
	// 블루 프린트 속성 -> 
	// ( C++ ) PostInitializeComponents-> 
	// ( C++ ) 비긴플레이 -> 
	// 블루프린트 이벤트 그래프

	// 생성자가 가장 먼저 동작하는데 뒤에 속성들이 덮어가면서
	// 가끔 초기화가 완전히 끝나지 않은 상태로 이벤트가 등록될 수 있다.

	// 언리얼의 공식적인 객체 초기화 플로우에서는 
	// BeginPlay 이후에 객체가 월드와 완전히 연결된 상태라고 보장
	// PostInitializeComponents 이용
	
	// 충돌 연동
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
		// 캐릭터 상태 저장
		myCharacter->SaveStateBeforeLevelChange();

		// 맵 전환 (Seamless Travel)
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BattleFields?listen"), true);
	}
}