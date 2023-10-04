/***************************************************************************
 * Module  : bacamera.h                                                    *
 *                                                                         *
 * Purpose : Camera functions                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWCAMERA_H
#define RWCAMERA_H




typedef struct rwCameraGlobals rwCameraGlobals;
struct rwCameraGlobals
{
    RwFreeList         *cameraFreeList;
};


struct RwCamera
{
    char _pad[392];

    /* TODO: RwCamera (size: 392 bytes) */
};

/**
 * \ingroup datatypes
 * \typedef RwCamera
 * Camera object for rendering a view. 
 * This should be considered an opaque type.
 * Use the RwCamera API functions to access.
 */
typedef struct RwCamera RWALIGN(RwCamera, rwCAMERAALIGNMENT);



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


/* Open closing */
extern void        *_rwCameraClose(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwCameraOpen(void *instance, RwInt32 offset,
                                  RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#endif                          /* RWCAMERA_H */
