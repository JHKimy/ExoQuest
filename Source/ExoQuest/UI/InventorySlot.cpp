#include "UI/InventorySlot.h"
#include "Components/Button.h"
#include "UI/InventoryUI.h"

void UInventorySlot::NativeConstruct()
{
    Super::NativeConstruct();

    // 월드에서 캐릭터를 검색
    character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (SlotButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Button Bound"));
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlot::OnSlotClicked);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("SlotButton is null!"));
    }
}

void UInventorySlot::UpdateSlotData(const FString& NewItemName, int32 NewItemNum)
{
    ItemName = NewItemName;
    ItemNum = NewItemNum;

    if (ItemDataBase)
    {
        ItemImage = ItemDataBase->GetImageByName(ItemName);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ItemDataBase is null!"));
    }
}

void UInventorySlot::OnSlotClicked()
{
    if (ItemName == "Medical Kit")
    {
        if (ItemDataBase && ItemDataBase->UseItem(ItemName))
        {
            if (character)
            {
                // 체력 회복
                character->health += 10;
                if (character->health > character->maxHealth)
                {
                    character->health = character->maxHealth;
                }

                // 디버그 메시지
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Health increased by 10!"));

                // 인벤토리 업데이트
                if (character->InventoryUI)
                {
                    character->InventoryUI->UpdateInventory();
                }
                else
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("InventoryUI is null!"));
                }
            }
            else
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CharacterInstance is null!"));
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to use item from ItemDataBase!"));
        }
    }


    if (ItemName == "GhostGear")
    {
        if (ItemDataBase && ItemDataBase->UseItem(ItemName))
        {
            if (character)
            {
                character->ActivateGhostMode(5.f);

                // 인벤토리 업데이트
                if (character->InventoryUI)
                {
                    character->InventoryUI->UpdateInventory();
                }
            }
        }
    }

    if (ItemName == "TimeFluxGear")
    {
        if (ItemDataBase && ItemDataBase->UseItem(ItemName))
        {
            if (character)
            {
                // 시간 되돌리기 활성화
                character->ActivateTimeRewind(5.f);


                // 인벤토리 업데이트
                if (character->InventoryUI)
                {
                    character->InventoryUI->UpdateInventory();
                }
            }
        }
    }
}


