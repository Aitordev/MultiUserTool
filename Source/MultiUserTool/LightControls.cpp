// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "LightControls.h"
#include "Local.h"
#include "LightControls.h"
#include "MyHUD.h"




void *Zmalloc(int sz) { void *p = malloc(sz); memset(p, 0, sz); return p; }
#define ZMALLOC(p) (p*)Zmalloc(sizeof(p)) 

NEURON_REF::NEURON_REF(ANeuron *neurona, FVector pos, FRotator rot) {
	NeuronaGenerada = nullptr;
	Neurona = neurona;
	Pos = pos;
	Rot = rot;
	Radio = 200.0f;
}

NEURON_REF::NEURON_REF(ACodeGeneratedNeuron *neurona, FVector pos, FRotator rot) {
	NeuronaGenerada = neurona;
	Neurona = nullptr;
	Pos = pos;
	Rot = rot;
	Radio = 200.0f;
}

HUD_INTERFACE *NEW_HUD_INTERFACE()
{
	HUD_INTERFACE *c = ZMALLOC(HUD_INTERFACE);
	/*
	if (NUM_CONTROLS == 0) {
		CONTROL_REGISTER.Add(nullptr);//EL PRIMERO NO SE USA
		NUM_CONTROLS++;
	}
	CONTROL_REGISTER.Add(c);
	c->id = NUM_CONTROLS++;
	*/
	return c;
}

HUD_CONTROL *NEW_HUD_CONTROL()
{
	HUD_CONTROL *c = ZMALLOC(HUD_CONTROL);
	if (NUM_CONTROLS == 0) {
		CONTROL_REGISTER.Add(nullptr);//EL PRIMERO NO SE USA
		NUM_CONTROLS++;
	}
	CONTROL_REGISTER.Add(c);
	c->id = NUM_CONTROLS++;
	return c;
}

HUD_MENU *NEW_HUD_MENU()
{
	HUD_MENU *c = ZMALLOC(HUD_MENU);
	if (NUM_CONTROLS == 0) {
		CONTROL_REGISTER.Add(nullptr);//EL PRIMERO NO SE USA
		NUM_CONTROLS++;
	}
	CONTROL_REGISTER.Add(c);
	c->id = NUM_CONTROLS++;
	return c;
}

HUD_SLIDER *NEW_HUD_SLIDER()
{
	HUD_SLIDER*c = ZMALLOC(HUD_SLIDER);
	/*
	if (NUM_CONTROLS == 0) {
		CONTROL_REGISTER.Add(nullptr);//EL PRIMERO NO SE USA
		NUM_CONTROLS++;
	}
	*/
	CONTROL_REGISTER.Add(c);
	c->id = NUM_CONTROLS++;
	return c;
}
/*
NEURON_REF::NEURON_REF(ACodeGeneratedNeuron * neurona, FVector pos, FRotator rot)
{
	NeuronaGenerada = neurona;
	Neurona = nullptr;
	Pos = pos;
	Rot = rot;
	Radio = 200.0f;
}
*/

void HUD_CONTROL::ReSize(float wnew, float hnew)
{
	if (xmax == SCREEN_WIDTH) {
		xmin += wnew - SCREEN_WIDTH;
		xmax += wnew - SCREEN_WIDTH;
	}
}


HUD_MENU *AddMenu(char *nombre, int x, int y, int w, int h)
{
	//extern UFont *Font;
	//extern UCanvas *Canvas;
	static int j = 0;
	HUD_MENU *c = NEW_HUD_MENU();
	c->tipo = 2;
	c->open = false;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->next = NULL;
	c->opciones = NULL;
	c->Xmin = c->xmin;
	c->Xmax = c->xmax;
	c->Ymin = c->ymin;
	c->Ymax = c->ymax;
	c->dxt = 20;
	c->dyt = 4;
	sprintf(c->texto, nombre);
	c->text = FString(nombre);// ::Printf(TEXT("MENU %d"), j++);
	//float szh, szv;
	//Canvas->TextSize(Font, c->text, szh, szv, 1.0f, 1.0f);
	//c->dxt = (c->w - szh) / 2.0f;
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}

