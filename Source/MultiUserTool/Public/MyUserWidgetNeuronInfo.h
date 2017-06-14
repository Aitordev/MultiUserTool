// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MyUserWidgetNeuronInfo.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API UMyUserWidgetNeuronInfo : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NeuronUi)
	int32 currentidneuron;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NeuronUi)
	int32 currentIdActorNotification;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NeuronUi)
	FString CurrentNeuronInfo;

	/** SetInfo funcion rpc to do in server*/
	UFUNCTION(BlueprintCallable, Category = NeuronUI)
	void SetInfo(int32 idneuron, int32 idInfo, const FString& NeuronInfo);
};
