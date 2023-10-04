/***************************************************************************
 *                                                                         *
 * Module  : badevice.h                                                    *
 *                                                                         *
 * Purpose : Device layer                                                  *
 *                                                                         *
 **************************************************************************/

#ifndef RWDEVICE_H
#define RWDEVICE_H

/****************************************************************************
 Includes
 */

#include "batkreg.h"


/**
 * \ingroup datatypes
 * RwEngineInitFlag 
 * Engine initialization flags.  An application can use
 * these to control the memory manager that RenderWare uses for dynamic
 * memory management.  By default RenderWare uses FreeLists.  This is the
 * preferred way of using RenderWare.  If the application does not want
 * RenderWare to use the memory manager, then the application can pass
 * rwENGINEINITNOFREELISTS as the argument to \ref RwEngineInit and
 * RenderWare will replace freelist calls to corresponding calls to
 * RwMalloc and RwFree.  This will result in more memory management
 * related calls.
 */
enum RwEngineInitFlag
{
    RwEngineInitFlag_0, /* TODO: Rename RwEngineInitFlag_0 */
    rwENGINEINITNOFREELISTS = 0x1,  /**<Don't use Freelists */
};
typedef enum RwEngineInitFlag RwEngineInitFlag;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */




/* Sequence of events to get RenderWare up and running */
extern RwBool RwEngineInit(RwMemoryFunctions *memFuncs,
                           RwUInt32 initFlags,
                           RwUInt32 resArenaSize);
extern RwInt32 RwEngineRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                  RwPluginObjectConstructor initCB,
                                  RwPluginObjectDestructor termCB);
extern RwInt32 RwEngineGetPluginOffset(RwUInt32 pluginID);



extern RwUInt32 _rwGetNumEngineInstances(void);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* RWDEVICE_H */

