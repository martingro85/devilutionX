#include "devilution.h"
#include "miniwin/ddraw.h"
#include "stubs.h"
#include <SDL.h>
#include <string>

#include "DiabloUI/diabloui.h"
#include "DiabloUI/dialogs.h"

#ifdef _MSC_VER
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#endif

namespace dvl {

DWORD last_error;

DWORD GetLastError()
{
	return last_error;
}

void SetLastError(DWORD dwErrCode)
{
	last_error = dwErrCode;
}

char *_strlwr(char *str)
{
	for (char *p = str; *p; ++p) {
		*p = tolower(*p);
	}
	return str;
}

int wsprintfA(LPSTR dest, LPCSTR format, ...)
{
	va_list args;
	va_start(args, format);
	return vsprintf(dest, format, args);
}

int wvsprintfA(LPSTR dest, LPCSTR format, va_list arglist)
{
	return vsnprintf(dest, 256, format, arglist);
}

int _strcmpi(const char *_Str1, const char *_Str2)
{
	return strcasecmp(_Str1, _Str2);
}

int _strnicmp(const char *_Str1, const char *_Str2, size_t n)
{
	return strncasecmp(_Str1, _Str2, n);
}

char *_itoa(int _Value, char *_Dest, int _Radix)
{
	switch (_Radix) {
	case 8:
		sprintf(_Dest, "%o", _Value);
		break;
	case 10:
		sprintf(_Dest, "%d", _Value);
		break;
	case 16:
		sprintf(_Dest, "%x", _Value);
		break;
	default:
		UNIMPLEMENTED();
		break;
	}

	return _Dest;
}

DWORD GetTickCount()
{
	return SDL_GetTicks();
}

void Sleep(DWORD dwMilliseconds)
{
	SDL_Delay(dwMilliseconds);
}

HANDLE FindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	DUMMY();
	return (HANDLE)-1;
}

WINBOOL FindClose(HANDLE hFindFile)
{
	UNIMPLEMENTED();
	return true;
}

WINBOOL GetComputerNameA(LPSTR lpBuffer, LPDWORD nSize)
{
	DUMMY();
	strncpy(lpBuffer, "localhost", *nSize);
	*nSize = strlen(lpBuffer);
	return true;
}

UINT GetDriveTypeA(LPCSTR lpRootPathName)
{
	return DVL_DRIVE_CDROM;
}

WINBOOL DeleteFileA(LPCSTR lpFileName)
{
	char name[DVL_MAX_PATH];
	TranslateFileName(name, sizeof(name), lpFileName);

	FILE *f = fopen(name, "r+");
	if (f) {
		fclose(f);
		remove(name);
		f = NULL;
		eprintf("Removed file: %s\n", name);
	} else {
		eprintf("Failed to remove file: %s\n", name);
	}

	return true;
}

void FakeWMDestroy()
{
	init_cleanup();
	PostMessageA(NULL, DVL_WM_QUERYENDSESSION, 0, 0);
}

bool SpawnWindow(LPCSTR lpWindowName, int nWidth, int nHeight)
{
	if (SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_HAPTIC) <= -1) {
		SDL_Log(SDL_GetError());
		return false;
	}

	atexit(SDL_Quit);

#ifdef USE_SDL1
	SDL_EnableUNICODE(1);
#endif

	int upscale = 1;
	DvlIntSetting("upscale", &upscale);
	DvlIntSetting("fullscreen", (int *)&fullscreen);

	int grabInput = 1;
	DvlIntSetting("grab input", &grabInput);

#ifdef USE_SDL1
	int flags = SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_HWPALETTE;
	if (fullscreen) {
		SDL_Log("fullscreen not yet supported with SDL1");
	}
	// flags |= fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE;
	SDL_WM_SetCaption(lpWindowName, WINDOW_ICON_NAME);
	SDL_SetVideoMode(nWidth, nHeight, /*bpp=*/0, flags);
	window = SDL_GetVideoSurface();
	if (grabInput)
		SDL_WM_GrabInput(SDL_GRAB_ON);
#else
	int flags = 0;
	if (upscale) {
		flags |= fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;

		char scaleQuality[2] = "2";
		DvlStringSetting("scaling quality", scaleQuality, 2);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, scaleQuality);
	} else if (fullscreen) {
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (grabInput) {
		flags |= SDL_WINDOW_INPUT_GRABBED;
	}

	window = SDL_CreateWindow(lpWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nWidth, nHeight, flags);
#endif
	if (window == NULL) {
		SDL_Log(SDL_GetError());
	}
	atexit(FakeWMDestroy);

	if (upscale) {
#ifdef USE_SDL1
		SDL_Log("upscaling not supported with USE_SDL1");
#else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (renderer == NULL) {
			SDL_Log(SDL_GetError());
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, nWidth, nHeight);
		if (texture == NULL) {
			SDL_Log(SDL_GetError());
		}

		if (SDL_RenderSetLogicalSize(renderer, nWidth, nHeight) <= -1) {
			SDL_Log(SDL_GetError());
		}
#endif
	}

	return window != NULL;
}

BOOL GetVersionExA(LPOSVERSIONINFOA lpVersionInformation)
{
	lpVersionInformation->dwMajorVersion = 5;
	lpVersionInformation->dwMinorVersion = 0;
	lpVersionInformation->dwPlatformId = DVL_VER_PLATFORM_WIN32_NT;
	return true;
}

void lstrcpynA(LPSTR lpString1, LPCSTR lpString2, int iMaxLength)
{
	strncpy(lpString1, lpString2, iMaxLength);
}
} // namespace dvl
