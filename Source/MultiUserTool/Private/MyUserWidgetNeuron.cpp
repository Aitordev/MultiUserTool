// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerController.h"
#include "MyUserWidgetNeuron.h"

void UMyUserWidgetNeuron::HideEnhacedNeurons()
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetOwningPlayer());
	if (MyPC) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("widgetfunction"));
		MyPC->HideEnhacedNeurons();
	}
}

void UMyUserWidgetNeuron::HideNotEnhacedNeurons()
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetOwningPlayer());
	if (MyPC) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("widgetfunction"));
		MyPC->HideNotEnhacedNeurons();
	}
}

void UMyUserWidgetNeuron::UnHideEnhacedNeurons()
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetOwningPlayer());
	if (MyPC) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("widgetfunction"));
		MyPC->UnHideEnhacedNeurons();
	}
}

void UMyUserWidgetNeuron::UnHideNotEnhacedNeurons()
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetOwningPlayer());
	if (MyPC) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("widgetfunction"));
		MyPC->UnHideNotEnhacedNeurons();
	}
}
