/*
 * Types for textures
 * Copyright (c) Criterion Software Limited
 */

/***************************************************************************
 *                                                                         *
 * Module  : batextur.h                                                    *
 *                                                                         *
 * Purpose : Texture map handling                                          *
 *                                                                         *
 **************************************************************************/

#ifndef RWTEXTUR_H
#define RWTEXTUR_H

/****************************************************************************
 Includes
 */

#include "batype.h"

#include "baraster.h"



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




typedef struct rwTextureGlobals rwTextureGlobals;
struct rwTextureGlobals
{
    RwLinkList          texDictList;
    RwFreeList         *textureFreeList;
    RwFreeList         *texDictFreeList;
    RwTexDictionary    *currentTexDict;
    RwTextureCallBackRead textureReadFunc;
    RwBool              haveTexMipmaps;
    RwBool              generateTexMipmaps;

    char _pad[16];

    /* TODO: rwTextureGlobals (size: 48 bytes) */
};





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



/* Opening and closing */
extern void        *_rwTextureOpen(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwTextureClose(void *instance, RwInt32 offset,
                                    RwInt32 size);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */





#endif                          /* RWTEXTUR_H */

