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
        AddSlot(Item.Name, Item.Num);
    }
}

void UInventoryUI::AddSlot(FString ItemName, int32 ItemNum)
{
    // ���� ���� ����
    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

    if (NewSlot)
    {
        // ���� �ʱ�ȭ (��: ������ �̸�, �̹���, ����)
        NewSlot->UpdateSlotData(ItemName, ItemNum);

        // ������ Wrap Box�� �߰�
        InventoryBox->AddChild(NewSlot);
        UE_LOG(LogTemp, Log, TEXT("Slot added to InventoryBox"));
    }
}
