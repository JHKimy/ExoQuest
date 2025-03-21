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
    UButton* SlotButton; // 버튼 위젯

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    UTexture2D* ItemImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    int32 ItemNum;

    // 데이터베이스 참조
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
    UItemDataBase* ItemDataBase;

    // 슬롯 데이터를 업데이트하는 함수 추가
    UFUNCTION(BlueprintCallable, Category = "Slot")
    void UpdateSlotData(const FString& NewItemName, int32 NewItemNum);

    UFUNCTION(BlueprintCallable, Category = "Slot")
    void OnSlotClicked(); // 클릭 이벤트 처리

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    ACharacterBase* character; // 블루프린트 캐릭터 클래스 참조

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TSubclassOf<ACharacterBase> CharacterBlueprintClass; // 블루프린트 캐릭터 클래스 참조

};