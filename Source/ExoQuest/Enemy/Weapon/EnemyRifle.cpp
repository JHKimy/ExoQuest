#include "Enemy/Weapon/EnemyRifle.h"
#include "Kismet/GameplayStatics.h"
#include"Components/StaticMeshComponent.h"

AEnemyRifle::AEnemyRifle()
{
	PrimaryActorTick.bCanEverTick = true;

    rifleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Enemy/Weapon/SM_EnemyRifle.SM_EnemyRifle'"));
    if (tempMesh.Succeeded())
    {
        rifleMesh->SetStaticMesh(tempMesh.Object);
    }
    rifleMesh->SetCollisionProfileName(TEXT("NoCollision"));
    RootComponent = rifleMesh;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
	MuzzleLocation->SetRelativeLocation(FVector(50.f, 0.f, 10.f));





}

void AEnemyRifle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyRifle::Fire()
{
    FVector Start = MuzzleLocation->GetComponentLocation();
    FVector Forward = MuzzleLocation->GetForwardVector();
    FVector End = Start + Forward * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신은 무시

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_Visibility,
        Params
    );

    if (bHit && Hit.GetActor())
    {
        UGameplayStatics::ApplyDamage(Hit.GetActor(), Damage, nullptr, this, nullptr);
    }

    if (bDrawDebugLine)
    {
        DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.0f, 0, 2.0f);
    }
}

