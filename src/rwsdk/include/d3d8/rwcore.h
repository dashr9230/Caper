/******************************************/
/*                                        */
/*    RenderWare(TM) Graphics Library     */
/*                                        */
/******************************************/

/*
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 1999. Criterion Software Ltd.
 * All Rights Reserved.
 */

/*************************************************************************
 *
 * Filename: <D:\Repositories\Caper\src\rwsdk\include\d3d8\rwcore.h>
 * Automatically Generated on: Thu Nov 30 22:51:32 2023
 *
 ************************************************************************/

#ifndef RWCORE_H
#define RWCORE_H

/*--- System Header Files ---*/
#include <rwplcore.h>


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2resort.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2macros.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2heap.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2dep.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2core.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/d3d8/nodeD3D8SubmitNoLight.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2define.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2altmdl.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8texdic.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8rendst.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8raster.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8device.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8convrt.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d82drend.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/common/ssematml.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/common/cpuext.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/baraster.h ---*/

/**
 * \ingroup datatypes
 *  \ref RwRasterFlipMode represents
 *  raster flip modes */
enum RwRasterFlipMode
{
    rwRASTERFLIPWAITVSYNC = 1,  /**<Flip on VSync */
};
typedef enum RwRasterFlipMode RwRasterFlipMode;

/**
 * \ingroup datatypes
 *  RwRasterType 
 *  This type represents the options available for creating a new 
 * raster (se API function \ref RwRasterCreate)*/
enum RwRasterType
{
    rwRASTERTYPENORMAL = 0x00,          /**<Normal */

    rwRASTERDONTALLOCATE = 0x80,        /**<If set the raster is not allocated */
};
typedef enum RwRasterType RwRasterType;


/* How big is my stack!!! */
#define rwRASTERCONTEXTSTACKSIZE 10

/****************************************************************************
 Global Types
 */

/**
 * \ingroup datatypes
 * \typedef RwRaster
 * Raster containing device-dependent pixels.
 * This should be considered an opaque type.
 * Use the RwRaster API functions to access.
 */
typedef struct RwRaster RwRaster;

struct RwRaster
{
    RwRaster           *parent; /* Top level raster if a sub raster */
    RwUInt8            *cpPixels; /* Pixel pointer when locked */
    RwUInt8            *palette; /* Raster palette */
    RwInt32             width, height, depth; /* Dimensions of raster */

    char _pad1[8];

    RwUInt8             cType;  /* Type of raster */
    RwUInt8             cFlags; /* Raster flags */

    char _pad3[18];

    

    /* TODO: RwRaster (size: 52 bytes) */
};



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */



extern RwBool       RwRasterDestroy(RwRaster * raster);


/* Displaying rasters */
extern RwRaster    *RwRasterShowRaster(RwRaster * raster, void *dev,
                                       RwUInt32 flags);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/drvmodel.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/d3d8/pip2model.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2renderstate.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/baimage.h ---*/

typedef struct RwImage RwImage;

struct RwImage
{
    char _pad[28];
    /* TODO: RwImage (size: 28 bytes) */
};

/**
 * \ingroup datatypes
 * \ref RwImageCallBackRead 
 * is the function registered with \ref RwImageRegisterImageFormat that is used,
 * for example by \ref RwImageRead and \ref RwImageReadMaskedImage,
 * to read images of a specific format from a disk file.
 * 
 * \param  imageName   Pointer to a string containing the file name of the image.
 *
 * \see RwImageRegisterImageFormat
 *
 */
typedef RwImage *(*RwImageCallBackRead)(const RwChar * imageName);

/**
 * \ingroup datatypes
 * \ref RwImageCallBackWrite 
 * is the function registered with \ref RwImageRegisterImageFormat that is used,
 * for example by \ref RwImageWrite, 
 * to write images of a specific format to a disk file.
 * 
 * \param  image   Pointer to the image.
 *
 * \param  imageName   Pointer to a string containing the file name
 * of the image.
 *
 *
 * \return Pointer to the written image.
 *
 * \see RwImageRegisterImageFormat
 *
 */
