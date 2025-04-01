// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy1HPBar.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API UEnemy1HPBar : public UUserWidget
{
	GENERATED_BODY()
public:
    void NativeConstruct();
    void Init(class AEnemyCharacter* OwnerCharacter);
    void UpdateHP(float Percent);

protected:
    //UPROPERTY(meta = (BindWidget))
    //class UProgressBar* HP_ProgressBar;

private:
    class AEnemyCharacter* Owner;

    //UPROPERTY(meta = (BindWidget))
    //float HP_ProgressBar;
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float percent;
};
