/***************************************************************************
 * Module  : baframe.h                                                     *
 *                                                                         *
 * Purpose : frame handling for engine                                     *
 *                                                                         *
 **************************************************************************/

#ifndef RWFRAME_H
#define RWFRAME_H

/****************************************************************************
 Includes
 */

#include "batkreg.h"



struct RwFrame
{
    char _pad0[164];

    /* TODO: RwFrame (size: 164 bytes) */
};


/**
 * \ingroup datatypes
 * \typedef RwFrame 
   Frame for defining object position and orientation. 
 * This should be considered an opaque type.
 * Use the RwFrame API functions to access.
 */
typedef struct RwFrame RWALIGN(RwFrame, rwFRAMEALIGNMENT);



typedef struct rwFrameGlobals rwFrameGlobals;
struct rwFrameGlobals
{
    RwFreeList         *frames;
};









/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */





/* Opening and closing */
extern void *
_rwFrameClose(void *instance,
              RwInt32 offset,
              RwInt32 size);

extern void *
_rwFrameOpen(void *instance,
             RwInt32 offset,
                RwInt32 size);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* RWFRAME_H */

