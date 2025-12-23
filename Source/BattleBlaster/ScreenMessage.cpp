// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetScreenMessage(const FString& ScreenMessage)
{
	MessageTextBlock->SetText(FText::FromString(ScreenMessage));
}
