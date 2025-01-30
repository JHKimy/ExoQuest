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
	TObjectPtr<UParticleSystem> bulletEffect;

	// 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float damage;

	UFUNCTION(BlueprintCallable)
	void Fire();


	// 발사 쿨다운 시간
	float fireCooldown {0.1f};

	// 마지막 발사 시간
	float lastFireTime;




	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void ApplyRecoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void RecoverRecoil();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTimerHandle RecoilRecoveryTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRotator recoveryRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMin{ 0.2f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMax{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMin{ -0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMax{ 0.3f };

	class APlayerController* PlayerController;
};
