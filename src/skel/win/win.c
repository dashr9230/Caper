
/* temporary including from rwsdk. TODO: remove this */
#include <ostypes.h> /* rwplcore.h */
#include <balibtyp.h>
#include <batypes.h> /* rwplcore.h */
#include <rwstring.h>


#include <windows.h>

#include "../../skeleton.h"
#include "../../platform.h"
#include "win.h"

/* Caper includes */
#include "../../game.h"

static RwBool       ForegroundApp = TRUE;
static RwBool       RwInitialized = FALSE;
static int          settings_field_14E; /* TODO: renamesettings_field_14E */

static float _fog_distance_1[] = { 100.0f, 150.0f, 200.0f };
static float _fog_distance_2[] = { 100.0f, 150.0f, 200.0f };

/* Class name for the MS Window's window class. */

static const RwChar *AppClassName = RWSTRING("RwAppClass");



/* platform specfic global data */

typedef struct
{
    HWND        window;
    HINSTANCE   instance;
    RwBool      fullScreen;
    RwV2d       lastMousePos;
}
psGlobalType;

static psGlobalType PsGlobal;


#define PSGLOBAL(var) (((psGlobalType *)(RsGlobal.ps))->var)

/* Error and warning dialog box titles */

static const RwChar *ErrorDialogTitle =
RWSTRING("RenderWare(tm) Application Error");
static const RwChar *WarningDialogTitle =
RWSTRING("RenderWare(tm) Application Warning");

/*
 *****************************************************************************
 */
void
psErrorMessage(const RwChar *message)
{
    MessageBox(NULL, message, ErrorDialogTitle,
#ifndef UNDER_CE
               MB_TASKMODAL |
#endif
               MB_ICONERROR | MB_TOPMOST | MB_OK);
}


/*
 *****************************************************************************
 */
void
psWarningMessage(const RwChar *message)
{
    MessageBox(NULL, message, WarningDialogTitle,
#ifndef UNDER_CE
               MB_TASKMODAL |
#endif
               MB_ICONWARNING | MB_TOPMOST | MB_OK);
}












/*
 *****************************************************************************
 */
RwUInt32
psTimer(void)
{
    TIMECAPS ptc;
    RwUInt32 time;

#ifdef UNDER_CE
    time = (RwUInt32) GetTickCount();
#else
    timeGetDevCaps(&ptc, sizeof(ptc));
    timeBeginPeriod(ptc.wPeriodMin);
    time = (RwUInt32) timeGetTime();
    timeEndPeriod(ptc.wPeriodMin);
#endif

    return time;
}


/*
 *****************************************************************************
 */
void
psMouseSetVisibility(RwBool visible)
{
    ShowCursor(visible);

    return;
}

static RwBool validDelta = FALSE;


/*
 *****************************************************************************
 */
void
psMouseSetPos(RwV2d *pos)
{
    POINT point;

    validDelta = FALSE;

    point.x = (RwInt32) pos->x;
    point.y = (RwInt32) pos->y;

    ClientToScreen(PSGLOBAL(window), &point);

    SetCursorPos(point.x, point.y);

    return;
}


/*
 *****************************************************************************
 */
RwChar *
psPathnameCreate(const RwChar *srcBuffer)
{
    RwChar *dstBuffer;
    RwChar *charToConvert;

    /*
     * First duplicate the string
     */
    dstBuffer = (RwChar *)
        RwMalloc(sizeof(RwChar) * (rwstrlen(srcBuffer) + 1));

    if( dstBuffer )
    {
        rwstrcpy(dstBuffer, srcBuffer);

        /* 
         * Convert a path for use on Windows. 
         * Convert all /s and :s into \s 
         */
        while( (charToConvert = rwstrchr(dstBuffer, '/')) )
        {
            *charToConvert = '\\';
        }
#if 0
        while( (charToConvert = rwstrchr(dstBuffer, ':')) )
        {
            *charToConvert = '\\';
        }
#endif
    }
}


/*
 *****************************************************************************
 */
