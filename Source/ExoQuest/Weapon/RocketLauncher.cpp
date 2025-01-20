#include "Weapon/RocketLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
//#include "RocketProjectile.h" // ARocketProjectile 클래스 포함
#include "Weapon/RocketProjectile.h"

#include "DrawDebugHelpers.h"


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

	// 발사 전 예측 경로 시각화
	if (bCanFire)
	{
		ShowProjectilePrediction();
	}

}

void ARocketLauncher::Fire()
{
    if (!bCanFire) return;
    
    bCanFire = false; // 발사 후 쿨타임 동안 발사 불가

    //UKismetSystemLibrary::PrintString(this, TEXT("Firing Rocket!"), true, true, FColor::Red, 2.0f);

    if (RocketProjectileClass)
    {
        FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // 총구 위치
        FRotator MuzzleRotation = muzzleLocation->GetComponentRotation(); // 총구 회전

        // 로켓 발사
        ARocketProjectile* Rocket = GetWorld()->SpawnActor<ARocketProjectile>(RocketProjectileClass, MuzzleLocation, MuzzleRotation);

        if (Rocket)
        {
           // UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawned Successfully!"), true, true, FColor::Green, 2.0f);
        }
        else
        {
            //UKismetSystemLibrary::PrintString(this, TEXT("Rocket Spawn Failed!"), true, true, FColor::Red, 2.0f);
        }
    }
    else
    {
        //UKismetSystemLibrary::PrintString(this, TEXT("Projectile Class Not Set"), true, true, FColor::Red, 2.0f);
    }

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, this, &ARocketLauncher::ResetFire, FireRateDelay, false);
}


void ARocketLauncher::ResetFire()
{
    bCanFire = true; // 쿨타임 해제, 발사 가능 상태로 전환
}



void ARocketLauncher::ShowProjectilePrediction()
{
    if (RocketProjectileClass && muzzleLocation)
    {
        FVector StartLocation = muzzleLocation->GetComponentLocation();
        FRotator LaunchRotation;

        // 카메라 방향을 반영한 발사 방향 계산
        if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
        {
            FVector CameraLocation;
            FRotator CameraRotation;
            PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
            LaunchRotation = CameraRotation;
        }
        else
        {
            // 기본적으로 총구 방향으로 설정
            LaunchRotation = muzzleLocation->GetComponentRotation();
        }

        // 카메라 방향 또는 총구 방향을 기반으로 한 초기 속도 설정
        FVector LaunchVelocity = LaunchRotation.Vector() * 4000.0f; // 로켓 발사 속도와 일치하는 초기 속도 설정

        // FPredictProjectilePathParams 설정
        FPredictProjectilePathParams PredictParams;
        PredictParams.StartLocation = StartLocation;
        PredictParams.LaunchVelocity = LaunchVelocity;
        PredictParams.bTraceWithCollision = true;
        PredictParams.ProjectileRadius = 15.0f; // 발사체 크기와 일치하도록 설정
        PredictParams.MaxSimTime = 5.0f; // 시뮬레이션 시간 (초)

        PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ()*2;

        
        PredictParams.SimFrequency = 15.0f;
        PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
        PredictParams.DrawDebugTime = 0.1f;

        FPredictProjectilePathResult PredictResult;
        UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
    }
}
