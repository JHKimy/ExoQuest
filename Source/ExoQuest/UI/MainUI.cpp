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

	// Owning Player�� Pawn�� ���� ĳ���� ���� ���
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		// healthRatio�� �ʱⰪ �Ҵ�
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);

		// healthRatio�� �ʱⰪ �Ҵ�
		staminaRatio = Character->stamina / Character->maxStamina;
		staminaString = FString::FromInt(Character->stamina);
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// �� �����Ӹ��� healthRatio ������Ʈ
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

	// TargetImage�� nullptr�� �ƴ� ��츸 ó��
	if (TargetImage)
	{
		UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(TargetImage->Slot);
		if (PanelSlot)
		{
			PanelSlot->SetZOrder(1);
		}
	}
}
