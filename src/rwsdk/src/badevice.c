/*
 * Device handling.
 *
 * Devices are the libraries interface onto a piece of rendering technology
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwengine
 * \page rwengineoverview RwEngine Overview
 *
 * This object exposes RenderWare Graphics engine. The engine manages:
 *
 * \li memory handling
 * \li plugin registration;
 * \li enumerating and querying available output devices;
 * \li setting and querying video modes;
 * \li Texture memory;
 * \li obtaining focus in a windowed environment;
 * \li Metrics (in Metrics Builds only) and;
 * \li initializing and terminating RenderWare Graphics itself.
 *
 * All RenderWare Graphics applications must Initialize, Open, then Start the RenderWare Graphics
 * Engine before entering their main loop.
 *
 * All plugins must be registered - by calling their respective "PluginAttach()" 
 * functions - prior to calling \ref RwEngineStart.
 *
*/

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"

#include "bamemory.h"

/* String abstraction API for unicode support */
#include "rwstring.h"

/* The core modules */
#include "baerr.h"
#include "bavector.h"
#include "bacolor.h"
#include "bamatrix.h"
#include "baframe.h"
#include "bastream.h"
#include "bacamera.h"
#include "baimage.h"
#include "baraster.h"
#include "batextur.h"


#include "badebug.h"

#include "bafsys.h"

/* devices !! */
#include "batkreg.h"
#include "badevice.h"






/****************************************************************************
 Globals -- across program
 */

static RwUInt32     engineInstancesOpened = 0;

/* These are global to all instances */
static RwPluginRegistry engineTKList = 
{ sizeof(RwGlobals),
  sizeof(RwGlobals),
#ifdef RWGLOBALSIZE
  RWGLOBALSIZE,
#else  /* RWGLOBALSIZE */
  0,
#endif  /* RWGLOBALSIZE */
  0, 
  (RwPluginRegEntry *)NULL, 
  (RwPluginRegEntry *)NULL };

/* The majority of globals across the library */
#ifndef RWGLOBALSIZE
void               *RwEngineInstance = NULL;
static RwGlobals    staticGlobals;
#else /* RWGLOBALSIZE */


#endif /* RWGLOBALSIZE */



/****************************************************************************
 Local (static) Globals
 */


#if (defined(RWDEBUG) && defined(RWMEMDEBUG) && defined(_MSC_VER))
#if ((_MSC_VER>=1000) && defined(_DEBUG))
static _CrtMemState _InitialMemState;
#endif /* ((_MSC_VER>=1000) && defined(_DEBUG)) */
#endif /* (defined(RWDEBUG) && defined(RWMEMDEBUG) && defined(_MSC_VER)) */





/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Device layer

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 CorePluginAttach

 On entry   :
 On exit    : TRUE on success
 */

static              RwBool
CorePluginAttach(void)
{
    RwInt32             state = 0;

    RWFUNCTION(RWSTRING("CorePluginAttach"));

    /* Register a bunch of default modules */
    state |=
        RwEngineRegisterPlugin(sizeof(rwErrorGlobals), rwID_ERRORMODULE,
                               _rwErrorOpen, _rwErrorClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwVectorGlobals), rwID_VECTORMODULE,
                               _rwVectorOpen, _rwVectorClose);
    state |=
        RwEngineRegisterPlugin(0, rwID_COLORMODULE,
                               _rwColorOpen, _rwColorClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwMatrixGlobals), rwID_MATRIXMODULE,
                               _rwMatrixOpen, _rwMatrixClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwFrameGlobals), rwID_FRAMEMODULE,
                               _rwFrameOpen, _rwFrameClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwStreamGlobals), rwID_STREAMMODULE,
                               _rwStreamModuleOpen, _rwStreamModuleClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwCameraGlobals), rwID_CAMERAMODULE,
                               _rwCameraOpen, _rwCameraClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwImageGlobals), rwID_IMAGEMODULE,
                               _rwImageOpen, _rwImageClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwRasterGlobals), rwID_RASTERMODULE,
                               _rwRasterOpen, _rwRasterClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwTextureGlobals), rwID_TEXTUREMODULE,
                               _rwTextureOpen, _rwTextureClose);

    // TODO: CorePluginAttach
}

