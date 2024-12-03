// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Components/TextBlock.h" //  텍스트 표시를 위한 헤더
#include "Components/Image.h"     //  이미지 표시를 위한 헤더
#include "Item/ItemDataBase.h"         // UItemDataBase 클래스 포함
#include "UI/InventorySlot.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 인벤토리 업데이트
    UpdateInventory();
}



void UInventoryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

	Super::NativeTick(MyGeometry, InDeltaTime);

    // UpdateInventory();

    //if (!InventoryBox)
    //{
    //    UE_LOG(LogTemp, Error, TEXT("InventoryBox is not set or not bound!"));
    //    return;
    //}
    //else {
    //    UE_LOG(LogTemp, Error, TEXT("InventoryBox set"));

    //}


 //   // 인벤토리 안에 아이템이 들어 있으면 수량 늘리기
 //   for (FItem& Item : ItemDataBase->Items)
 //   {
 //       if (Item. == "Starflux")
 //       {
 //           InventoryItemQuantity += InventoryItem.Quantity;
 //           return;
 //       }
 //   }

    //UpdateInventory();
}



void UInventoryUI::UpdateInventory()
{
    // Wrap Box 초기화 (기존 슬롯 제거)
    InventoryBox->ClearChildren();

    ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // ItemDataBase의 Items 배열을 순회하며 슬롯 추가
    for (const FItem& Item : Character->ItemDataBase->Items)
    {
        // Wrap Box에 아이템 추가
        AddSlot(Item.Name, Item.Image, Item.Num);
    }
}

void UInventoryUI::AddSlot(FString ItemName, UTexture2D* ItemImage, int32 ItemQuantity)
{
    // 슬롯 위젯 생성
    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

    if (NewSlot)
    {
        // 슬롯 초기화 (예: 아이템 이름, 이미지, 수량)
        NewSlot->UpdateSlotData(ItemName, ItemImage, ItemQuantity);

        // 슬롯을 Wrap Box에 추가
        InventoryBox->AddChild(NewSlot);
        UE_LOG(LogTemp, Log, TEXT("Slot added to InventoryBox"));
    }
}
