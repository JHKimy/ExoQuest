#include "Item/ItemDataBase.h"





UTexture2D* UItemDataBase::GetImageByName(const FString& ItemName) const
{
    // 이름을 기반으로 이미지 찾기
    if (ItemImageMap.Contains(ItemName)) {
        return *ItemImageMap.Find(ItemName);
    }
    return nullptr;
}


FItem UItemDataBase::FindItem(const FString& ItemName) const
{
    for (const FItem& Item : Items)
    {
        if (Item.Name == ItemName)
        {
            return Item; // 값 반환
        }
    }

    // 아이템이 없을 경우 기본값 반환
    return FItem();
}



void UItemDataBase::AddItem(const FString& ItemName, int32 ItemNum)
{
    // Null 체크
    {
        if (!this)
        {
            UE_LOG(LogTemp, Error, TEXT("UItemDataBase is null!"));
            return;
        }
        if (Items.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Items array is empty. Initializing."));
        }
    }


    for (FItem& Item : Items)
    {
        if (Item.Name == ItemName)
        {
            Item.Num += ItemNum;
            return;
        }
    }

    FItem NewItem;
    NewItem.Name = ItemName;
    NewItem.Image = GetImageByName(ItemName);   // 이름 기반으로 이미지 자동 설정
    NewItem.Num = ItemNum;

    Items.Add(NewItem);
}









bool UItemDataBase::UseItem(const FString& ItemName)
{
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].Name == ItemName)
        {
            if (Items[i].Num > 0)
            {
                Items[i].Num--;

                // 수량이 0이 되면 아이템 제거
                if (Items[i].Num == 0)
                {
                    Items.RemoveAt(i);
                }
                return true;
            }
            break;
        }
    }

    return false; // 아이템이 없거나 사용 불가
}


