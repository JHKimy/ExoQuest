#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventorySlot.h" // UInventorySlot 및 FInventorySlotData 포함
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
    // 블루프린트에서 Grid Panel을 바인딩
    //UPROPERTY(meta = (BindWidget))
    //UGridPanel* GridPanel;

    // 슬롯 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UInventorySlot> InventorySlotClass;

    // 행(Row)과 열(Column) 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Rows = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Columns = 6;

    // 인벤토리 데이터 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventory(const TArray<FInventorySlotData>& InventoryData);

};
