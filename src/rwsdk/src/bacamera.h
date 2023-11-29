/***************************************************************************
 * Module  : bacamera.h                                                    *
 *                                                                         *
 * Purpose : Camera functions                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWCAMERA_H
#define RWCAMERA_H

/****************************************************************************
 Includes
 */

#include "baraster.h"


typedef struct rwCameraGlobals rwCameraGlobals;
struct rwCameraGlobals
{
    RwFreeList         *cameraFreeList;
};

/* RWPUBLIC */

/**
 * \ingroup datatypes
 * \typedef RwCamera
 * Camera object for rendering a view. 
 * This should be considered an opaque type.
 * Use the RwCamera API functions to access.
 */
typedef struct RwCamera RWALIGN(RwCamera, rwCAMERAALIGNMENT);

struct RwCamera
{
    char _gap0[0x60];

    /* The cameras image buffer */
    RwRaster           *frameBuffer;

    char _pad1[292];

    /* TODO: RwCamera (size: 392 bytes) */
};

/* RWPUBLICEND */



/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Displaying results */
extern RwCamera    *RwCameraShowRaster(RwCamera * camera, void *pDev,
                                       RwUInt32 flags);

/* RWPUBLICEND */

/* Open closing */
extern void        *_rwCameraClose(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwCameraOpen(void *instance, RwInt32 offset,
                                  RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */
/* RWPUBLICEND */

#endif                          /* RWCAMERA_H */
