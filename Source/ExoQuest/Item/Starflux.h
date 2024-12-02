// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Starflux.generated.h"

UCLASS(Blueprintable)
class EXOQUEST_API AStarflux : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStarflux();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	FVector StartPos;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float Amplitude;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float RotationSpeed;




	// �浹ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* capsuleComp;

	// �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComp)
	class UStaticMeshComponent* meshComp;



	// �� óġ �� ���� ȿ��
	UFUNCTION()
	void Pop(float DeltaTime);


	UFUNCTION()
	void Floating(float DeltaTime);



	
	
	// �浹
	UFUNCTION()
	void OnBeginOverlap
	(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);



protected:
	// �ִϸ��̼� ���� �ð�
	float ElapsedTime;

	// �˾� ȿ�� Ÿ�̸� �ڵ�
	bool bIsPopping;


	FVector StartBelowLocation;






public:
	// ������ �̹��� (�������Ʈ���� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemImage;

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
};

