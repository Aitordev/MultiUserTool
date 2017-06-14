// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/HUD.h"
#include "Funciones.h"
#include "Neuron.h"
#include "LightControls.h"
#include "Local.h"
#include "MyHUD.generated.h"

/**
*
*/

#define THUD_BUTTON 0
#define THUD_SLIDER 1
#define THUD_MENU	2
#define THUD_OPCION	4

class HUD_INTERFACE;
class NEURON_REF;

USTRUCT()
struct FBoton {
	GENERATED_USTRUCT_BODY()

		//Vars
	int32 		type;
	FString	toolTip;
	float 		minX;
	float 		maxX;
	float 		minY;
	float 		maxY;

	//~

	//default properties

	FBoton()
	{
		type = -1;
		toolTip = "";
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
	}
};

UCLASS()
class MULTIUSERTOOL_API AMyHUD : public AHUD
{
	GENERATED_BODY()
		AMyHUD(const class FObjectInitializer& PCIP);
	//Variables para almacenar la fuente.
public:
	UPROPERTY()
		UFont* HUDFont;

	/** Cursor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* CursorMain;

	/** Hovering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* CursorHoveringButton;

	/** Button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = T2D)
		UTexture2D* ButtonBackground;

	// Materials 
	/** Events */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterialInterface* MaterialBackground;

	/* Draw Hud? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Options)
		bool DontDrawHUD;
	
	FVector2D MouseLocation;
	void DrawHUD_DrawCursor();

	const FLinearColor * ColorPtr;

	//Colors
	static const FColor		FColorBlack;
	static const FColor		FColorRed;
	static const FColor		FColorGreen;
	static const FColor		FColorYellow;
	static const FColor		FColorBlue;
	static const FColor		FColor_White;

	static const FLinearColor LC_Black;
	static const FLinearColor LC_Pink;
	static const FLinearColor LC_Red;
	static const FLinearColor LC_Yellow;


	//UFUNCTION(Category = "My HUD", BlueprintCallable)
	void OnButtonX();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Reference to an SCompoundWidget, TSharedPtr adds to the refcount of MyUIWidget
	/////MyUIWidget will not self-destruct as long as refcount > 0
	/////MyUIWidget refcount will be (refcout-1) if HUD is destroyed.
	TSharedPtr<class SSlateChat> MyUIWidget;// Reference to the main chat widget
 
	//TSharedPtr<SCombo> Combo001;
	//TSharedPtr<SCombo> Combo002;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Called as soon as game starts, create SCompoundWidget and give Viewport access
	APlayerController* ThePC;
	
	//UFUNCTION(BlueprintCallable, Category = "User")
	// A Blueprint function you can use to place messages in the chat box during runtime
	//void AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate); 


	//Primera llamada para pinter el hud
	virtual void DrawHUD() override;
	virtual void Tick(float DeltaSeconds) override;


	void BeginPlay();
	void DrawControls(void);
	void RenderDebugText(int X, int Y);
	void DrawCruz(int X, int Y, int sz, FColor color);
	void ResizeControls(float neww, float newh);
	int  SelectNeuron();
	float UpdateFps(float DeltaSeconds);

	bool LoadResources();
	HUD_INTERFACE *CreateDefaultInterface();

	virtual void PostInitializeComponents() override;
	void UpdateInput();
	HUD_CONTROL *AddControl(char *nombre, int x, int y, int w, int h);
	HUD_MENU    *AddMenu(char *nombre, int x, int y, int w, int h);
	HUD_CONTROL *AddOption(char *nombre, int x, int y, int w, int h);
	HUD_SLIDER  *AddSlider(char *txt, int x, int y, int w, int h, float *bind, float vmin, float vmax, float vi);
	//HUD_MENU	*CreateDirMenu(char *menu, char *dir,char *ter);
	HUD_MENU *CreateDirMenu(char *menu,const char *dir,const char *ter, int X, int Y, int W, int H);

	void DrawMainMenu();

	FORCEINLINE void XDrawTile(UTexture2D* tex, float x, float y, float w, float h, const FColor& TheColor)
	{
		if (!Canvas) return;
		if (!tex) return;
		Canvas->SetDrawColor(TheColor);
		int wt = tex->GetSurfaceWidth(); //texture width from start
		int ht = tex->GetSurfaceHeight(); //texture height from start
		Canvas->DrawTile(tex, x, y, w, h, 0, 0, wt, ht, BLEND_Translucent);
	}

	
	FORCEINLINE void DrawButton(int X, int Y, int W, int H,const FString &text, bool enhance) {
		XDrawTile(ButtonBackground, X, Y,W,H, FColor::White);
		float escala = 1.0f;
		if (enhance) {
			DrawText(text, FColor::Green, X, Y, HUDFont);
		} else {
			DrawText(text, FColor::White, X, Y, HUDFont);
		}
	}
	FORCEINLINE void DrawControl(HUD_CONTROL *c,bool enhance) {//int X, int Y, int W, int H, const FString &text, bool enhance) {
		
		XDrawTile(ButtonBackground,c->xmin,c->ymin,c->w, c->h, FColor::White);
		float escala = 1.0f;
		if (enhance) {
			DrawText(c->text, FColor::Green, c->xmin+c->dxt,c->ymin+c->dyt, HUDFont);
		}
		else {
			DrawText(c->text, FColor::White, c->xmin + c->dxt, c->ymin + c->dyt, HUDFont);
		}
	}

	void DrawMenu(HUD_MENU *c, bool enhance);
	/*
	void DrawMenu(HUD_MENU *c, bool enhance) {//int X, int Y, int W, int H, const FString &text, bool enhance) {
		XDrawTile(ButtonBackground, c->xmin, c->ymin, c->w, c->h, FColor::White);
		float escala = 1.0f;
		if (enhance) {
			DrawText(c->text, FColor::Green, c->xmin + c->dxt, c->ymin + c->dyt, HUDFont);
		}
		else {
			DrawText(c->text, FColor::White, c->xmin + c->dxt, c->ymin + c->dyt, HUDFont);
		}
		if (enhance) {
			HUD_CONTROL *opc = c->opciones;
			while (opc) {
				DrawControl(opc, false);
				opc = opc->next;
			}
		}
	}
	*/

	FORCEINLINE void DrawSlider(HUD_SLIDER *c) {//int X, int Y, int W, int H, const FString &text, bool enhance) {
		XDrawTile(ButtonBackground, c->xmin, c->ymin, c->w, c->h, FColor::White);
		int x = c->xmin + c->h + (int)(c->pos*(float)(c->w - 2 * c->h));
		int s = 6;
		int y = (c->ymin + c->ymax) / 2;
		XDrawTile(ButtonBackground, x-s, y-s, 2*s, 2*s, FColor::White);
		char aux[256];
		sprintf(aux, "%s=%2.3f", c->texto, c->valor);
		FString S = ToTCHAR(aux);
		DrawText(S, FColor::Green, c->xmax + 10, c->ymin + 5, GHUD->HUDFont);
	}

	//Stop Camera From Moving With Mouse
	FORCEINLINE void SetCursorMoveOnly(bool CursorOnly)
	{
		if (!ThePC) return;
		//
		ThePC->SetIgnoreLookInput(CursorOnly);

	}

	void CreateDirMenu2(char *menu, char *dir);
	HUD_CONTROL *CreateNeuronSliders(int X, int Y);
	void DispatchActions();
	void RenderSliders();
	void RenderSliders(HUD_CONTROL *S);
	void RenderControls();
	ANeuron *AddNeurona();
	bool On(char *txt);
	bool OnMenu(char *txt);


};

