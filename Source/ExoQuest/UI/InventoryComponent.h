//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "InventoryComponent.generated.h"
//
//
////USTRUCT(BlueprintType)
////struct FInventoryItem
////{
////	GENERATED_BODY()
////
////	// 아이템 데이터
////	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
////	FItem ItemData;
////
////	// 아이템 개수
////	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
////	int32 Quantity;
////};
//
//
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class EXOQUEST_API UInventoryComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	UInventoryComponent();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//public:
//	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
//	//TArray<FInventoryItem> Inventory;
//
//	// 아이템 데이터베이스 참조
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
//	class UItemDataBase* ItemDataBase;
//
//
//
//	UFUNCTION(BlueprintCallable, Category = "Inventory")
//	void AddItem(const FItem& ItemData, int32 Quantity);
//
//	UFUNCTION(BlueprintCallable, Category = "Inventory")
//	void UseItem(const FString& ItemName);
//		
//};
