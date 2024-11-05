#include "Weapon/RocketLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
//#include "RocketProjectile.h" // ARocketProjectile Ŭ���� ����
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

	// MuzzleLocation Scene Component ���� �� �ʱ�ȭ
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// �⺻ ��ġ ���� (�ѱ� �պκ����� ��ġ ����)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 2.f)); // X, Y, Z ��ǥ ���� ����

    // �������Ʈ �������̵�
    //// RocketProjectileClass �ʱ�ȭ
    //static ConstructorHelpers::FClassFinder<ARocketProjectile> RocketProjectileBP(TEXT("/Game/BluePrint/Weapon/BP_RocketProjectile.BP_RocketProjectile_C"));
    //if (RocketProjectileBP.Succeeded())
    //{
    //    RocketProjectileClass = RocketProjectileBP.Class;
    //}
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
    UKismetSystemLibrary::PrintString(this, TEXT("Firing Rocket!"), true, true, FColor::Red, 2.0f);

    if (RocketProjectileClass)
    {
        FVector MuzzleLocation = muzzleLocation->GetComponentLocation(); // �ѱ� ��ġ
        FRotator MuzzleRotation = muzzleLocation->GetComponentRotation(); // �ѱ� ȸ��

        // ���� �߻�
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
}


void ARocketLauncher::ResetFire()
{
}

