#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponForEquip.generated.h"

UENUM(BlueprintType)
enum class EWeaponTypeForEquip : uint8
{
	Rifle,
	Shotgun,
	RocketLauncher,
	Sword
};

UCLASS()
class EXOQUEST_API AWeaponForEquip : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponForEquip();



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	FVector StartPos;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float Amplitude;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = WeaponType)
	EWeaponTypeForEquip WeaponType;




	// ���� ����� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class UStaticMeshComponent> Weapon;

	// �浹ü 
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "BoxCollision")
	class UBoxComponent* BoxComponent;

	// �޽�
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = StaticMesh)
	class UStaticMeshComponent* MeshComponent;



	// ���� ������ ������
	void Floating();


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
