/***************************************************************************
 *                                                                         *
 * Module  : baimage.h                                                     *
 *                                                                         *
 * Purpose : Handling bitmap images                                        *
 *                                                                         *
 **************************************************************************/

#ifndef RWIMAGE_H
#define RWIMAGE_H


typedef struct RwImage RwImage;

struct RwImage
{
    char _pad[28];
    /* TODO: RwImage (size: 28 bytes) */
};


typedef struct rwImageFormat rwImageFormat;
struct rwImageFormat
{
    char _pad0[48];

    rwImageFormat      *nextFormat;

    /* TODO: rwImageFormat (size: 52 bytes) */
};

typedef struct rwImageGlobals rwImageGlobals;
struct rwImageGlobals
{
    RwFreeList         *imageFreeList;
    RwChar             *imagePath;
    RwInt32             imagePathSize;

    RwUInt8             gammaTable[256];
    RwUInt8             invGammaTable[256];
    RwReal              gammaVal;

    RwUInt8            *scratchMem;
    RwInt32             scratchMemSize;

    RwFreeList         *imageFormatFreeList;
    rwImageFormat      *imageFormats;

    /* TODO: rwImageGlobals (size: 544 bytes) */
};






/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


/* Gamma correction */
extern RwBool       RwImageSetGamma(RwReal gammaValue);


/* Opening and closing libraries */
extern void        *_rwImageOpen(void *instance, RwInt32 offset,
                                 RwInt32 size);
extern void        *_rwImageClose(void *instance, RwInt32 offset,
                                  RwInt32 size);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



#endif                          /* RWIMAGE_H */

