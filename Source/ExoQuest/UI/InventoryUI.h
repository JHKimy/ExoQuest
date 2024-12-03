#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventorySlot.h"
#include "Components/WrapBox.h"
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

    //// ���� ���� Ŭ����
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    //TSubclassOf<UInventorySlot> InventorySlotClass;

    //// ��(Row)�� ��(Column) ����
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    //int32 Rows = 4;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    //int32 Columns = 6;

protected:
    /** Wrap Box: ������ ��� �����̳� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UWrapBox> InventoryBox;


    //UPROPERTY(meta = (BindWidget))
    //TObjectPtr<class UButton>Login;
    //if (Login)
    //{
    //    Login->OnClicked.AddDynamic(this, &ULogin::PressLogin);
    //    Login->OnHovered.AddDynamic(this, &ULogin::HoverLog);
    //}
    // Login->OnClicked.AddDynamic(this, &ULogin::PressLogin);




public:
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventorySlot)
    //TObjectPtr<UInventorySlot>SlotUI;


    /** ���� ���� Ŭ���� (�������Ʈ ���� ����) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TSubclassOf<UInventorySlot> InventorySlotClass;


    // �κ��丮 ������ ������Ʈ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateInventory();


    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddSlot(FString ItemName, UTexture2D* ItemImage, int32 ItemQuantity);


};
