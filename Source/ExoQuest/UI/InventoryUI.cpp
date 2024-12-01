// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "UI/InventoryComponent.h"

void UInventoryUI::UpdateInventory()
{
    if (!InventoryComponent) return;

    // UI를 업데이트하는 로직 (예: GridPanel에 버튼 추가)
    for (const FInventoryItem& Item : InventoryComponent->Inventory)
    {
        // 아이템 이름과 개수 표시
        FString ItemInfo = FString::Printf(TEXT("%s x%d"), *Item.ItemData.ItemName, Item.Quantity);

        // 아이콘과 이름을 기반으로 UI에 표시
        // 버튼 클릭 시 아이템 사용
    }
}
