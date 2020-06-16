// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MMORPG.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class MMORPG_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UChatWidget> HUDChatWidgetClass;

private:
	UPROPERTY()
	class UChatWidget* HUDChatWidget;

	void OnChatPause();

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

};
