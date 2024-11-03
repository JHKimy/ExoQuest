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
	//// �浹ü
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	//class UCapsuleComponent* capsuleComp;

	// �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComp)
	class UStaticMeshComponent* meshComp;

	// �ѱ� ��ġ�� ��Ÿ���� Scene Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Muzzle")
	class USceneComponent* muzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacterBase* ownerCharacter;

	// �Ѿ� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float damage;

	UFUNCTION(BlueprintCallable)
	void Fire();
};
