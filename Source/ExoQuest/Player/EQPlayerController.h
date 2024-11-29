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
// 입력 시스템
// ==============
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IMC")
	TObjectPtr<UInputMappingContext> IMC_Control;

	// 마우스 클릭 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_MouseRightClickMove;
	// WASD 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_WASD;
	// 마우스 회전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Rotate;
	// 점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Jump;
	// 빨리 달리기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Run;
	// 대쉬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Dash;
	// 공격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Attack;
	// 조준 줌인
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_Zoom;
	// 대검 공격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_SwordAttack;

	// 수류탄 투척
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_ThrowGrenade;

	// 주 무기 변동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_1;

	// 수류탄 변동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> IA_2;
};
