// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Components/TextBlock.h" //  �ؽ�Ʈ ǥ�ø� ���� ���
#include "Components/Image.h"     //  �̹��� ǥ�ø� ���� ���
#include "Item/ItemDataBase.h"         // UItemDataBase Ŭ���� ����
#include "Components/GridSlot.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

}



void UInventoryUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Super::NativeTick(MyGeometry, InDeltaTime);

 //   // �κ��丮 �ȿ� �������� ��� ������ ���� �ø���
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

    //// GridPanel�� ���� ���� ����
    //GridPanel->ClearChildren();

    //// ���� ���� ���
    //int32 TotalSlots = Rows * Columns;

    //// InventoryData�� ���� ���� �߰�
    //for (int32 i = 0; i < TotalSlots; i++)
    //{
    //    // ���� ���� ����
    //    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
    //    if (NewSlot)
    //    {
    //        // ���� �����͸� ����
    //        if (i < InventoryData.Num())
    //        {
    //            NewSlot->ItemSlot = InventoryData[i];
    //        }

    //        // GridPanel�� �߰�
    //        GridPanel->AddChild(NewSlot);

    //        // ������ ��(Row)�� ��(Column) ����
    //        UGridSlot* GridSlot = Cast<UGridSlot>(NewSlot->Slot);
    //        if (GridSlot)
    //        {
    //            GridSlot->SetRow(i / Columns);
    //            GridSlot->SetColumn(i % Columns);
    //        }
    //    }
    //}
}