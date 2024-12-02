// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

void UInventorySlot::UpdateSlotData(const FString& NewItemName, UTexture2D* NewItemImage, int32 NewItemQuantity)
{
    ItemSlot.ItemName = NewItemName;
    ItemSlot.ItemImage = NewItemImage;
    ItemSlot.ItemQuantity = NewItemQuantity;

    // UI의 시각적인 요소를 업데이트하고 싶다면 추가적인 로직을 여기에 작성합니다.
    // 예를 들어, 텍스트나 이미지를 실제 UI에 반영하는 로직.
}