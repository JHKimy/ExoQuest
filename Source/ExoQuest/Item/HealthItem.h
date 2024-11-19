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

	// Į ���� �浹 ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	class UCapsuleComponent* capsuleCollision;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// �浹 �Լ�
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);

};