static void *
MallocWrapper(RwFreeList *fl)
{
    RWFUNCTION(RWSTRING("MallocWrapper"));

    RWRETURN(RwMalloc(fl->entrySize));
}

static RwFreeList *
FreeWrapper(RwFreeList *fl, void *pData)
{
    RWFUNCTION(RWSTRING("FreeWrapper"));

    RwFree(pData);

    RWRETURN(fl);
}












/****************************************************************************
 _rwGetNumEngineInstances

 On entry   : None
 On exit    : Number of engine instances
 */
RwUInt32
_rwGetNumEngineInstances(void)
{
    RWFUNCTION(RWSTRING("_rwGetNumEngineInstances"));
    RWRETURN(engineInstancesOpened);
}







/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Adding modules

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwengine
 * \ref RwEngineRegisterPlugin is used to register a plugin and reserve
 * some space within the global engine space. This must happen after the
 * engine has been initialized but before the engine is opened and is the
 * mechanism for adding plugins to RenderWare. The global data is initialized
 * using the specified object constructor when \ref RwEngineStart is called.
 * Note that a NULL return value from the constructor will cause 
 * \ref RwEngineStart to fail.
 *
 * \param size  A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A Rwint32 value equal to the plugin ID (must be unique - used to
 *       indentify binary chunks).
 * \param initCB  Constructor for the plugin data block.
 * \param termCB  Destructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the engine
 *        global variables of memory reserved for this plugin or -1 if there
 *        is an error.
 *
 * \see RwEngineGetPluginOffset
 * \see RwEngineInit
 *
 */
