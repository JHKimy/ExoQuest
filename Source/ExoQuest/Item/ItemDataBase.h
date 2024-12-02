#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataBase.generated.h"


// ������ ����ü
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Name;

	// ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Image;

	// ������ Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AActor* Class;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Num;
};



UCLASS()
class EXOQUEST_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ������ ������ ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;

	// ������ �߰�
	void AddItem(
		const FString& ItemName,
		UTexture2D* ItemImage,
		AActor* ItemClass,
		int32 Quantity);

	// ������ ���
	bool UseItem(const FString& ItemName);
};
