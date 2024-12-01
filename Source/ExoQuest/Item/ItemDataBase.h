#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataBase.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	// 아이템 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;

	// 아이템 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AActor> ItemClass;

	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Description;
};



UCLASS()
class EXOQUEST_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 아이템 데이터 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;

};
