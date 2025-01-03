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
	// DataBase ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerPlant")
	TObjectPtr<UItemDataBase> ItemDataBase;


	// ��ư�� �̹����� ���ε��� ����
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

	// StarFlux ǥ�ÿ� �ؽ�Ʈ ���
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StarFluxText;




	// ������ ���� ����
	TMap<UButton*, FString> ButtonToItemMap;

	// ������ ��ư ���� �Լ�
	void PopulateItemData();

	// ��ư Ŭ�� �̺�Ʈ ó�� �Լ�
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
