#include "UI/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::AddItem(const FItem& ItemData, int32 Quantity)
{
    for (FInventoryItem& InventoryItem : Inventory)
    {
        if (InventoryItem.ItemData.ItemName == ItemData.ItemName)
        {
            InventoryItem.Quantity += Quantity;
            return;
        }
    }

    FInventoryItem NewItem;
    NewItem.ItemData = ItemData;
    NewItem.Quantity = Quantity;

    Inventory.Add(NewItem);
}

void UInventoryComponent::UseItem(const FString& ItemName)
{
    for (int32 i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i].ItemData.ItemName == ItemName)
        {
            Inventory[i].Quantity--;

            if (Inventory[i].Quantity <= 0)
            {
                Inventory.RemoveAt(i);
            }

            // 아이템 사용 로직
            if (Inventory[i].ItemData.ItemClass)
            {
                GetWorld()->SpawnActor<AActor>(Inventory[i].ItemData.ItemClass);
            }
            return;
        }
    }
}