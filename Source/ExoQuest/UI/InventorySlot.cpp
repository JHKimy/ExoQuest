// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventorySlot.h"

void UInventorySlot::UpdateSlotData(const FString& NewItemName, int32 NewItemNum)
{
    
    // ������ �̸��� ���� ����5
    ItemName = NewItemName;
    ItemNum = NewItemNum;

    // �����ͺ��̽����� �̹����� ������
    // ��� ����Ʈ���� �̹��� ������ �ϱ� ������
    // ���� �� ������ ������ ���̽��� ��� ����Ʈ Ŭ������ �����ͼ� ����ؾ� ��
    if (ItemDataBase)
    {
        ItemImage = ItemDataBase->GetImageByName(ItemName);
    }
}
