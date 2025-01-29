#include "Item/TimeFluxGear.h"
#include "Character/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemDataBase.h"
#include "UI/InventoryUI.h"

ATimeFluxGear::ATimeFluxGear()
{
	PrimaryActorTick.bCanEverTick = true;

    // 메시 컴포넌트 초기화
    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/TimeFluxGear/TimeFluxGear.TimeFluxGear'"));

    if (tempMesh.Succeeded())
    {
        meshComp->SetStaticMesh(tempMesh.Object);
    }
    meshComp->SetCollisionProfileName(TEXT("NoCollision"));
    meshComp->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
    meshComp->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
    RootComponent = meshComp;

    // 캡슐 컴포넌트 초기화
    capsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
    capsuleCollision->SetCapsuleHalfHeight(55.f);
    capsuleCollision->SetCapsuleRadius(25.f);
    capsuleCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    capsuleCollision->SetupAttachment(RootComponent);
    capsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ATimeFluxGear::OnBeginOverlap);

    // 기본 값 초기화
    ItemName = TEXT("TimeFluxGear");
    ItemNum = 1;
}

void ATimeFluxGear::BeginPlay()
{
	Super::BeginPlay();
	
    InitialLocation = GetActorLocation();

    RiseHeight = InitialLocation.Z + 300.f;

    TargetLocation = InitialLocation + FVector(0, 0, RiseHeight);

    SetActorLocation(InitialLocation - FVector(0, 0, RiseHeight));

    GetWorldTimerManager().SetTimer(
        DistanceCheckTimerHandle,
        this,
        &ATimeFluxGear::CheckForNearbyCharacters,
        CheckInterval,
        true
    );
}

void ATimeFluxGear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();

    if (!HasReachedTarget)
    {
        SetActorLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, RiseSpeed));
        if (FVector::Dist(CurrentLocation, TargetLocation) < KINDA_SMALL_NUMBER)
        {
            SetActorLocation(TargetLocation);
            HasReachedTarget = true;
        }
    }

    if (HasReachedTarget && IsCharacterNearby && character)
    {
        AbsorbToCharacter(DeltaTime);
    }

}

void ATimeFluxGear::CheckForNearbyCharacters()
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
            if (Distance < 1000.0f && Distance < ClosestDistance)
            {
                IsCharacterNearby = true;
                character = DetectedCharacter;
                ClosestDistance = Distance;
            }
        }
    }

}

void ATimeFluxGear::AbsorbToCharacter(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();

    FVector CharacterLocation = character->GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, CharacterLocation, DeltaTime, MoveToCharacterSpeed);
    SetActorLocation(NewLocation);

}

void ATimeFluxGear::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
    // ACharacterBase로 캐스팅
    ACharacterBase* tempCharacter = Cast<ACharacterBase>(OtherActor);

    // 캐스팅 성공 여부 확인
    if (tempCharacter)
    {
        // tempCharacter->health += 10;  // 캐스팅이 성공했을 때만 health 접근

        tempCharacter->ItemDataBase->AddItem(ItemName, ItemNum);

        tempCharacter->InventoryUI->UpdateInventory();

        Destroy();
    }

}

