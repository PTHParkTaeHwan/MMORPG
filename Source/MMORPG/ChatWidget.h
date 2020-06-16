// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MMORPG.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MMORPG_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UTextBlock* ChattingString;

public:
	UFUNCTION()
	void OnInputString();

};
