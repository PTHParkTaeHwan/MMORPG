// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "Components/TextBlock.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChattingString = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBox")));



}

void UChatWidget::OnInputString()
{
}