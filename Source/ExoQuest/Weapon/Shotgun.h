#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shotgun.generated.h"

UCLASS()
class EXOQUEST_API AShotgun : public AActor
{
	GENERATED_BODY()
	
public:	
	AShotgun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
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
	float damage = 5;

	UFUNCTION(BlueprintCallable)
	void Fire();


private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 Pellets = 10;  // 샷건의 총알 개수

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float SpreadAngle = 10.f;  // 총알이 퍼지는 각도

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireRange = 1000.0f;  // 총알이 도달할 거리





	bool bCanFire = true; // 발사 가능 여부
	FTimerHandle FireRateTimerHandle; // 타이머 핸들

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRateDelay = 2.0f; // 발사 대기 시간 (초 단위)

	void ResetFire(); // 발사 가능 상태로 리셋하는 함수

public:
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void ApplyRecoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void RecoverRecoil();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTimerHandle RecoilRecoveryTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRotator recoveryRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMin{ 3.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMax{ 5.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMin{ -0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMax{ 0.3f };

	class APlayerController* PlayerController;


	// 카메라 셰이크 효과 추가
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<class UCameraShakeBase> cameraShakeClass;
};
