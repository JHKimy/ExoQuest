//#include "UI/InventoryComponent.h"
//#include "Item/ItemDataBase.h" // FItem ����ü ����
//
//UInventoryComponent::UInventoryComponent()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//
//
//}
//
//
//void UInventoryComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//    if (ItemDataBase)
//    {
//        // ItemDataBase�� ������ �����͸� �ε�
//        for (const FItem& Item : ItemDataBase->Items)
//        {
//            FInventoryItem NewInventoryItem;
//            NewInventoryItem.ItemData = Item;
//            NewInventoryItem.Quantity = 0; // �ʱ� ������ 0���� ����
//
//            Inventory.Add(NewInventoryItem);
//        }
//    }
//}
//
//
//void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}
//
//void UInventoryComponent::AddItem(const FItem& ItemData, int32 Quantity)
//{
//    // �κ��丮 �ȿ� �������� ��� ������ ���� �ø���
//    for (const Item& InventoryItem : ItemDataBase->Items)
//    {
//        if (InventoryItem.ItemData.ItemName == ItemData.ItemName)
//        {
//            InventoryItem.Quantity += Quantity;
//            return;
//        }
//    }
//
//    // ���ο� ���������� �߰�
//    FInventoryItem NewItem;
//    NewItem.ItemData = ItemData;
//    NewItem.Quantity = Quantity;
//
//    Inventory.Add(NewItem);
//}
//
//void UInventoryComponent::UseItem(const FString& ItemName)
//{
//    for (int32 i = 0; i < Inventory.Num(); i++)
//    {
//        if (Inventory[i].ItemData.ItemName == ItemName)
//        {
//            Inventory[i].Quantity--;
//
//            if (Inventory[i].Quantity <= 0)
//            {
//                Inventory.RemoveAt(i);
//            }
//
//            // ������ ��� ����
//            if (Inventory[i].ItemData.ItemClass)
//            {
//                GetWorld()->SpawnActor<AActor>(Inventory[i].ItemData.ItemClass);
//            }
//            return;
//        }
//    }
//}