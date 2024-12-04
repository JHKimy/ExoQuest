#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthItem.generated.h"

UCLASS()
class EXOQUEST_API AHealthItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class UStaticMeshComponent* meshComp;

	// 칼 공격 충돌 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	class UCapsuleComponent* capsuleCollision;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 충돌 함수
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);







	// ========
	// 인벤토리 
	// ========
public:

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	//// 아이템 이미지 (블루프린트에서 설정 가능)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	//UTexture2D* ItemImage;

	// 아이템 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemNum;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	//class UItemDataBase* ItemDataBase;
};
