/***************************************************************************
 *                                                                         *
 * Module  : balibtyp.h                                                    *
 *                                                                         *
 * Purpose : Macros/types only accessable from the library                 *
 *           This should be included directly after batypes.h in all       *
 *           modules which are part of the library (as opposed to the      *
 *           device.                                                       *
 *                                                                         *
 **************************************************************************/

#ifndef RWLIBTYP_H
#define RWLIBTYP_H

/****************************************************************************
 Includes
 */

#include "badebug.h"


#include "bafsys.h"
#include "bamemory.h"
#include "rwstring.h"


/* RWPUBLIC */

/****************************************************************************
 Defines
 */

/* macro used to access plugin data in objects */
#define RWPLUGINOFFSET(type, base, offset) \
   ((type *)((RwUInt8 *)base + offset))



/* macro used to access global data structure (the root type is RwGlobals) */
#define RWSRCGLOBAL(variable) \
   (((RwGlobals *)RwEngineInstance)->variable)

#define RWASSERTISTYPE(_f, _t) \
   RWASSERT((((const RwObject *)(_f))->type)==(_t))

/****************************************************************************
 Global Types
 */

enum RwEngineStatus
{
    rwENGINESTATUSIDLE = 0,                     /* This needs to be zero */
    rwENGINESTATUSINITED = 1,
};
typedef enum RwEngineStatus RwEngineStatus;

typedef struct RwGlobals RwGlobals;
struct RwGlobals
{
        /* TODO: RwGlobals (size: 340 bytes) */
        char _pad1[188];

        /* All of the frames which have been updated */
        RwLinkList          dirtyFrameList;

        /* The file functions */
        RwFileFunctions     fileFuncs;

        /* The string functions */
        RwStringFunctions   stringFuncs;

        /* The memory allocation functions */
        RwMemoryFunctions   memoryFuncs;

        /* virtual memory alloc/free functions */
        RwMemoryAllocFn         memoryAlloc;
        RwMemoryFreeFn          memoryFree;

        char _pad2[4];

        /* Current engine status */
        RwEngineStatus      engineStatus;

        /* Resource arena init size. */
        RwUInt32            resArenaInitSize;
};

typedef struct RwModuleInfo RwModuleInfo;
struct RwModuleInfo
{
        RwInt32     globalsOffset;
        RwInt32     numInstances;
};



/****************************************************************************
 Program wide globals
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifdef RWGLOBALSIZE

#else /* RWGLOBALSIZE */
extern void         *RwEngineInstance;
#endif /* RWGLOBALSIZE */



#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */


#endif /* RWLIBTYP_H */