void
psPathnameDestroy(RwChar *buffer)
{
    if( buffer )
    {
        RwFree(buffer);
    }

    return;
}


/*
 *****************************************************************************
 */
RwChar
psPathGetSeparator(void)
{
    return '\\';
}


/*
 *****************************************************************************
 */
RwMemoryFunctions *
psGetMemoryFunctions(void)
{
    return 0;
}


/*
 *****************************************************************************
 */
RwBool
psInstallFileSystem(void)
{
    return (TRUE);
}







/*
 *****************************************************************************
 */
static BOOL
InitApplication(HANDLE instance)
{
    /*
     * Perform any necessary MS Windows application initialization. Basically,
     * this means registering the window class for this application.
     */

    WNDCLASS windowClass;

#ifdef UNDER_CE
    windowClass.style = 0;
#else
    windowClass.style = CS_BYTEALIGNWINDOW;
#endif
    windowClass.lpfnWndProc = (WNDPROC) MainWndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = (HINSTANCE)instance;
    windowClass.hIcon = NULL;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = AppClassName;

    return RegisterClass(&windowClass);
}


/*
 *****************************************************************************
 */
static HWND
InitInstance(HANDLE instance)
{
    /*
     * Perform any necessary initialization for this instance of the
     * application.
     *
     * Create the MS Window's window instance for this application. The
     * initial window size is given by the defined camera size. The window
     * is not given a title as we set it during Init3D() with information
     * about the version of RenderWare being used.
     */

    RECT rect;

    rect.left = rect.top = 0;
    rect.right = RsGlobal.maximumWidth;
    rect.bottom = RsGlobal.maximumHeight;

    AdjustWindowRect(&rect, WS_POPUP, FALSE);

    return CreateWindowA(AppClassName, RsGlobal.appName,
                        WS_POPUP,
                        0, 0,
                        rect.right - rect.left, rect.bottom - rect.top,
                        (HWND)NULL, (HMENU)NULL, (HINSTANCE)instance, NULL);
}


/*
 *****************************************************************************
 */
#ifdef UNDER_CE
#define CMDSTR  LPWSTR
#else
#define CMDSTR  LPSTR
#endif


/*
 *****************************************************************************
 */
RwBool
psInitialize(void)
{
    PsGlobal.lastMousePos.x = PsGlobal.lastMousePos.y = 0.0;

    PsGlobal.fullScreen = FALSE;

    RsGlobal.ps = &PsGlobal;

    return TRUE;
}


/*
 *****************************************************************************
 */
void
psTerminate(void)
{
    return;
}


/*
 *****************************************************************************
 */
RwBool
psAlwaysOnTop(RwBool alwaysOnTop)
{
    RECT winRect;
    HWND hwnd;

    hwnd = PSGLOBAL(window);

    GetWindowRect(hwnd, &winRect);

    if( alwaysOnTop )
    {
        return (RwBool)SetWindowPos(hwnd, HWND_TOPMOST,
                                     winRect.left, winRect.top,
                                     winRect.right - winRect.left,
                                     winRect.bottom - winRect.top, 0);
    }
    else
    {
        return (RwBool)SetWindowPos(hwnd, HWND_NOTOPMOST,
                                     winRect.left, winRect.top,
                                     winRect.right - winRect.left,
                                     winRect.bottom - winRect.top, 0);
    }
}




/*
 *****************************************************************************
 */
LRESULT CALLBACK
MainWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: MainWndProc

    /*
     * Let Windows handle all other messages.
     */
    return DefWindowProc(window, message, wParam, lParam);
}






/*
 *****************************************************************************
 */
