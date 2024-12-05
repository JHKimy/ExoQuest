// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerPlant.generated.h"

UCLASS()
class EXOQUEST_API APowerPlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerPlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;






	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "StaticMesh")
	TObjectPtr<class UStaticMeshComponent> StaticMeshInstance;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "StaticMesh")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Collision")
	class USphereComponent* CollisionSphere;


	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "PlayerController")
	class APlayerController* PlayerController;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "PlayerController")
	class ACharacterBase* PlayerCharacter;

	// 충돌 함수
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Inventory")
	TSubclassOf<class UUserWidget> UIClass;

	// 실제 인벤토리 UI 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Inventory")
	class UUserWidget* UI;
};
