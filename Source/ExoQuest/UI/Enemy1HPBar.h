#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy1HPBar.generated.h"

UCLASS()
class EXOQUEST_API UEnemy1HPBar : public UUserWidget
{
	GENERATED_BODY()
public:
    void NativeConstruct();
    void UpdateHP(float Percent);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float maxHealthPercent = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float healthPercent;
};
