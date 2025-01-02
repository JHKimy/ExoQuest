// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"  // FItem 정의 포함
#include "EQGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API UEQGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 캐릭터 상태를 저장할 변수
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<EWeaponType> SavedEquippedWeapons;  // 장착된 무기 정보

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType SavedPrimaryWeapon;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ECharacterState SavedState;  // 캐릭터 상태

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SavedHealth;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SavedStamina;
	
	// 아이템 데이터베이스에 저장된 아이템 목록
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	TArray<FItem> SavedItems;  // FItem 배열로 아이템 저장

	//// 기존 캐릭터 상태 변수들
	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	//TArray<UInventorySlot> SavedSlots;  // 슬롯 데이터 저장

};