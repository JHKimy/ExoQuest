#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDataBase.h"
#include "Character/CharacterBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "InventorySlot.generated.h"

UCLASS()
class EXOQUEST_API UInventorySlot : public UUserWidget
{
    GENERATED_BODY()

public:

    void NativeConstruct();

    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton; // ��ư ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    UTexture2D* ItemImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    int32 ItemNum;

    // �����ͺ��̽� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
    UItemDataBase* ItemDataBase;

    // ���� �����͸� ������Ʈ�ϴ� �Լ� �߰�
    UFUNCTION(BlueprintCallable, Category = "Slot")
    void UpdateSlotData(const FString& NewItemName, int32 NewItemNum);

    UFUNCTION(BlueprintCallable, Category = "Slot")
    void OnSlotClicked(); // Ŭ�� �̺�Ʈ ó��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    ACharacterBase* character; // �������Ʈ ĳ���� Ŭ���� ����

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TSubclassOf<ACharacterBase> CharacterBlueprintClass; // �������Ʈ ĳ���� Ŭ���� ����

};