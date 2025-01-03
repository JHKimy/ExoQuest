#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataBase.generated.h"


// 아이템 구조체
USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	// 아이템 이름 (이름으로 맵과 연동 되게)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Name;

	// 아이템 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Image;

	// 아이템 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Num;
};




UCLASS()
class EXOQUEST_API UItemDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:


	// 아이템 데이터 베이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItem> Items;


	// *** 이름과 이미지 간의 매핑을 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<FString, UTexture2D*> ItemImageMap;


	// 이름 기반으로 이미지 찾기
	UTexture2D* GetImageByName(const FString& ItemName) const;


	UFUNCTION(BlueprintCallable, Category = "Item")
	FItem FindItem(const FString& ItemName) const;



	// 아이템 추가
	void AddItem(const FString& ItemName, int32 ItemNum);

	// 아이템 사용
	bool UseItem(const FString& ItemName);
};
