// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MyUserWidgetNeuron.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API UMyUserWidgetNeuron : public UUserWidget
{
	GENERATED_BODY()

public:
	/** HideEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void HideEnhacedNeurons();

	/** HideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void HideNotEnhacedNeurons();

	/** UnHideEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void UnHideEnhacedNeurons();

	/** UnHideNotEnhacedNeurons funcion rpc to do in server*/
	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void UnHideNotEnhacedNeurons();
		
};