void HUD_MENU::AddOpcion(char *texto)
{
	HUD_CONTROL *c = NEW_HUD_CONTROL();
	HUD_CONTROL *opc = opciones;
	while (opc) {
		if (opc->next == NULL) {
			c->tipo = 4;
			c->xmin = opc->xmin;
			c->ymin = opc->ymin + opc->h;
			c->xmax = opc->xmax;
			c->ymax = opc->ymin + 2 * opc->h;
			c->w = opc->w;
			c->h = opc->h;
			c->next = NULL;
			Ymax = c->ymax;
			sprintf(c->texto, texto);
			c->text = FString(texto);// ::Printf(TEXT("OPCION"));
			opc->next = c;
			return;
		}
		opc = opc->next;
	}
	c->tipo = 4;
	c->xmin = xmin;
	c->ymin = ymin + h;
	c->xmax = xmax;
	c->ymax = ymin + 2 * h;
	c->next = NULL;
	c->w = w;
	c->h = h;
	Ymax = c->ymax;
	sprintf(c->texto, texto);
	c->text = FString(texto);// ::Printf(TEXT("OPCION"));
	opciones = c;
}



bool HUD_CONTROL::IsMouseInside(void)
{
	int mx = (int)GFMX;
	int my = (int)GFMY;
	int dy = 2;
	if (tipo == 2) dy = h / 2;
	if (mx > xmin && my > ymin-2 && mx < xmax && my < ymax+dy) {
		SELECTED_CONTROL = id;  
		return true;
	}
	return false;
}

bool TRACKING_SLIDER4 = false;

void HUD_SLIDER::Update(void)
{
	if (IsMouseInside()) {
		SELECTED_CONTROL = id;
		if (GLBD) {
			pos = (GFMX - (float)(xmin + h)) / (float)(w - 2 * h);
			pos = fmin(fmax(0.0f, pos), 1.0f);
			valor= vmin + (vmax - vmin)*pos;
			*Bind = valor;
			TRACKING_SLIDER4 = true;
		} else {
			TRACKING_SLIDER4 = false;
		}
	}
}

void HUD_SLIDER::Render(UCanvas *Canvas)
{
	//static FColor Rojo = FColor(255, 0, 0, 255);
	//static FColor Verde = FColor(0, 255, 0, 255);
	int ymed = (ymin + ymax) / 2;
	//Canvas->SetDrawColor(Verde);// 0, 255, 0, 255);
	//Canvas->K2_DrawBox(FVector2D(xmin, ymin), FVector2D(w, h), 2);
	//Canvas->K2_DrawBox(FVector2D(xmin, ymed), FVector2D(w, 3), 2);
	if(TEXTURA_SLIDER) Canvas->K2_DrawTexture(TEXTURA_SLIDER, FVector2D(xmin, ymin), FVector2D(w, h), FVector2D(0, 0), FVector2D(1, 1), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), BLEND_Opaque, 0.0f, FVector2D(xmin, ymin));
	int x = xmin + h + (int)(pos*(float)(w - 2 * h));
	int s = 6;
	//Canvas->K2_DrawBox(FVector2D(x-s, ymed), FVector2D(2*s, 2*s), 2);
	if(TEXTURA_TRACKER) Canvas->K2_DrawTexture(TEXTURA_TRACKER, FVector2D(x - s, ymed - s), FVector2D(2 * s, 2 * s), FVector2D(0, 0), FVector2D(1, 1), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), BLEND_Opaque, 0.0f, FVector2D(xmin, ymin));
	char aux[256];
	sprintf(aux, "%s=%2.3f", texto, valor);
	FString S = ToTCHAR(aux);
	GHUD->DrawText(S, FColor::Green, xmax + 10, ymin + 5, GHUD->HUDFont);
}

