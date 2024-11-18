#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

UCLASS()
class EXOQUEST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // NativeTick ¼±¾ð

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = health)
	FString healthString;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = health)
	float healthRatio;


};
