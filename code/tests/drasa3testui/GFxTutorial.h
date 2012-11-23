#pragma once 
#ifndef __GFX_TUTORIAL_H__
#define __GFX_TUTORIAL_H__

#include "GTimer.h"
#include "GFxEvent.h"
#include "GFxPlayer.h"
#include "GFxFontLib.h"

#include "FxPlayerLog.h"
#include "GRendererD3D9.h"
#include <MMSystem.h>
#include <core/refcounted.h>

namespace ScaleForms
{
class GFxTutorial : public Core::RefCounted
{
	__DeclareClass(GFxTutorial);
public:
	GFxTutorial();
	~GFxTutorial();
	bool InitGFx();
	//void OnCreateDevice(IDirect3DDevice9* pd3dDevice);
	void OnResetDevice(/*IDirect3DDevice9* pd3dDevice*/);
	void OnLostDevice();
	void OnDestroyDevice();
	void StorePresentParameters(D3DPRESENT_PARAMETERS p);
	void AdvanceAndRender(void);
	void ProcessEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool *pbNoFurtherProcessing);
	void SetTextboxFocus(bool f) { textboxHasFocus = f; }
	void LogPrintf(const char *format, va_list al);

	void ProcessEvent(const GFxMouseEvent& mevent);

private:

	// One GFxLoader per application
	GFxLoader           gfxLoader;

	// One GFxMovieDef per SWF/GFx file
	GPtr<GFxMovieDef>   pUIMovieDef;

	// One GFxMovieView per playing instance of movie
	GPtr<GFxMovieView>  pUIMovie;

	// D3D9 Renderer
	GPtr<GRendererD3D9>   pRenderer;
	GPtr<GFxRenderConfig> pRenderConfig;
	GPtr<GFxRenderStats>  pRenderStats;

	D3DPRESENT_PARAMETERS	presentParams;

	// Movie frame time
	//double MovieLastTime;
	DWORD MovieLastTime;

	// Textbox Focus
	bool textboxHasFocus;
};
}

#endif