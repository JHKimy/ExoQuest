// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"


UCLASS()
class EXOQUEST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    FString ItemName;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    UTexture2D* ItemImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    int32 ItemQuantity;


    // ���� �����͸� ������Ʈ�ϴ� �Լ� �߰�
    UFUNCTION(BlueprintCallable, Category = "Slot")
    void UpdateSlotData(const FString& NewItemName, UTexture2D* NewItemImage, int32 NewItemQuantity)
    {
        ItemName = NewItemName;
        ItemImage = NewItemImage;
        ItemQuantity = NewItemQuantity;
    };
};
