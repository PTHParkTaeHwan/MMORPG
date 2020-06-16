// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Components/WidgetComponent.h"
#include "ChatWidget.h"
#include "UObject/ConstructorHelpers.h"


AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UChatWidget> UI_CHAT_C(TEXT("WidgetBlueprint'/Game/Dev/UI/TextRender/UI_ChatWidget.UI_ChatWidget_C'"));
	if (UI_CHAT_C.Succeeded())
	{
		HUDChatWidgetClass = UI_CHAT_C.Class;
	}

}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Chat"), EInputEvent::IE_Pressed, this, &AMyPlayerController::OnChatPause);

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//WidgetSpawn();	
	//HUDChatWidget = CreateWidget<UChatWidget>(this, HUDChatWidgetClass);
	//HUDChatWidget->AddToViewport();

}

void AMyPlayerController::OnChatPause()
{
	HUDChatWidget = CreateWidget<UChatWidget>(this, HUDChatWidgetClass);
	HUDChatWidget->AddToViewport(3);
	SetInputMode(UIInputMode);
	bShowMouseCursor = true;
}
