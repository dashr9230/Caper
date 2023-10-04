/*
 *
 * Image handling.
 * Images are the device independent representation of rasters.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwimage
 * \page rwimageoverview RwImage Overview
 *
 * This object represents a platform-neutral bitmap.
 *
 * Images are intended for loading and processing of bitmap data. 
 * They cannot be rendered directly by RenderWare Graphics and must be converted 
 * into raster (RwRaster) objects prior to rendering. 

 * The image object also supports file handling extensions that are completely 
 * separate from the RenderWare Binary Stream (see RwStream) system. This allows it to read 
 * and write known bitmap file formats, such as .BMP, .RAS and .PNG. By default, 
 * no such support is provided. You must link and register the appropriate Toolkits 
 * to support these extensions, or write your own.
 *
*/


/*****************************************************************************************
 * NOTE: NOTHING IN THIS FILE SHOULD DEPEND ON RWRASTER - THAT'S WHAT BAIMRAS.C/H IS FOR *
 *****************************************************************************************/

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"

#include "batkreg.h"

 /* Math abstraction API */
#include "bamath.h"

/* Image handling */
#include "baimage.h"


#define rwDEFAULTGAMMA (RwReal)((1))

#define RWIMAGEGLOBAL(var)                              \
   (RWPLUGINOFFSET(rwImageGlobals,                      \
                   RwEngineInstance,                    \
                   imageModule.globalsOffset)->var)



/****************************************************************************
 Local (static) Globals
 */

static  RwPluginRegistry   imageTKList = 
{
    sizeof(RwImage),
    sizeof(RwImage),
    0,
    0,
    (RwPluginRegEntry *)NULL,
    (RwPluginRegEntry *)NULL 
};

static RwModuleInfo imageModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Image handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwImageOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwImageOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwImageOpen"));

    /* Store global data offset (same for all instances) */
    imageModule.globalsOffset = offset;

    RWIMAGEGLOBAL(imageFreeList) =
        RwFreeListCreate(imageTKList.sizeOfStruct, 20, 0);
    if (!RWIMAGEGLOBAL(imageFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    RWIMAGEGLOBAL(imageFormatFreeList) =
        RwFreeListCreate(sizeof(rwImageFormat), 3, 0);
    if (!RWIMAGEGLOBAL(imageFormatFreeList))
    {
        /* Failure */
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
        RWRETURN(NULL);
    }

    /* Set up a default imagePath buffer */
    RWIMAGEGLOBAL(imagePathSize) = 256;
    RWIMAGEGLOBAL(imagePath) = (RwChar *)
        RwMalloc(sizeof(RwChar) * RWIMAGEGLOBAL(imagePathSize));
    if (!RWIMAGEGLOBAL(imagePath))
    {
        /* Failure */
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;

        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;

        RWRETURN(NULL);
    }

    RWIMAGEGLOBAL(imagePath)[0] = '\0';

    /* One more module instance */
    imageModule.numInstances++;

    /* Set up the gamma correction */
    RwImageSetGamma(rwDEFAULTGAMMA);

    /* No image formats to start with */
    RWIMAGEGLOBAL(imageFormats) = (rwImageFormat *)NULL;

    RWIMAGEGLOBAL(scratchMemSize) = 256;
    RWIMAGEGLOBAL(scratchMem) = (RwUInt8 *)
        RwMalloc(sizeof(RwUInt8) * RWIMAGEGLOBAL(scratchMemSize));
    if (!RWIMAGEGLOBAL(scratchMem))
    {
        /* Failure */
        RwFree(RWIMAGEGLOBAL(imagePath));
        RWIMAGEGLOBAL(imagePath) = (char *)NULL;
        RWIMAGEGLOBAL(imagePathSize) = 0;

        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;

        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;

        RWRETURN(NULL);
    }

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwImageClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwImageClose(void *instance,
              RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwImageClose"));

    if (RWIMAGEGLOBAL(scratchMem))
    {
        RwFree(RWIMAGEGLOBAL(scratchMem));
        RWIMAGEGLOBAL(scratchMem) = (unsigned char *)NULL;
        RWIMAGEGLOBAL(scratchMemSize) = 0;
    }

    if (RWIMAGEGLOBAL(imagePath))
    {
        RwFree(RWIMAGEGLOBAL(imagePath));
        RWIMAGEGLOBAL(imagePath) = (char *)NULL;
        RWIMAGEGLOBAL(imagePathSize) = 0;
    }

    /* Destroy registered formats */
    while (RWIMAGEGLOBAL(imageFormats))
    {
        rwImageFormat      *formatToDestroy =
            RWIMAGEGLOBAL(imageFormats);

        /* Remove from the list */
        RWIMAGEGLOBAL(imageFormats) = formatToDestroy->nextFormat;

        /* Free it up */
        RwFreeListFree(RWIMAGEGLOBAL(imageFormatFreeList),
                       formatToDestroy);
    }

    if (RWIMAGEGLOBAL(imageFormatFreeList))
    {
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFormatFreeList));
        RWIMAGEGLOBAL(imageFormatFreeList) = (RwFreeList *)NULL;
    }

    if (RWIMAGEGLOBAL(imageFreeList))
    {
        RwFreeListDestroy(RWIMAGEGLOBAL(imageFreeList));
        RWIMAGEGLOBAL(imageFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    imageModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}










/**
 * \ingroup rwimage
 * \ref RwImageSetGamma is used to specify the current
 * gamma correction value. Gamma
 * values greater than 1.0 will lighten an image while values less than 1.0
 * will darken an image. Typically, used to adjust an image prior to display
 * on a monitor whose gamma value differs from unity.
 *
 * The default gamma value is (RwReal)((1.0)).
 *
 * \param gammaValue  A RwReal value equal to the gamma value to be set.
 *
 * \return Returns TRUE.
 *
 * \see RwImageGetGamma
 * \see RwImageGammaCorrect
 *
 */
/* 
 * TODO: This function practicly fully reversed, but the code
 * for initializing both gammaTable and invGammaTable is kinda slow and rough
 */
RwBool
RwImageSetGamma(RwReal gammaValue)
{
    RwReal              nGammaExp, nGammaInv;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RwImageSetGamma"));
    RWASSERT(imageModule.numInstances);

    RWIMAGEGLOBAL(gammaVal) = gammaValue;

    /* calc gamma tables */
    nGammaExp = ((gammaValue));
    nGammaInv = (1.0f) / nGammaExp;

    RWIMAGEGLOBAL(gammaTable[0]) = 0;
    RWIMAGEGLOBAL(invGammaTable[0]) = 0;
    for (i = 1; i < 256; i++)
    {
        /* We rely on theRwReal being promoted to a double
         * in the call - this keeps the equator compiler happy
         */
        RwReal              nT;
        RwInt32             quantize;

        nT = ((float) i) / 255.0f;
        quantize = (RwInt32) (RwPow(nT, nGammaInv) * 255.0f + 0.5f);
        RWIMAGEGLOBAL(gammaTable[i]) = (RwUInt8) quantize;

        quantize = (RwInt32) (RwPow(nT, nGammaExp) * 255.0f + 0.5f);
        RWIMAGEGLOBAL(invGammaTable[i]) = (RwUInt8) quantize;
    }

    RWRETURN(TRUE);
}
