// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

void UInventorySlot::UpdateSlotData(const FString& NewItemName, UTexture2D* NewItemImage, int32 NewItemQuantity)
{
    ItemSlot.ItemName = NewItemName;
    ItemSlot.ItemImage = NewItemImage;
    ItemSlot.ItemQuantity = NewItemQuantity;

    // UI�� �ð����� ��Ҹ� ������Ʈ�ϰ� �ʹٸ� �߰����� ������ ���⿡ �ۼ��մϴ�.
    // ���� ���, �ؽ�Ʈ�� �̹����� ���� UI�� �ݿ��ϴ� ����.
}