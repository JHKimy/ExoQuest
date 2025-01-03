#include "UI/PowerPlantUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/PowerPlant.h"
#include "EngineUtils.h" // TActorIterator�� ����ϱ� ���� �ʿ�


void UPowerPlantUI::NativeConstruct()
{
	Super::NativeConstruct();

	// ������ ������ ����
	PopulateItemData();

	// StarFlux UI ������Ʈ
	UpdateStarFluxDisplay();
}

void UPowerPlantUI::PopulateItemData()
{
	if (!ItemDataBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataBase is null!"));
		return;
	}

	// Medical Kit ������ ����
	if (MedicalKitImage && MedicalKitButton)
	{
		MedicalKitImage->SetBrushFromTexture(ItemDataBase->GetImageByName("MedicalKit"));
		ButtonToItemMap.Add(MedicalKitButton, "MedicalKit");
		MedicalKitButton->OnClicked.AddDynamic(this, &UPowerPlantUI::OnMedicalKitButtonClicked);
	}

	// Ghost Gear ������ ����
	if (GhostGearImage && GhostGearButton)
	{
		GhostGearImage->SetBrushFromTexture(ItemDataBase->GetImageByName("GhostGear"));
		ButtonToItemMap.Add(GhostGearButton, "GhostGear");
		GhostGearButton->OnClicked.AddDynamic(this, &UPowerPlantUI::OnGhostGearButtonClicked);
	}

	// Time Flux Gear ������ ����
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

    // ��ư�� ���ε� ������ �̸� ��������
    FString ItemName = ButtonToItemMap[ClickedButton];
    UE_LOG(LogTemp, Log, TEXT("Button clicked for item: %s"), *ItemName);

    // StarFlux ���� ó��
    FItem StarFluxItem = ItemDataBase->FindItem("Starflux");
    if (StarFluxItem.Num > 0)
    {
        ItemDataBase->UseItem("Starflux");

        // PowerPlant���� ������ ����
        APowerPlant* PowerPlant = nullptr;
        for (TActorIterator<APowerPlant> It(GetWorld()); It; ++It)
        {
            PowerPlant = *It;
            break;
        }

        if (PowerPlant)
        {
            // ItemClasses���� ItemName�� �ش��ϴ� �ε����� ã��
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

        // StarFlux UI ������Ʈ
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
		// StarFlux ���� �ؽ�Ʈ�� ����
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