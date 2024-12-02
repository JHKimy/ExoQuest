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




	// 충돌체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UCapsuleComponent* capsuleComp;

	// 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComp)
	class UStaticMeshComponent* meshComp;



	// 적 처치 후 생성 효과
	UFUNCTION()
	void Pop(float DeltaTime);


	UFUNCTION()
	void Floating(float DeltaTime);



	
	
	// 충돌
	UFUNCTION()
	void OnBeginOverlap
	(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);



protected:
	// 애니메이션 진행 시간
	float ElapsedTime;

	// 팝업 효과 타이머 핸들
	bool bIsPopping;


	FVector StartBelowLocation;






public:
	// 아이템 이미지 (블루프린트에서 설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemImage;

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
};

