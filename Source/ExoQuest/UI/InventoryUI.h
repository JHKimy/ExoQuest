// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    class UInventoryComponent* InventoryComponent;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventory();
};
