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

	// Į ���� �浹 ����
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* attackCollision;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float damage;

	// �� Ÿ�� �� ȣ��� �Լ� (�浹 ó��)
	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
