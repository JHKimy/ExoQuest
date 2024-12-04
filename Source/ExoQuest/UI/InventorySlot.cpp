// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

void UInventorySlot::UpdateSlotData(const FString& NewItemName, int32 NewItemNum)
{
    
    // 아이템 이름과 수량 설정5
    ItemName = NewItemName;
    ItemNum = NewItemNum;

    // 데이터베이스에서 이미지를 가져옴
    // 블루 프린트에서 이미지 설정을 하기 때문에
    // 설정 된 아이템 데이터 베이스의 블루 프린트 클래스를 가져와서 사용해야 함
    if (ItemDataBase)
    {
        ItemImage = ItemDataBase->GetImageByName(ItemName);
        if (ItemImage)
        {
            UE_LOG(LogTemp, Warning, TEXT("Image successfully loaded for %s"), *ItemName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Image not found for %s"), *ItemName);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataBase is null."));
    }    
}
