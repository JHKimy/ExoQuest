#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EQPlayerController.generated.h"



class UInputMappingContext;
class UInputAction;


UCLASS()
class EXOQUEST_API AEQPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEQPlayerController();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

//====================================================
private:
	void SetupInputBindings(class ACharacterBase* myCharacter);
//====================================================

private:
	class ACharacterBase* MyControlledCharacter;







// ==============
// �Է� �ý���
// ==============
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IMC")
	TObjectPtr<UInputMappingContext> IMC_Control;

	// ���콺 Ŭ�� �Է�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_MouseRightClickMove;
	// WASD �Է�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_WASD;
	// ���콺 ȸ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Rotate;
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Jump;
	// ���� �޸���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Run;
	// �뽬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Dash;
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Attack;
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Zoom;
	// ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_SwordAttack;

	// ����ź ��ô
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_ThrowGrenade;

	// �� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_1;

	// ����ź ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_2;
};
