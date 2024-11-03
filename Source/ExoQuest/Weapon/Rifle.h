#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class EXOQUEST_API ARifle : public AActor
{
	GENERATED_BODY()

public:
	ARifle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



public:
	//// 충돌체
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	//class UCapsuleComponent* capsuleComp;

	// 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComp)
	class UStaticMeshComponent* meshComp;

	// 총구 위치를 나타내는 Scene Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Muzzle")
	class USceneComponent* muzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacterBase* ownerCharacter;

	// 총알 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float damage;

	UFUNCTION(BlueprintCallable)
	void Fire();
};
