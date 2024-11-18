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

	// Owning Player의 Pawn을 통해 캐릭터 참조 얻기
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		// healthRatio에 초기값 할당
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 매 프레임마다 healthRatio 업데이트
	ACharacterBase* Character = Cast<ACharacterBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Character)
	{
		healthRatio = Character->health / Character->maxHealth;
		healthString = FString::FromInt(Character->health);
	}
}
