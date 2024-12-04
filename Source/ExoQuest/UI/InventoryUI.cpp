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
        AddSlot(Item.Name, Item.Num);
    }
}

void UInventoryUI::AddSlot(FString ItemName, int32 ItemNum)
{
    // 슬롯 위젯 생성
    UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotClass);

    if (NewSlot)
    {
        // 슬롯 초기화 (예: 아이템 이름, 이미지, 수량)
        NewSlot->UpdateSlotData(ItemName, ItemNum);

        // 슬롯을 Wrap Box에 추가
        InventoryBox->AddChild(NewSlot);
        UE_LOG(LogTemp, Log, TEXT("Slot added to InventoryBox"));
    }
}