int PASCAL
WinMain(HINSTANCE instance,
    HINSTANCE prevInstance  __RWUNUSED__,
    CMDSTR cmdLine,
    int cmdShow)
{
    DEVMODE devMode;
    MSG message;
    RwV2d pos;

    /* 
     * Initialize the platform independent data.
     * This will in turn initialize the platform specific data...
     */
    if( RsEventHandler(rsINITIALIZE, NULL) == rsEVENTERROR )
    {
        return FALSE;
    }

    /*
     * Register the window class...
     */
    if( !InitApplication(instance) )
    {
        return FALSE;
    }

    /*
     * Parse any command line parameters before RW startup...
     */
    {
        char               arg[256];
        RwChar            *origCmdLine, *aptr;

        origCmdLine = cmdLine;
        while( *cmdLine )
        {
            aptr = arg;
            arg[0] = '\0';
            while (*cmdLine == ' ')
                cmdLine++;
            while (*cmdLine != '\0' && *cmdLine != ' ')
                *aptr++ = *cmdLine++;
            *aptr = 0;
            if (arg[0])
                RsEventHandler(rsPREINITCOMMANDLINE, arg);
        }
        cmdLine = origCmdLine;
    }

    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
    RsGlobal.maximumWidth = devMode.dmPelsWidth;
    RsGlobal.maximumHeight = devMode.dmPelsHeight;

    PSGLOBAL(fullScreen) = TRUE;

    /*
     * Create the window...
     */
    PSGLOBAL(window) = InitInstance(instance);

    SetForegroundWindow(PSGLOBAL(window));

    if( PSGLOBAL(window) == NULL )
    {
        return FALSE;
    }

    game_window = PSGLOBAL(window);

    PSGLOBAL(instance) = instance;

    if (LoadSettings())
        settings_field_14E = res_data->field_14E;
    else
        res_data->field_14E = settings_field_14E;

    fog_distance_1 = _fog_distance_1[res_data->draw_distance];
    fog_distance_2 = _fog_distance_2[res_data->draw_distance];

    object_detail = res_data->object_detail;
    reflections = res_data->reflections;
    transparencies = res_data->transparencies;

    /* 
     * Initialize the 3D (RenderWare) components of the app...
     */
    if( rsEVENTERROR == RsEventHandler(rsRWINITIALIZE, PSGLOBAL(window)) )
    {
        RsErrorMessage("Failed to Initialise RenderWare");

        DestroyWindow(PSGLOBAL(window));

        RsEventHandler(rsTERMINATE, NULL);

        return FALSE;
    }

    RwInitialized = TRUE;

    /*
     * Parse command line parameters (except program name) one at
     * a time AFTER RenderWare initialization...
     */
    {
        char               arg[256];
        RwChar            *aptr;

        while (*cmdLine)
        {
            aptr = arg;
            arg[0] = '\0';
            while (*cmdLine == ' ')
                cmdLine++;
            while (*cmdLine != '\0' && *cmdLine != ' ')
                *aptr++ = *cmdLine++;
            *aptr = '\0';
            if(rwstrlen(arg))
                RsEventHandler(rsCOMMANDLINE, arg);
        }
    }

    /*
     * Show the window, and refresh it...
     */
    ShowWindow(PSGLOBAL(window), cmdShow);
    UpdateWindow(PSGLOBAL(window));

    RsAlwaysOnTop(TRUE);

    /*
     * Set the initial mouse position...
     */
    pos.x = RsGlobal.maximumWidth * 0.5f;
    pos.y = RsGlobal.maximumHeight * 0.5f;

    RsMouseSetPos(&pos);

    /*
     * Enter the message processing loop...
     */
    while( !RsGlobal.quit )
    {
        if( PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE|PM_NOYIELD) )
        {
            if( message.message == WM_QUIT )
            {
                break;
            }
            else
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        else if( ForegroundApp )
        {
            RsEventHandler( rsIDLE, NULL );
        }
        else
        {
            WaitMessage();
        }
    }

    /* 
     * About to shut down - block resize events again...
     */
    RwInitialized = FALSE;

    SaveSettings();

    RsAlwaysOnTop(FALSE);

    /*
     * Tidy up the 3D (RenderWare) components of the application...
     */
    RsEventHandler(rsRWTERMINATE, NULL);

    /*
     * Kill the window...
     */
    DestroyWindow(PSGLOBAL(window));

    /*
     * Free the platform dependent data...
     */
    RsEventHandler(rsTERMINATE, NULL);

    return message.wParam;
}

/*
 *****************************************************************************
 */
