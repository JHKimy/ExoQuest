#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketLauncher.generated.h"

UCLASS()
class EXOQUEST_API ARocketLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	ARocketLauncher();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARocketProjectile> RocketProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector MuzzleOffset;





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
	float damage =100.f;


	UFUNCTION(BlueprintCallable)
	void Fire();

	bool bCanFire = true; // 발사 가능 여부
	FTimerHandle FireRateTimerHandle; // 타이머 핸들

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRateDelay = 2.0f; // 발사 대기 시간 (초 단위)

	void ResetFire(); // 발사 가능 상태로 리셋하는 함수

};
