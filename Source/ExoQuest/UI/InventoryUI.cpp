// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryUI.h"
#include "Components/TextBlock.h" //  �ؽ�Ʈ ǥ�ø� ���� ���
#include "Components/Image.h"     //  �̹��� ǥ�ø� ���� ���
#include "Item/ItemDataBase.h"         // UItemDataBase Ŭ���� ����
#include "UI/InventorySlot.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeConstruct()
{
    Super::NativeConstruct();

    // �κ��丮 ������Ʈ
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



void UInventoryUI::UpdateInventory()
{
    // Wrap Box �ʱ�ȭ (���� ���� ����)
    InventoryBox->ClearChildren();

    ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // ItemDataBase�� Items �迭�� ��ȸ�ϸ� ���� �߰�
    for (const FItem& Item : Character->ItemDataBase->Items)
    {
        // Wrap Box�� ������ �߰�
        AddSlot(Item.Name, Item.Image, Item.Num);
    }
}

void UInventoryUI::AddSlot(FString ItemName, UTexture2D* ItemImage, int32 ItemQuantity)
{
    // ���� ���� ����
    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

    if (NewSlot)
    {
        // ���� �ʱ�ȭ (��: ������ �̸�, �̹���, ����)
        NewSlot->UpdateSlotData(ItemName, ItemImage, ItemQuantity);

        // ������ Wrap Box�� �߰�
        InventoryBox->AddChild(NewSlot);
        UE_LOG(LogTemp, Log, TEXT("Slot added to InventoryBox"));
    }
}
