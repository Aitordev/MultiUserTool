// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "XPlayerController.h"
#include "MyUserWidgetNeuronInfo.h"

void UMyUserWidgetNeuronInfo::SetInfo(int32 idneuron, int32 idInfo, const FString& NeuronInfo)
{
	AXPlayerController* MyPC = Cast<AXPlayerController>(GetOwningPlayer());
	if (MyPC) {
		MyPC->SetInfoNeuron(idneuron, idInfo, NeuronInfo);
	}
}



