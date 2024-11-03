#include "GameMode/GameRoomMode.h"
#include "GameRoomMode.h"
#include "Player/EQPlayerController.h"
#include "Character/CharacterBase.h"


AGameRoomMode::AGameRoomMode()
{
	// C++로 만들고 BP로 덮어주기
	// BP가 제일 마지막 쯤에 생성 되기 때문
	// DefaultPawnClass = ACharacterBase::StaticClass();
	// PlayerControllerClass = AEQPlayerController::StaticClass();
}
