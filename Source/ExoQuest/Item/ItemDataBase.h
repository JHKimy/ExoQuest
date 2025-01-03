#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataBase.generated.h"


// ������ ����ü
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	// ������ �̸� (�̸����� �ʰ� ���� �ǰ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Name;

	// ������ �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Image;

	// ������ ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Num;
};




UCLASS()
class EXOQUEST_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:


	// ������ ������ ���̽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;


	// *** �̸��� �̹��� ���� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<FString, UTexture2D*> ItemImageMap;


	// �̸� ������� �̹��� ã��
	UTexture2D* GetImageByName(const FString& ItemName) const;


	UFUNCTION(BlueprintCallable, Category = "Item")
	FItem FindItem(const FString& ItemName) const;



	// ������ �߰�
	void AddItem(const FString& ItemName, int32 ItemNum);

	// ������ ���
	bool UseItem(const FString& ItemName);
};
