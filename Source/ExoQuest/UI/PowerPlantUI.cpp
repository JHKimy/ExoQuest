#include "UI/PowerPlantUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/PowerPlant.h"
#include "EngineUtils.h" // TActorIterator를 사용하기 위해 필요


void UPowerPlantUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 아이템 데이터 설정
	PopulateItemData();

	// StarFlux UI 업데이트
	UpdateStarFluxDisplay();
}

void UPowerPlantUI::PopulateItemData()
{
	if (!ItemDataBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataBase is null!"));
		return;
	}

	// Medical Kit 데이터 설정
	if (MedicalKitImage && MedicalKitButton)
	{
		MedicalKitImage->SetBrushFromTexture(ItemDataBase->GetImageByName("MedicalKit"));
		ButtonToItemMap.Add(MedicalKitButton, "MedicalKit");
		MedicalKitButton->OnClicked.AddDynamic(this, &UPowerPlantUI::OnMedicalKitButtonClicked);
	}

	// Ghost Gear 데이터 설정
	if (GhostGearImage && GhostGearButton)
	{
		GhostGearImage->SetBrushFromTexture(ItemDataBase->GetImageByName("GhostGear"));
		ButtonToItemMap.Add(GhostGearButton, "GhostGear");
		GhostGearButton->OnClicked.AddDynamic(this, &UPowerPlantUI::OnGhostGearButtonClicked);
	}

	// Time Flux Gear 데이터 설정
	if (TimeFluxGearImage && TimeFluxGearButton)
	{
		TimeFluxGearImage->SetBrushFromTexture(ItemDataBase->GetImageByName("TimeFluxGear"));
		ButtonToItemMap.Add(TimeFluxGearButton, "TimeFluxGear");
		TimeFluxGearButton->OnClicked.AddDynamic(this, &UPowerPlantUI::OnTimeFluxGearButtonClicked);
	}
}

void UPowerPlantUI::OnMedicalKitButtonClicked()
{
	OnButtonClicked(MedicalKitButton);
}



void UPowerPlantUI::OnTimeFluxGearButtonClicked()
{
	OnButtonClicked(GhostGearButton);
}

void UPowerPlantUI::OnGhostGearButtonClicked()
{
	OnButtonClicked(TimeFluxGearButton);
}


void UPowerPlantUI::OnButtonClicked(UButton* ClickedButton)
{
    if (!ClickedButton || !ButtonToItemMap.Contains(ClickedButton))
    {
        UE_LOG(LogTemp, Warning, TEXT("Clicked button not found in map."));
        return;
    }

    // 버튼에 매핑된 아이템 이름 가져오기
    FString ItemName = ButtonToItemMap[ClickedButton];
    UE_LOG(LogTemp, Log, TEXT("Button clicked for item: %s"), *ItemName);

    // StarFlux 감소 처리
    FItem StarFluxItem = ItemDataBase->FindItem("Starflux");
    if (StarFluxItem.Num > 0)
    {
        ItemDataBase->UseItem("Starflux");

        // PowerPlant에서 아이템 스폰
        APowerPlant* PowerPlant = nullptr;
        for (TActorIterator<APowerPlant> It(GetWorld()); It; ++It)
        {
            PowerPlant = *It;
            break;
        }

        if (PowerPlant)
        {
            // ItemClasses에서 ItemName에 해당하는 인덱스를 찾음
            int32 ItemIndex = INDEX_NONE;
            for (int32 i = 0; i < PowerPlant->ItemClasses.Num(); ++i)
            {
                if (PowerPlant->ItemClasses[i] && PowerPlant->ItemClasses[i]->GetName().Contains(ItemName))
                {
                    ItemIndex = i;
                    break;
                }
            }

            if (ItemIndex != INDEX_NONE)
            {
                PowerPlant->SpawnItemByIndex(ItemIndex);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Item %s not found in PowerPlant's ItemClasses!"), *ItemName);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No PowerPlant found in the world!"));
        }

        // StarFlux UI 업데이트
        UpdateStarFluxDisplay();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough StarFlux to purchase %s!"), *ItemName);
    }
}


void UPowerPlantUI::UpdateStarFluxDisplay()
{
	if (StarFluxText && ItemDataBase)
	{
		// StarFlux 값을 텍스트로 설정
		FItem StarFluxItem = ItemDataBase->FindItem("Starflux");
		if (StarFluxItem.Num > 0)
		{
			StarFluxText->SetText(FText::FromString(FString::Printf(TEXT("%d"), StarFluxItem.Num)));
		}
		else
		{
			StarFluxText->SetText(FText::FromString(TEXT("0")));
		}
	}
}