#include "Item/ItemDataBase.h"

void UItemDataBase::AddItem(
    const FString& ItemName,
    UTexture2D* ItemImage,
    AActor* ItemClass,
    int32 ItemNum)
{

    // Null üũ
    if (!this)
    {
        UE_LOG(LogTemp, Error, TEXT("UItemDataBase is null!"));
        return;
    }

    if (Items.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Items array is empty. Initializing."));
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
    NewItem.Image = ItemImage;
    NewItem.Class = ItemClass;
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

                // ������ 0�� �Ǹ� ������ ����
                if (Items[i].Num == 0)
                {
                    Items.RemoveAt(i);
                }
                return true;
            }
            break;
        }
    }

    return false; // �������� ���ų� ��� �Ұ�
}
