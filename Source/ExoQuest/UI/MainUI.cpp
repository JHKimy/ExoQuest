#include "MainUI.h"
#include "Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

UMainUI::UMainUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Owning Player�� Pawn�� ���� ĳ���� ���� ���
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		// healthRatio�� �ʱⰪ �Ҵ�
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// �� �����Ӹ��� healthRatio ������Ʈ
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);
	}
}
