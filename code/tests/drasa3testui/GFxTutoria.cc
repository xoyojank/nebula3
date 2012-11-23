#include "stdneb.h"
#include "GFxTutorial.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/renderdevice.h"
#include "framesync/framesynctimer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/inputserver.h"

//--------------------------------------------------------------------------------------
// GFxTutorial Implementation
//--------------------------------------------------------------------------------------
namespace ScaleForms
{
using namespace CoreGraphics;

__ImplementClass(GFxTutorial, 'GFTL', Core::RefCounted);

#define UIMOVIE_FILENAME "d3d9guide.swf"

GFxTutorial::GFxTutorial()
{
	MovieLastTime = 0;
}

GFxTutorial::~GFxTutorial()
{
	pUIMovie = NULL;
	pUIMovieDef = NULL;
}

bool GFxTutorial::InitGFx()
{
	// Initialize logging -- GFx will print errors to the log
	// stream. GFx log messages are very helpful when
	// debugging. To generate a simpele error, comment out
	// gfxLoader.SetFileOpener(pfileOpener).
	gfxLoader.SetLog(GPtr<GFxLog>(*new GFxPlayerLog()));

	// Give the loader the default file opener
	GPtr<GFxFileOpener> pfileOpener = *new GFxFileOpener;
	gfxLoader.SetFileOpener(pfileOpener);

	// Associate the renderer with the GFxLoader -- this is applied to all children,
	// meaning we do not have to register the renderer with individual GFxMovieView
	// objects.
	pRenderer = *GRendererD3D9::CreateRenderer();
	pRenderConfig = *new GFxRenderConfig(pRenderer);
	if(!pRenderer || !pRenderConfig)
		return false;
	gfxLoader.SetRenderConfig(pRenderConfig);

	// Use EdgeAA to improve the appearance of the interface without the computational
	// expense of full AA through the video card.
	pRenderConfig->SetRenderFlags(GFxRenderConfig::RF_EdgeAA);

	// Load the movie and crate instance.
	pUIMovieDef = *(gfxLoader.CreateMovie(UIMOVIE_FILENAME,
		GFxLoader::LoadKeepBindData |
		GFxLoader::LoadWaitFrame1));
	if(!pUIMovieDef)
		return false;

	// We pass 'true' argument to make sure that first frame is initialized.
	// We can use 'false' if we need to pass some information to the movie
	// before its first frame 1 ActionScript logic is executed.
	pUIMovie = *pUIMovieDef->CreateInstance(GFxMovieDef::MemoryParams(), true);
	if(!pUIMovie)
		return false;

	// Advance the movie to the first frame
	pUIMovie->Advance(0.0f, 0);

	// Note the time to determine the amount of time elapsed between this frame and the next
	MovieLastTime = timeGetTime();//FrameSync::FrameSyncTimer::Instance()->GetFrameTime();

	// Set the background stage color to alpha blend with the underlying 3D environment
	pUIMovie->SetBackgroundAlpha(0.0f);

	// Set the scale mode to preserve the aspect ratio of the Flash content
	pUIMovie->SetViewScaleMode(GFxMovieView::SM_ShowAll);
	pUIMovie->SetViewAlignment(GFxMovieView::Align_CenterRight);

	return true;
}

//void GFxTutorial::OnCreateDevice(IDirect3DDevice9* pd3dDevice)
//{
//}
//
void GFxTutorial::OnResetDevice(/*IDirect3DDevice9* pd3dDevice*/)
{
	DisplayDevice* displayDevice = DisplayDevice::Instance();
	HWND hWND = displayDevice->GetHwnd();
	RenderDevice* renderDevice = RenderDevice::Instance();
	pRenderer->SetDependentVideoMode(renderDevice->GetDirect3DDevice(), &presentParams, GRendererD3D9::VMConfig_NoSceneCalls /* VMCFlags */, hWND);

	// Use the window client rect size as the viewport.
	D3DVIEWPORT9 vp;
	if(renderDevice->GetDirect3DDevice()->GetViewport(&vp) == D3D_OK)
		pUIMovie->SetViewport(vp.Width, vp.Height, 0, 0, vp.Width, vp.Height);
}

void GFxTutorial::OnLostDevice()
{
	pRenderer->ResetVideoMode();
}

void GFxTutorial::OnDestroyDevice()
{
}

void GFxTutorial::StorePresentParameters(D3DPRESENT_PARAMETERS p)
{
	presentParams = p;
}

void GFxTutorial::AdvanceAndRender(void)
{
	//double mtime = FrameSync::FrameSyncTimer::Instance()->GetFrameTime();
	//double deltaTime = ((double)(mtime - MovieLastTime)) / 1000.0f;
	//MovieLastTime = mtime;

	DWORD mtime = timeGetTime();
	float deltaTime = ((float)(mtime - MovieLastTime)) / 1000.0f;
	MovieLastTime = mtime;

	pUIMovie->Advance((float)deltaTime, 0);
	pUIMovie->Display();
}

void GFxTutorial::ProcessEvent(const GFxMouseEvent& mevent)
{
	if(pUIMovie)
	{
		bool processedMouseEvent = false;
		if( mevent.Type == GFxMouseEvent::MouseMove)
		{
			pUIMovie->HandleEvent(mevent);
			processedMouseEvent = true;
		}

		HWND hWnd = DisplayDevice::Instance()->GetHwnd();
		if( mevent.Type == GFxMouseEvent::MouseDown)
		{	
			pUIMovie->HandleEvent(mevent);
			processedMouseEvent = true;
		}

		if( mevent.Type == GFxMouseEvent::MouseUp)
		{
			//::ReleaseCapture();
			pUIMovie->HandleEvent(mevent);
			processedMouseEvent = true;
		}
		if( processedMouseEvent )
			pUIMovie->HitTest((Float)mevent.x, (Float)mevent.y, GFxMovieView::HitTest_Shapes);		
	}
}


void GFxTutorial::LogPrintf(const char *format, va_list al)
{
	//if(pLog)
	//	pLog->LogMessageVarg(GFxLog::Log_Channel_General, format, al);
}

static struct
{
	WPARAM winKey;
	GFxKey::Code appKey;
} KeyCodeMap[] =
{
	{VK_BACK,    GFxKey::Backspace},
	{VK_TAB,     GFxKey::Tab},
	{VK_CLEAR,   GFxKey::Clear},
	{VK_RETURN,  GFxKey::Return},
	{VK_SHIFT,   GFxKey::Shift},
	{VK_CONTROL, GFxKey::Control},
	{VK_MENU,    GFxKey::Alt},
	{VK_PAUSE,   GFxKey::Pause},
	{VK_CAPITAL, GFxKey::CapsLock},
	{VK_ESCAPE,  GFxKey::Escape},
	{VK_SPACE,   GFxKey::Space},
	{VK_PRIOR,   GFxKey::PageUp},
	{VK_NEXT,    GFxKey::PageDown},
	{VK_END,     GFxKey::End},
	{VK_HOME,    GFxKey::Home},
	{VK_LEFT,    GFxKey::Left},
	{VK_UP,      GFxKey::Up},
	{VK_RIGHT,   GFxKey::Right},
	{VK_DOWN,    GFxKey::Down},
	{VK_INSERT,  GFxKey::Insert},
	{VK_DELETE,  GFxKey::Delete},
	{VK_HELP,    GFxKey::Help},
	{VK_NUMLOCK, GFxKey::NumLock},
	{VK_SCROLL,  GFxKey::ScrollLock},

	{VK_OEM_1,     GFxKey::Semicolon},
	{VK_OEM_PLUS,  GFxKey::Equal},
	{VK_OEM_COMMA, GFxKey::Comma},
	{VK_OEM_MINUS, GFxKey::Minus},
	{VK_OEM_PERIOD,GFxKey::Period},
	{VK_OEM_2,     GFxKey::Slash},
	{VK_OEM_3,     GFxKey::Bar},
	{VK_OEM_4,     GFxKey::BracketLeft},
	{VK_OEM_5,     GFxKey::Backslash},
	{VK_OEM_6,     GFxKey::BracketRight},
	{VK_OEM_7,     GFxKey::Quote}
};

enum KeyModifiers
{
	KM_Control = 0x1,
	KM_Alt     = 0x2,
	KM_Shift   = 0x4,
	KM_Num     = 0x8,
	KM_Caps    = 0x10,
	KM_Scroll  = 0x20
};

static int GetModifiers()
{
	int new_mods = 0;
	if (::GetKeyState(VK_SHIFT) & 0x8000)
		new_mods |= KM_Shift;
	if (::GetKeyState(VK_CONTROL) & 0x8000)
		new_mods |= KM_Control;
	if (::GetKeyState(VK_MENU) & 0x8000)
		new_mods |= KM_Alt;
	if (::GetKeyState(VK_NUMLOCK) & 0x1)
		new_mods |= KM_Num;
	if (::GetKeyState(VK_CAPITAL) & 0x1)
		new_mods |= KM_Caps;
	if (::GetKeyState(VK_SCROLL) & 0x1)
		new_mods |= KM_Scroll;
	return new_mods;
}

void KeyEvent(GFxMovieView *pMovie, GFxKey::Code keyCode, unsigned char asciiCode,
			  unsigned int wcharCode, unsigned int mods, bool down)
{
	if(pMovie == NULL)
		return;

	if (keyCode != GFxKey::VoidSymbol)
	{
		GFxKeyEvent keyEvent(down ? GFxEvent::KeyDown : GFxKeyEvent::KeyUp, keyCode, asciiCode, wcharCode);
		keyEvent.SpecialKeysState.SetShiftPressed(mods & KM_Shift ? 1 : 0);
		keyEvent.SpecialKeysState.SetCtrlPressed(mods & KM_Control ? 1 : 0);
		keyEvent.SpecialKeysState.SetAltPressed(mods & KM_Alt ? 1 : 0);
		keyEvent.SpecialKeysState.SetNumToggled(mods & KM_Num ? 1 : 0);
		keyEvent.SpecialKeysState.SetCapsToggled(mods & KM_Caps ? 1 : 0);
		keyEvent.SpecialKeysState.SetScrollToggled(mods & KM_Scroll ? 1 : 0);
		pMovie->HandleEvent(keyEvent);
	}
}

void OnKey(GFxMovieView *pMovie, GFxKey::Code keyCode, unsigned char asciiCode,
		   unsigned int wcharCode, unsigned int mods, bool downFlag)
{
	if(pMovie == NULL)
		return;

	// See if CTRL is held down. If the event is the acual CTRL key going
	// up or down, ignore it.
	bool ctrl = mods & KM_Control;
	if(keyCode == GFxKey::Control)
		return;

	if (ctrl && downFlag)
	{
		// Todo: Application can handle Ctrl-Key combinations here
	}

	// Change to if(!ctrl && downFlag) to filter out key up events
	if(!ctrl)
	{
		// Inform the player about keystroke
		if (!ctrl)
			KeyEvent(pMovie, (GFxKey::Code)keyCode, asciiCode, wcharCode, mods, downFlag);
	}
}

void ProcessKeyEvent(GFxMovieView *pMovie, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(pMovie == NULL)
		return;

	if(uMsg == WM_CHAR)
	{
		UInt32 wcharCode = (UInt32)wParam;
		GFxCharEvent charEvent(wcharCode);
		pMovie->HandleEvent(charEvent);
		return;
	}

	GFxKey::Code kc = GFxKey::VoidSymbol;
	if (wParam >= 'A' && wParam <= 'Z')
	{
		kc = (GFxKey::Code) ((wParam - 'A') + GFxKey::A);
	}
	else if (wParam >= VK_F1 && wParam <= VK_F15)
	{
		kc = (GFxKey::Code) ((wParam - VK_F1) + GFxKey::F1);
	}
	else if (wParam >= '0' && wParam <= '9')
	{
		kc = (GFxKey::Code) ((wParam - '0') + GFxKey::Num0);
	}
	else if (wParam >= VK_NUMPAD0 && wParam <= VK_DIVIDE)
	{
		kc = (GFxKey::Code) ((wParam - VK_NUMPAD0) + GFxKey::KP_0);
	}
	else
	{
		for (int i = 0; KeyCodeMap[i].winKey != 0; i++)
		{
			if (wParam == (UInt)KeyCodeMap[i].winKey)
			{
				kc = KeyCodeMap[i].appKey;
				break;
			}
		}
	}
	unsigned char asciiCode = 0;
	if (kc != GFxKey::VoidSymbol)
	{
		// get the ASCII code, if possible.
		UINT uScanCode = ((UINT)lParam >> 16) & 0xFF; // fetch the scancode
		BYTE ks[256];
		WORD charCode;

		// Get the current keyboard state
		::GetKeyboardState(ks);

		if (::ToAscii((UINT)wParam, uScanCode, ks, &charCode, 0) > 0)
		{
			asciiCode = LOBYTE (charCode);
		}
	}
	bool downFlag = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) ? 1 : 0;
	OnKey(pMovie, kc, asciiCode, 0, GetModifiers(), downFlag);
}
}