/*
// Used to set the callback on keyboard dialog close
delegate KeyboardInputFunction(optional String TextInput);


//Launches saved callback function (we can't pass delegates directly)

exec function ProcessKeyboardInput()
{
KeyboardInputFunction();
}


// Keyboard call interface: opens native iOS keyboard to receive user's text input

// @param 	Title 			Dialog title
// @param 	DefaultText 	Default text input value
// @param 	InputFuntion 	Function to be called on dialog close (callback)

function MobileOpenKeyboard(String Title, String DefaultText, delegate<KeyboardInputFunction> InputFuntion)
{
KeyboardInputFunction = InputFuntion;
ConsoleCommand("mobile getuserinput \""$Title$"\" \""$DefaultText$"\" \""$nameof(ProcessKeyboardInput)$"\"");
}


//EXAMPLE: HOW TO USE

exec function SomeBtnPressed()
{
MobileOpenKeyboard("Please enter your name", "Tought Guy", SetPlayerName);
}


// EXAMPLE: CALLBACK FUNCTION

function SetPlayerName(optional string TextInput)
{
log("New player name is:" @ TextInput);
}
*/

/* TEXT METRIC
void TextSize
(
const UFont * InFont,
const FString & InText,
float & XL,
float & YL,
float ScaleX,
float ScaleY
)



*/

/*
HUD_CONTROL::HUD_CONTROL() {
	memset(this, 0, sizeof(HUD_CONTROL));
	if (NUM_CONTROLS == 0) {
		CONTROL_REGISTER.Add(nullptr);//EL PRIMERO NO SE USA
		NUM_CONTROLS++;
	}
	CONTROL_REGISTER.Add(this);
	id = NUM_CONTROLS++;
}
*/

bool cmpt(char *a, char *b)
{
	while (*a != 0 && *b != 0) {
		if (*a++ != *b++) return false;
	}
	if (*a != *b) return false;
	return true;
}

bool GET_MENU_OPTION(char *path,char *opcion, bool clicked)
{
	if (!SELECTED_CONTROL) return false;
	HUD_CONTROL *c = CONTROL_REGISTER[SELECTED_CONTROL];
	sprintf(opcion, "%s/%s", path, opcion);
	return true;
}

HUD_CONTROL *GET_SELECTED_CONTROL()
{
	return CONTROL_REGISTER[SELECTED_CONTROL];
}

bool IS_CONTROL_CLICKED(char *name, bool clicked)
{
	if (!SELECTED_CONTROL) return false;
	HUD_CONTROL *c = CONTROL_REGISTER[SELECTED_CONTROL];
	if (cmpt(c->texto, name) && clicked) return true;
	return false;
}

bool GET_CONTROL_CLICKED(char *file, bool clicked)
{
	if (!SELECTED_CONTROL) return false;
	HUD_CONTROL *c = CONTROL_REGISTER[SELECTED_CONTROL];
	if (c->tipo == 4 && clicked) {
		sprintf(file, "C:\\TFG\\scenes\\%s", c->texto);
		return true;
	}
	return false;
}




void HUD_CONTROL::Render(UCanvas *Canvas)
{
	if (tipo == 2 && ((HUD_MENU *)this)->open) {
		HUD_MENU *m = ((HUD_MENU *)this);
		int mx = (int)GFMX;
		int my = (int)GFMY;
		if (mx > m->Xmin && my > m->Ymin && mx < m->Xmax && my < m->Ymax) {
			((HUD_MENU *)this)->open = true;
		}
		else {
			((HUD_MENU *)this)->open = false;
		}
	}
	if (tipo == 1) {
		((HUD_SLIDER *)this)->Update();
		((HUD_SLIDER *)this)->Render(Canvas);
	}
	else {
		//static FColor Rojo = FColor(255, 0, 0, 255);
		//static FColor Verde = FColor(0, 255, 0, 255);
		int dxt = w / 4;
		int dyt = h / 4;
		int ymed = (ymin + ymax) / 2;
		//Canvas->SetDrawColor(Verde);// 0, 255, 0, 255);
		//if(TEXTURA_BOTON) Canvas->K2_DrawTexture(TEXTURA_BOTON, FVector2D(xmin, ymin), FVector2D(w, h), FVector2D(0, 0), FVector2D(1, 1), FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), BLEND_Opaque, 0.0f, FVector2D(xmin, ymin));

		//Canvas->K2_DrawBox(FVector2D(xmin, ymin), FVector2D(w, h), 2);
		//Canvas->K2_DrawBox(FVector2D(xmin, ymed), FVector2D(w, 3), 2);
		if (IsMouseInside()) {

			GHUD->DrawText(text, FColor::Green, xmin + dxt, ymin + dyt, GHUD->HUDFont);
			if (tipo == 2) {
				((HUD_MENU *)this)->open = true;
			}
			//if(tipo==0) 
			SELECTED_CONTROL = id;
		}
		else {
			GHUD->DrawText(text, FColor::White, xmin + dxt, ymin + dyt, GHUD->HUDFont);
		}
		if (tipo == 2 && ((HUD_MENU *)this)->open == true) {
			HUD_CONTROL *opc = ((HUD_MENU *)this)->opciones;
			while (opc) {
				opc->Render(Canvas);
				opc = opc->next;
			}
		}
	}
}





