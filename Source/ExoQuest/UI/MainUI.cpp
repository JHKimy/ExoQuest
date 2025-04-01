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
	Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
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
	// ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);

		staminaRatio = Character->stamina / Character->maxStamina;
		staminaString = FString::FromInt(Character->stamina);
	}

	// 모든 무기 숨기고 시작
	HideAllWeaponIcons();


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

	//// TargetImage가 nullptr이 아닌 경우만 처리
	//if (TargetImage)
	//{
	//	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(TargetImage->Slot);
	//	if (PanelSlot)
	//	{
	//		PanelSlot->SetZOrder(1);
	//	}
	//}

	// 2. 현재 선택된 무기 아이콘만 보이기
	switch (Character->PrimaryWeapon)
	{
	case EWeaponType::Rifle:
		if (RifleImage) RifleImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(RifleImage, 1);
		break;
	case EWeaponType::Shotgun:
		if (ShotgunImage) ShotgunImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(ShotgunImage, 1);
		break;
	case EWeaponType::RocketLauncher:
		if (RocketLauncherImage) RocketLauncherImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(RocketLauncherImage, 1);
		break;
	case EWeaponType::Sword:
		if (SwordImage) SwordImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(SwordImage, 1);
		break;
	default:
		break;
	}

	EWeaponType SubWeapon = (Character->EquippedWeapons.Num() > 1) ? Character->EquippedWeapons[1] : EWeaponType::None;

	switch (SubWeapon)
	{
	case EWeaponType::Rifle:
		if (RifleImage) RifleImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(RifleImage, 0);

		break;
	case EWeaponType::Shotgun:
		if (ShotgunImage) ShotgunImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(ShotgunImage, 0);
		break;
	case EWeaponType::RocketLauncher:
		if (RocketLauncherImage) RocketLauncherImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(RocketLauncherImage, 0);
		break;
	case EWeaponType::Sword:
		if (SwordImage) SwordImage->SetVisibility(ESlateVisibility::Visible);
		SetZorder(SwordImage, 0);
		break;
	default:
		break;
	}
	GrenadeCountText = FString::FromInt(Character->CurrentGrenadeCount);

	GhostSkillText = FString::FromInt(Character->ItemDataBase->FindItem("GhostGear").Num);
	TimeBackSkillText = FString::FromInt(Character->ItemDataBase->FindItem("TimeFluxGear").Num);

}

void UMainUI::HideAllWeaponIcons()
{
	if (RifleImage) RifleImage->SetVisibility(ESlateVisibility::Hidden);
	if (ShotgunImage) ShotgunImage->SetVisibility(ESlateVisibility::Hidden);
	if (RocketLauncherImage) RocketLauncherImage->SetVisibility(ESlateVisibility::Hidden);
	if (SwordImage) SwordImage->SetVisibility(ESlateVisibility::Hidden);
}

void UMainUI::SetZorder(UImage* image, int32 z)
{
	if (!image) return;

	// image가 속한 Slot이 CanvasPanelSlot인지 확인
	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(image->Slot);
	if (canvasSlot)
	{
		canvasSlot->SetZOrder(z);
	}
}