#include "MainUI.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"

UMainUI::UMainUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Owning Player의 Pawn을 통해 캐릭터 참조 얻기
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		// healthRatio에 초기값 할당
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);

		// healthRatio에 초기값 할당
		staminaRatio = Character->stamina / Character->maxStamina;
		staminaString = FString::FromInt(Character->stamina);
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 매 프레임마다 healthRatio 업데이트
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);

		staminaRatio = Character->stamina / Character->maxStamina;
		staminaString = FString::FromInt(Character->stamina);
	}

	
	
	UImage* TargetImage = nullptr;
	
	switch (Character->PrimaryWeapon) 
	{
	case EWeaponType::Rifle:
		TargetImage = RifleImage;
		break;
	case EWeaponType::Shotgun:
		TargetImage = ShotgunImage;
		break;
	case EWeaponType::RocketLauncher:
		TargetImage = RocketLauncherImage;
		break;
	case EWeaponType::Sword:
		TargetImage = SwordImage;
		break;

	default:
		break;
	}

	// TargetImage가 nullptr이 아닌 경우만 처리
	if (TargetImage)
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(TargetImage->Slot);
		if (PanelSlot)
		{
			PanelSlot->SetZOrder(1);
		}
	}
}