typedef RwImage *(*RwImageCallBackWrite)(RwImage *image, const RwChar *imageName);





/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


    /* Gamma correction */
extern RwBool       RwImageSetGamma(RwReal gammaValue);

extern RwBool       RwImageRegisterImageFormat(const RwChar * extension,
                                               RwImageCallBackRead
                                               imageRead,
                                               RwImageCallBackWrite
                                               imageWrite);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/common/palquant.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/batextur.h ---*/

/* Type ID */
#define rwTEXDICTIONARY 6


typedef struct RwTexDictionary RwTexDictionary;

struct RwTexDictionary
{
    RwObject            object; /* Homogeneous type */
    RwLinkList          texturesInDict; /* List of textures in dictionary */
    RwLLLink            lInInstance; /* Link list of all dicts in system */
};



typedef struct RwTexture RwTexture;

struct RwTexture
{
    RwRaster           *raster; /** pointer to RwRaster with data */
    RwTexDictionary    *dict;   /* Dictionary this texture is in */
    RwLLLink            lInDictionary; /* List of textures in this dictionary */

    char _pad0[68];

    RwInt32             refCount; /* Reference count, surprisingly enough */

    /* TODO: RwTexture (size: 88 bytes) */
};







/**
 * \ingroup datatypes
 * \ref RwTextureCallBack
 * represents the function called from \ref RwTexDictionaryForAllTextures
 * for all textures in a given texture dictionary. This function should
 * return the current texture to indicate success. The callback may return
 * NULL to terminate further callbacks on the texture dictionary.
 * 
 * \param  texture   Pointer to the current texture.
 *
 * \param  pData   User-defined data pointer.
 *
 * \return Pointer to the current texture
 *
 * \see RwTexDictionaryForAllTextures
 */
typedef RwTexture *(*RwTextureCallBack)(RwTexture *texture, void *pData);











/**
 * \ingroup datatypes
 * \ref RwTextureCallBackRead 
 * represents the function used by \ref RwTextureRead to read the specified
 * texture from a disk file. This function should return a pointer to the
 * texture to indicate success.
 * 
 * \param  name   Pointer to a string containing the name of
 * the texture to read.
 *
 * \param  maskName   Pointer to a string containing the name
 * of the mask to read and apply to the texture.
 *
 * \return Pointer to the texture
 *
 * \see RwTextureSetReadCallBack
 * \see RwTextureGetReadCallBack
 */
typedef RwTexture *(*RwTextureCallBackRead)(const RwChar *name,
                                            const RwChar *maskName);




/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


    /* Setting mip mapping states */
extern RwBool       RwTextureSetMipmapping(RwBool enable);
extern RwBool       RwTextureGetMipmapping(void);
extern RwBool       RwTextureSetAutoMipmapping(RwBool enable);
extern RwBool       RwTextureGetAutoMipmapping(void);


extern RwBool       RwTextureSetReadCallBack(RwTextureCallBackRead
                                             fpCallBack);



    /* Creating/destroying dictionaries */
extern RwTexDictionary *RwTexDictionaryCreate(void);

    /* Textures */
extern RwBool       RwTextureDestroy(RwTexture* texture);


    /* Enumerating textures */
extern const RwTexDictionary *RwTexDictionaryForAllTextures(const
                                                            RwTexDictionary
                                                            * dict,
                                                            RwTextureCallBack
                                                            fpCallBack,
                                                            void *pData);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */





/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/p2stdcls.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/baim3d.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/d3d8/im3dpipe.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/d3d8dxttex.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/drvfns.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/baresamp.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/baimras.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/baframe.h ---*/

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


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */



#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/batypehf.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/basync.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/babintex.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/babinfrm.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/babbox.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/bacamera.h ---*/

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

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/bacamval.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/pipe/p2/bapipe.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/babincam.h ---*/

#endif /* RWCORE_H */
