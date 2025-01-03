// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDataBase.h"
#include "PowerPlantUI.generated.h"


UCLASS()
class EXOQUEST_API UPowerPlantUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// DataBase 참조
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerPlant")
	TObjectPtr<UItemDataBase> ItemDataBase;


	// 버튼과 이미지를 바인딩할 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MedicalKitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MedicalKitImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> GhostGearButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> GhostGearImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> TimeFluxGearButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TimeFluxGearImage;

	// StarFlux 표시용 텍스트 블록
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StarFluxText;




	// 아이템 매핑 정보
	TMap<UButton*, FString> ButtonToItemMap;

	// 아이템 버튼 생성 함수
	void PopulateItemData();

	// 버튼 클릭 이벤트 처리 함수
	UFUNCTION()
	void OnMedicalKitButtonClicked();

	UFUNCTION()
	void OnGhostGearButtonClicked();

	UFUNCTION()
	void OnTimeFluxGearButtonClicked();

	UFUNCTION()
	void OnButtonClicked(UButton* ClickedButton);

	UFUNCTION()
	void UpdateStarFluxDisplay();
};
