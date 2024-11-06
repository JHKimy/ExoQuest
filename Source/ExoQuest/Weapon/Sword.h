#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class EXOQUEST_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	ASword();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Mesh)
	class UStaticMeshComponent* meshComp;

	// 칼 공격 충돌 영역
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* attackCollision;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float damage;

	// 적 타격 시 호출될 함수 (충돌 처리)
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
