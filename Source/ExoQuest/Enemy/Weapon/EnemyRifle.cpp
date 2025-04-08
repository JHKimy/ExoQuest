#include "Enemy/Weapon/EnemyRifle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Enemy/EnemyBase.h"
#include "Character/CharacterBase.h"

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
	MuzzleLocation->SetRelativeLocation(FVector(0.f, 77.f, 10.f));

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
	if (!OwnerEnemy) return;

	FVector MuzzlePos = MuzzleLocation->GetComponentLocation();
	FVector TargetLocation = OwnerEnemy->target->GetActorLocation();

	FVector Direction = (TargetLocation - MuzzlePos).GetSafeNormal();
	FVector EndPoint = MuzzlePos + Direction * TraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerEnemy); // 총 주인 무시

	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		MuzzlePos,
		EndPoint,
		ECC_GameTraceChannel1, // 커스텀 트레이스 채널 설정해도 OK
		Params
	);

	// 디버그 선
	if (bDrawDebugLine)
	{
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), MuzzlePos, EndPoint, LineColor, false, 1.0f, 0, 2.0f);
	}

	// 피격 처리
	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyDamage(HitActor, Damage, OwnerEnemy->GetController(), this, nullptr);
			UKismetSystemLibrary::PrintString(this, TEXT("Hit Target!"), true, false, FLinearColor::Red, 1.5f);
		}
	}
}


