// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIUSERTOOL_API UMyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My New User Widget")
	FString MyNewWidgetName="Adios";
	
	/*Responsible for adding the ItemToAdd to our UI*/
	UFUNCTION(BlueprintCallable, Category = "Events")
	FString AddItemToUI();
	
};
