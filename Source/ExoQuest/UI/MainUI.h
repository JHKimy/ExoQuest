#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MainUI.generated.h"

UCLASS()
class EXOQUEST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // NativeTick 선언

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = health)
	FString healthString;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = health)
	float healthRatio;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = stamina)
	FString staminaString;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = stamina)
	float staminaRatio;

	// UI 요소들 (이미지)
	UPROPERTY(meta = (BindWidget))
	UImage* ShotgunImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RocketLauncherImage;

	UPROPERTY(meta = (BindWidget))
	UImage* SwordImage;

	UPROPERTY(meta = (BindWidget))
	UImage* RifleImage;
};
