#include "Enemy/Weapon/EnemyRifle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyRifle::AEnemyRifle()
{
	PrimaryActorTick.bCanEverTick = true;

    rifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
    SetRootComponent(rifleMesh);

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Weapon/SK_Rifle.SK_Rifle'")
    );
    if (TempMesh.Succeeded())
    {
        rifleMesh->SetSkeletalMesh(TempMesh.Object);
    }

    rifleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // rifleMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
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

FTransform AEnemyRifle::GetLeftHandSocketTransform() const
{
    if (rifleMesh && rifleMesh->DoesSocketExist(FName("LeftHandIK")))
    {

        return rifleMesh->GetSocketTransform(FName("LeftHandIK"), RTS_World);

    }

    return FTransform::Identity; // 소켓이 없을 경우 기본값 반환
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

