#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataBase.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	// ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;

	// ������ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AActor> ItemClass;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Description;
};



UCLASS()
class EXOQUEST_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ������ ������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;

};
