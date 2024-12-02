//#include "UI/InventoryComponent.h"
//#include "Item/ItemDataBase.h" // FItem 구조체 포함
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
//        // ItemDataBase의 아이템 데이터를 로드
//        for (const FItem& Item : ItemDataBase->Items)
//        {
//            FInventoryItem NewInventoryItem;
//            NewInventoryItem.ItemData = Item;
//            NewInventoryItem.Quantity = 0; // 초기 수량은 0으로 설정
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
//    // 인벤토리 안에 아이템이 들어 있으면 수량 늘리기
//    for (const Item& InventoryItem : ItemDataBase->Items)
//    {
//        if (InventoryItem.ItemData.ItemName == ItemData.ItemName)
//        {
//            InventoryItem.Quantity += Quantity;
//            return;
//        }
//    }
//
//    // 새로운 아이템으면 추가
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
//            // 아이템 사용 로직
//            if (Inventory[i].ItemData.ItemClass)
//            {
//                GetWorld()->SpawnActor<AActor>(Inventory[i].ItemData.ItemClass);
//            }
//            return;
//        }
//    }
//}