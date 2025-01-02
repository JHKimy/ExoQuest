// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Character/CharacterBase.h"
#include "Item/ItemDataBase.h"  // FItem ���� ����
#include "EQGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API UEQGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// ĳ���� ���¸� ������ ����
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<EWeaponType> SavedEquippedWeapons;  // ������ ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType SavedPrimaryWeapon;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ECharacterState SavedState;  // ĳ���� ����

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SavedHealth;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SavedStamina;
	
	// ������ �����ͺ��̽��� ����� ������ ���
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	TArray<FItem> SavedItems;  // FItem �迭�� ������ ����

	//// ���� ĳ���� ���� ������
	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Inventory")
	//TArray<UInventorySlot> SavedSlots;  // ���� ������ ����

};