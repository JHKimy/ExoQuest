#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDataBase.h"
#include "InventorySlot.generated.h"


UCLASS()
class EXOQUEST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
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
};