RwInt32
RwEngineRegisterPlugin(RwInt32 size,
                       RwUInt32 pluginID,
                       RwPluginObjectConstructor initCB,
                       RwPluginObjectDestructor termCB)
{
    RwInt32 plug;

    RWAPIFUNCTION(RWSTRING("RwEngineRegisterPlugin"));
    RWASSERT(engineInstancesOpened == 0);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on (no stream or copy constructor) */
    plug = _rwPluginRegistryAddPlugin(&engineTKList, 
                                     size, 
                                     pluginID,
                                     initCB, 
                                     termCB, 
                                     (RwPluginObjectCopy)NULL);


    RWRETURN(plug);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetPluginOffset is used to get the offset of a previously
 * registered RenderWare engine plugin.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns a RwInt32 value equal to the data block offset if successful
 * or -1 if the plugin is not registered.
 *
 * \see RwEngineRegisterPlugin
 *
 */
RwInt32
RwEngineGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32 offset;

    RWAPIFUNCTION(RWSTRING("RwEngineGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&engineTKList, pluginID);

    RWRETURN(offset);
}



/**
 * \ingroup rwengine
 * \ref RwEngineStop is used to stop the RenderWare engine. This
 * function must be called before any attempt is made to close (and then
 * terminate, if required) the rendering engine.
 *
 * Note the sequence of function calls used to run-up and run-down the RenderWare
 * engine is as follows: \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to
 * run-up and \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineClose
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineStop(void)
{
    // TODO: RwEngineStop

    return FALSE;
}

/**
 * \ingroup rwengine
 * \ref RwEngineStart is used to start the RenderWare engine. This
 * function must only be called after the rendering engine has been
 * initialized and opened (in that order). This function also initializes
 * the memory requested by all registered plugins.
 *
 * Note the sequence of function calls used to run-up and run-down the RenderWare
 * engine is as follows: \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to
 * run-up and \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * A NULL return value from any of the registered plugin object constructors 
 * will cause this function to fail.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineStart(void)
{
    // TODO: RwEngineStart

    return FALSE;
}

/**
 * \ingroup rwengine
 * \ref RwEngineClose is used to close the RenderWare engine by releasing
 * the currently selected rendering subsystem (graphics device). This
 * function must only be called after the rendering engine has been stopped.
 *
 * Note the sequence of function calls used to run-up and run-down the RenderWare
 * engine is as follows: \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to
 * run-up and \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineClose(void)
{
    // TODO: RwEngineClose

    return FALSE;
}

/**
 * \ingroup rwengine
 * \ref RwEngineOpen is used to initialize the default rendering subsystem
 * (graphics device) and video mode. The device is opened on the specified
 * display. Once the device has been opened use the API functions
 * \ref RwEngineSetSubSystem and \ref RwEngineSetVideoMode to set the display
 * device, as appropriate.
 *
 * This function must only be called after the rendering engine has been initialized.
 *
 * Note the sequence of function calls used to run-up and run-down the RenderWare
 * engine is as follows: \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to
 * run-up and \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \param initParams  Pointer to \ref RwEngineOpenParams value describing a
 *       device-dependent parameter (see example below).
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwEngineInit
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 *
 * \verbatim
   To open the RenderWare engine from a Windows application. The variable
   hwnd is a Windows device handle returned from CreateWindow (type HWND).
   Assume the engine has been initialized: 
  
   RwEngineOpenParams openParams;
  
   openParams.displayID = hwnd;
   if( !RwEngineOpen(&openParams) )
   {
       RwEngineTerm();
       fprintf(stderr, "RwEngineOpen failed\n");
       ...
  }
  \endverbatim
 *
 */
RwBool
RwEngineOpen(RwEngineOpenParams *initParams)
{
    // TODO: RwEngineOpen

    return FALSE;
}

/**
 * \ingroup rwengine
 * \ref RwEngineTerm is used to terminate the RenderWare engine.
 * Before termination, the engine must first be stopped and then
 * closed (in that order).
 * This function should be called before the application closes down.
 *
 * Note the sequence of function calls used to run-up and run-down
 * the RenderWare engine is as follows:
 * \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to run-up and
 * \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 *
 */
RwBool
RwEngineTerm(void)
{
    RwBool result;

#if (0)
    RWAPIFUNCTION(RWSTRING("RwEngineTerm"));
#endif /* (0) */

    /* Can't shut down whilst there are instances opened */

    result = (0 == engineInstancesOpened);

    if (result)
    {
        _rwPluginRegistryClose();
        _rwFileSystemClose();
        _rwMemoryClose();
#ifdef RWDEBUG
        _rwDebugClose();
#endif /* RWDEBUG */

        /* The processor fast divide mode stuff is done in
         * RwCameraBeginUpdate and RwCameraEndUpdate.
         */

        /* Mark the engine as terminated */
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSIDLE;

        RWCRTCHECKMEMORY();

        RWCRTHEAPDIFFERENCESINCE(&_InitialMemState);

    }

    return (result);
}








/**
 * \ingroup rwengine
 * \ref RwEngineInit initializes the core of the RenderWare
 * Graphics rendering engine.
 *
 * This function must be called before any other RenderWare function except
 * functions listed in \ref rwfreelistoverview.
 *
 * This function may also be used to specify an alternative interface that
 * RenderWare Graphics will use for memory management. If specified, RenderWare
 * Graphics will use the supplied memory allocation routines rather than the
 * default standard library ones. Specifying NULL for this parameter forces the
 * default ones to be used.
 *
 * \if gcn
 * On GameCube specifying NULL will cause RwEngineInit to fail as
 * there are no default memory allocation routines.
 * \endif
 *
 * Similarly, a default file system interface is initialized by
 * \ref RwEngineInit.
 *
 * Note the sequence of function calls used to run-up and run-down the
 * RenderWare Graphics engine is as follows: \ref RwEngineInit,
 * \ref RwEngineOpen, \ref RwEngineStart to run-up and \ref RwEngineStop,
 * \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * Note that the purpose of FreeLists is to improve memory management
 * performance. It is recommended that developers benchmark their applications
 * with and without the use of FreeLists. FreeLists should be used if
 * possible.
 *
 * Note too that ultimately RenderWare Graphics FreeLists make use of the
 * memory functions supplied as the first parameter. Most customers will
 * want to use FreeLists as they increase performance.
 *
 * Running RenderWare Graphics without Freelists is allowed purely to help
 * developers observe memory usage since calls to RwMalloc are easily hooked.
 *
 * \param memFuncs  Pointer to an \ref RwMemoryFunctions value specifying the
 * memory management interface (specify NULL to use the default interface).
 *
 * \param openFlags  Additional parameters for the Initialization of
 * RenderWare Graphics. Currently the only flag is whether to use Freelists or
 * not. A value of rwENGINEINITNOFREELISTS can be used to tell RenderWare
 * Graphics to use \ref RwMalloc and \ref RwFree in place of
 * \ref RwFreeListAlloc and
 * \ref RwFreeListFree.
 *
 * \param resArenaSize    An \ref RwUInt32 specifying the size of the memory
 * block (in bytes) reserved for the resources instancing arena. Can be set to
 * zero if no instance data will be used
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 * \see RwOsGetMemoryInterface
 *
 */
RwBool
RwEngineInit(RwMemoryFunctions *memFuncs,
             RwUInt32 openFlags,
             RwUInt32 resArenaSize)
{
    RwBool  result = FALSE;

#ifndef RWGLOBALSIZE
    /* Bootstrap the library by giving it some static globals */
    RwEngineInstance = &staticGlobals;
#endif /* RWGLOBALSIZE */

    if (openFlags & rwENGINEINITNOFREELISTS)
    {
        RWSRCGLOBAL(memoryAlloc) = MallocWrapper;
        RWSRCGLOBAL(memoryFree)  = FreeWrapper;
    }
    else
    {
        RWSRCGLOBAL(memoryAlloc) = _rwFreeListAllocReal;
        RWSRCGLOBAL(memoryFree)  = _rwFreeListFreeReal;
    }

    RWSRCGLOBAL(resArenaInitSize) = resArenaSize;

    /* We mustn't have initted the engine previously */
    /* We can't use RWASSERTs yet - the library isn't initted */
    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSIDLE)
    {
        /*
         * The first thing we need is the string operations,
         * so that the debug stuff stands a chance
         */
        result = _rwStringOpen();

        if (result)
        {
            /* The first thing that must be done is to initialise the
             * debug reporting mechanism
             */
#ifdef RWDEBUG
            result = _rwDebugOpen();
            if (result)
#endif /* RWDEBUG */
            {
                result = _rwMemoryOpen(memFuncs);
                if (result)
                {
                    result = _rwFileSystemOpen();
                    if (result)
                    {
                        result = _rwPluginRegistryOpen();
                        if (result)
                        {
                            result = CorePluginAttach();
                            if (result)
                            {
                                /* RWREGSETDEBUGTRACE(_T("RWDEBUGTRACE")); */
                                {
                                    RWAPIFUNCTION(RWSTRING("RwEngineInit"));

                                    /* We've succeeded */
                                    RWSRCGLOBAL(engineStatus) =
                                        rwENGINESTATUSINITED;

                                    RWRETURN(result);
                                }
                            }
                            _rwPluginRegistryClose();
                        }
                        _rwFileSystemClose();
                    }
                    _rwMemoryClose();
                }
#ifdef RWDEBUG
                _rwDebugClose();
#endif /* RWDEBUG */
            }
            _rwStringClose();
        }
    }

    /* We don't have debug macro capability yet */
    return result;
}

