#include "Weapon/WeaponForEquip.h"
#include "Components/BoxComponent.h"
#include "Character/CharacterBase.h"			// 충돌처리
#include "Kismet/KismetSystemLibrary.h"

AWeaponForEquip::AWeaponForEquip()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->InitBoxExtent(FVector(20.f, 35.f, 18.f));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 충돌 연동
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponForEquip::OnBeginOverlap);


	Amplitude = 15.f;
	RotationSpeed = 1.f;
}

void AWeaponForEquip::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent && MeshComponent->GetStaticMesh())
	{
		FString MeshName = MeshComponent->GetStaticMesh()->GetName();

		// 스태틱 메쉬의 이름을 기반으로 무기 타입을 설정
		if (MeshName.Contains("Rifle"))
		{
			WeaponType = EWeaponTypeForEquip::Rifle;
		}
		else if (MeshName.Contains("Shotgun"))
		{
			WeaponType = EWeaponTypeForEquip::Shotgun;
		}
		else if (MeshName.Contains("RocketLauncher"))
		{
			WeaponType = EWeaponTypeForEquip::RocketLauncher;
		}
		else if (MeshName.Contains("Sword"))
		{
			WeaponType = EWeaponTypeForEquip::Sword;
		}
	}

	StartPos = GetActorLocation();


}

void AWeaponForEquip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 움직임
	Floating();

}

void AWeaponForEquip::Floating()
{
	FVector NewLocation = StartPos;
	float Time = GetWorld()->GetTimeSeconds();
	NewLocation.Z += FMath::Sin(Time) * Amplitude;

	SetActorLocation(NewLocation);
	AddActorLocalRotation(FRotator(0, RotationSpeed, 0));
}

void AWeaponForEquip::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	ACharacterBase* EquipCharacter = Cast<ACharacterBase>(OtherActor);
	
	if (EquipCharacter) 
	{
		Destroy();
	}

	

	switch (WeaponType)
	{
	case EWeaponTypeForEquip::Rifle:
		EquipCharacter->EquippedWeapons.Add(EWeaponType::Rifle);
		break;

	case EWeaponTypeForEquip::Shotgun:
		EquipCharacter->EquippedWeapons.Add(EWeaponType::Shotgun);
		break;

	case EWeaponTypeForEquip::RocketLauncher:
		EquipCharacter->EquippedWeapons.Add(EWeaponType::RocketLauncher);
		break;

	case EWeaponTypeForEquip::Sword:
		EquipCharacter->EquippedWeapons.Add(EWeaponType::Sword);
		break;
	
	default:
		break;
	}

}

