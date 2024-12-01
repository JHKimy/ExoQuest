// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "UI/InventoryComponent.h"

void UInventoryUI::UpdateInventory()
{
    if (!InventoryComponent) return;

    // UI�� ������Ʈ�ϴ� ���� (��: GridPanel�� ��ư �߰�)
    for (const FInventoryItem& Item : InventoryComponent->Inventory)
    {
        // ������ �̸��� ���� ǥ��
        FString ItemInfo = FString::Printf(TEXT("%s x%d"), *Item.ItemData.ItemName, Item.Quantity);

        // �����ܰ� �̸��� ������� UI�� ǥ��
        // ��ư Ŭ�� �� ������ ���
    }
}