HUD_CONTROL *AddControl(char *txt, int x, int y, int w, int h)
{
	HUD_CONTROL *c = NEW_HUD_CONTROL();
	static int j = 0;
	c->tipo = 0;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->next = NULL;
	sprintf(c->texto, txt);
	c->text = FString(txt);// ::Printf(TEXT("BOTON %d"), j++);
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}

HUD_SLIDER *AddSlider(char *txt, int x, int y, int w, int h, float *bind,  float vmin, float vmax, float vi)
{
	HUD_SLIDER *c = NEW_HUD_SLIDER();
	c->tipo = 1;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->Bind = bind;
	c->valor = vi;
	c->vmin = vmin;
	c->vmax = vmax;
	c->next = NULL;
	sprintf(c->texto, txt);
	c->text = FString(txt);// ::Printf(TEXT("%s"), (void *)txt);
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}


/*
HANDLE WINAPI FindFirstFile(
_In_  LPCTSTR           lpFileName,
_Out_ LPWIN32_FIND_DATA lpFindFileData
);
BOOL WINAPI FindNextFile(
_In_  HANDLE            hFindFile,
_Out_ LPWIN32_FIND_DATA lpFindFileData
);

BOOL WINAPI FindClose(
_Inout_ HANDLE hFindFile
);
*/
/*
char *ToChar(const TCHAR *txt)
{
	size_t newsize = wcslen(txt) + 1;
	char * nstring = new char[newsize];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, nstring, newsize, txt, _TRUNCATE);
	return nstring;
}

TCHAR *ToTCHAR(char *txt)
{
	size_t newsize = strlen(txt) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, txt, _TRUNCATE);
	return wcstring;
}
*/



class FFileManagerGeneric;

void CreateDirMenu(char *menu, char *dir)
{
	TArray<FString> files;

	//FFileManagerGeneric *Fm=new FFileManagerGeneric();
	IFileManager& FileManager = IFileManager::Get();
	FString FinalPath = "C://TFG/scenes";// +"/" + "*.txt";
	FString Term = "*.txt";
	FileManager.FindFiles(files, *FinalPath, *Term);// TEXT("/Game/scenes"));
	HUD_MENU *m = AddMenu(menu, 0, 0, 250, 40);
	for (int i = 0; i < files.Num(); i++) {
		FString S = files[i];
		//const TCHAR* TheMessage = *TheString;
		const TCHAR* aux = *S;// (TCHAR *)(S.GetCharArray());
		m->AddOpcion(ToChar(aux));
	}
	/*
	m->AddOpcion("escena1.txt");
	m->AddOpcion("escena2.txt");
	m->AddOpcion("LowRes_4.txt");
	*/
	/*
	TCHAR *Term = ToTCHAR(".txt");
	TCHAR *Dir = ToTCHAR(dir);
	WIN32_FIND_DATA fdata;
	HANDLE h = FindFirstFile(ToTCHAR("C:\\TFG\\scenes"), &fdata);

	HUD_MENU *m = AddMenu(menu, 0, 0, 150, 40);
	if (h!= INVALID_HANDLE_VALUE) {
	m->AddOpcion(ToChar(fdata.cFileName));
	while (FindNextFile(h, &fdata)) {
	if (1){//wcsstr(fdata.cFileName, Term)) {
	m->AddOpcion(ToChar(fdata.cFileName));
	}
	}
	m->AddOpcion("fin");
	FindClose(h);
	}
	else {
	m->AddOpcion("no dir");
	}
	*/

}






LightControls::LightControls()
{
}

LightControls::~LightControls()
{
}
