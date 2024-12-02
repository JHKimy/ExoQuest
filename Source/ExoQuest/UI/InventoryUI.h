#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventorySlot.h" // UInventorySlot �� FInventorySlotData ����
#include "Components/GridPanel.h"
#include "InventoryUI.generated.h"


UCLASS()
class EXOQUEST_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // �������Ʈ���� Grid Panel�� ���ε�
    //UPROPERTY(meta = (BindWidget))
    //UGridPanel* GridPanel;

    // ���� ���� Ŭ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UInventorySlot> InventorySlotClass;

    // ��(Row)�� ��(Column) ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Rows = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Columns = 6;

    // �κ��丮 ������ ������Ʈ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventory(const TArray<FInventorySlotData>& InventoryData);

};
