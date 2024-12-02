// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Components/TextBlock.h" //  텍스트 표시를 위한 헤더
#include "Components/Image.h"     //  이미지 표시를 위한 헤더
#include "Item/ItemDataBase.h"         // UItemDataBase 클래스 포함
#include "Components/GridSlot.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

}



void UInventoryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

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



void UInventoryUI::UpdateInventory(const TArray<FInventorySlotData>& InventoryData)
{
    //if (!GridPanel || !InventorySlotClass) return;

    //// GridPanel의 기존 슬롯 제거
    //GridPanel->ClearChildren();

    //// 슬롯 개수 계산
    //int32 TotalSlots = Rows * Columns;

    //// InventoryData에 따라 슬롯 추가
    //for (int32 i = 0; i < TotalSlots; i++)
    //{
    //    // 슬롯 위젯 생성
    //    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
    //    if (NewSlot)
    //    {
    //        // 슬롯 데이터를 설정
    //        if (i < InventoryData.Num())
    //        {
    //            NewSlot->ItemSlot = InventoryData[i];
    //        }

    //        // GridPanel에 추가
    //        GridPanel->AddChild(NewSlot);

    //        // 슬롯의 행(Row)과 열(Column) 설정
    //        UGridSlot* GridSlot = Cast<UGridSlot>(NewSlot->Slot);
    //        if (GridSlot)
    //        {
    //            GridSlot->SetRow(i / Columns);
    //            GridSlot->SetColumn(i % Columns);
    //        }
    //    }
    //}
}