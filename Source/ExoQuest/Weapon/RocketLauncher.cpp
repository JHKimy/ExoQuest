#include "Weapon/RocketLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
//#include "RocketProjectile.h" // ARocketProjectile 클래스 포함
#include "Weapon/RocketProjectile.h"

ARocketLauncher::ARocketLauncher()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RocketLauncher/SM_RocketLauncher.SM_RocketLauncher'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	// MuzzleLocation Scene Component 생성 및 초기화
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// 기본 위치 설정 (총구 앞부분으로 위치 설정)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 60.f, 2.f)); // X, Y, Z 좌표 조정 가능

    // 블루프린트 오버라이드
    // RocketProjectileClass 초기화
    static ConstructorHelpers::FClassFinder<ARocketProjectile> RocketProjectileBP(TEXT("/Game/BluePrint/Weapon/BP_RocketProjectile.BP_RocketProjectile_C"));
    if (RocketProjectileBP.Succeeded())
    {
        RocketProjectileClass = RocketProjectileBP.Class;
    }

    bCanFire = true; // 시작 시 발사 가능 상태로 설정
}

void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();

	ownerCharacter = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	
}

void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketLauncher::Fire()
{
    if (!bCanFire) return;
    
    bCanFire = false; // 발사 후 쿨타임 동안 발사 불가

    UKismetSystemLibrary::PrintString(this, TEXT("Firing Rocket!"), true, true, FColor::Red, 2.0f);

    if (RocketProjectileClass)
    {
        FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // 총구 위치
        FRotator MuzzleRotation = muzzleLocation->GetComponentRotation(); // 총구 회전

        // 로켓 발사
        ARocketProjectile* Rocket = GetWorld()->SpawnActor<ARocketProjectile>(RocketProjectileClass, MuzzleLocation, MuzzleRotation);

        if (Rocket)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawned Successfully!"), true, true, FColor::Green, 2.0f);
        }
        else
        {
            UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawn Failed!"), true, true, FColor::Red, 2.0f);
        }
    }
    else
    {
        UKismetSystemLibrary::PrintString(this, TEXT("Projectile Class Not Set"), true, true, FColor::Red, 2.0f);
    }

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, this, &ARocketLauncher::ResetFire, FireRateDelay, false);
}


void ARocketLauncher::ResetFire()
{
    bCanFire = true; // 쿨타임 해제, 발사 가능 상태로 전환
}

