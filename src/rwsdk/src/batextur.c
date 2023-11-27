/*
 * Texture handling.
 * Textures are special cases of rasters which can be applied to polygons
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwtexdict
 * \page rwtexdictoverview RwTexDictionary Overview
 *
 * This object represents a Texture Dictionary: a database of Textures (\ref RwTexture) 
 * and their associated Rasters (\ref RwRaster).
 *
 * When RenderWare Graphics is asked to load a Texture, it first checks the active Texture Dictionary 
 * to see if it is already loaded, thus Texture Dictionaries are simple cacheing mechanism.
 * 
 * Developers should note that, when written to a RenderWare Binary Stream, Texture Dictionaries 
 * also store the associated Rasters. Rasters are, by definition, platform-dependent, so Texture 
 * Dictionaries cannot normally be read on platforms other than those on which they were generated.
 *
 *
*/

/**
 * \ingroup rwtexture
 * \page rwtextureoverview RwTexture Overview
 *
 * Represents a standard RenderWare Texture map.
 *
 * Textures reference Rasters (\ref RwRaster), which contain the actual texture and, if applicable, 
 * any MIP-map data. The Texture object is therefore an extension of the basic Raster 
 * object and some functionality is split across the two APIs because of this. 
 * For instance, MIP-map functions are found under \ref RwRaster, not RwTexture.
 *
 * Textures rely on the \ref RwImage object to load bitmaps from standard image file formats 
 * such as .BMP, .RAS and .PNG.
 *
 *
*/


/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"

#include "batkreg.h"



/* Texture handling */
#include "batextur.h"

#include <windows.h> /* TODO: remove windows.h here */



/****************************************************************************
 Local Defines
 */

#define RWTEXTUREGLOBAL(var)                                    \
      (RWPLUGINOFFSET(rwTextureGlobals,                         \
                      RwEngineInstance,                         \
                      textureModule.globalsOffset)->var)




#define RWSTRNSTORE(_dst, _src)                               \
MACRO_START                                                   \
{                                                             \
    const size_t n = sizeof((_dst))/sizeof((_dst)[0]);        \
                                                              \
    rwstrncpy((_dst), (_src), n);                               \
                                                              \
    if ( n <= rwstrlen((_src)))                               \
    {                                                         \
        const size_t trunc = n-1;                             \
                                                              \
        RWERROR((E_RW_STRING_TRUNCATION,                      \
                (_src), n, trunc, (_src)[trunc]));            \
        (_dst)[trunc] = ((RwChar)0);                          \
    }                                                         \
}                                                             \
MACRO_STOP

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    textureTKList =
    { sizeof(RwTexture),
      sizeof(RwTexture),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

RwPluginRegistry    texDictTKList =
    { sizeof(RwTexDictionary),
      sizeof(RwTexDictionary),
      0,
      0,
      (RwPluginRegEntry *)NULL, 
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo textureModule;

static RwTexDictionary *dummyTexDict = /* dummy texture dictionary */
    (RwTexDictionary *)NULL;  





/****************************************************************************/
static RwTexture   *
TextureDefaultMipmapRead(const RwChar * name, const RwChar * maskName)
{
    RwChar              mipName[256];

    RWFUNCTION(RWSTRING("TextureDefaultMipmapRead"));
    RWASSERT(name);

    RWSTRNSTORE(mipName, name);
}

/****************************************************************************
TextureDefaultRead

 On entry   : Name
            : Masks name
 On exit    :
 */

/* this function simply chooses which method
 * is appropriate for reading the texture */
static RwTexture   *
TextureDefaultRead(const RwChar * name, const RwChar * maskName)
{
    RwTexture          *texture = (RwTexture *)NULL;

    RWFUNCTION(RWSTRING("TextureDefaultRead"));

    if (RWTEXTUREGLOBAL(haveTexMipmaps))
    {

    }
    else
    {

    }

    RWRETURN(texture);
}

















/****************************************************************************
 TextureAnnihilate

 On entry   : texture, user data pointer
 On exit    : texture pointer on success
 */

static              RwBool
TextureAnnihilate(RwTexture * texture)
{
    RWFUNCTION(RWSTRING("TextureAnnihilate"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(texture->refCount <= 0);

    /* Temporarily bump up reference count to avoid assertion failures */
    texture->refCount++;

    RWASSERT(texture->refCount > 0);

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&textureTKList, texture);

    if (texture->dict)
    {
        rwLinkListRemoveLLLink(&texture->lInDictionary);
    }

    if (texture->raster)
    {
        /* We still have the raster to destroy */
        RwRasterDestroy(texture->raster);
        texture->raster = (RwRaster *)NULL;
    }

    /* Reinstate reference count */
    --texture->refCount;
    RWASSERT(texture->refCount <= 0);

    RwFreeListFree(RWTEXTUREGLOBAL(textureFreeList), texture);

    RWRETURN(TRUE);
}







/**
 * \ingroup rwtexture
 * \ref RwTextureSetReadCallBack is used to override the function that
 * will be used to read textures from image files.
 *
 * The default texture loading mechanism can be used to read Sun Raster (*.ras) 
 * or Windows Bitmaps (*.bmp) only.  All other formats must be read using a user 
 * defined procedure. The format of the callback function is:
 *
 * RwTexture * (*RwTextureCallBackRead) (const RwChar *name, const RwChar *maskName)
 *
 * \param fpCallBack  Pointer to the texture reading function.
 *
 * \return Returns TRUE.
 *
 * \see RwTextureGetReadCallBack
 * \see RwTextureRead
 * \see RwImageRead
 *
 */
RwBool
RwTextureSetReadCallBack(RwTextureCallBackRead fpCallBack)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetReadCallBack"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(fpCallBack);

    RWTEXTUREGLOBAL(textureReadFunc) = fpCallBack;
    RWRETURN(TRUE);
}







/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Mipmapping of textures

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureSetMipmapping is used to indicate whether mipmaps will
 * be available whenever a texture is read from an image file. Use this
 * function to turn-on and turn-off creation of textures with mipmaps.
 *
 * The default setting is FALSE, i.e. textures will be created without
 * mipmaps.
 *
 * \param enable  A RwBool value equal to TRUE if mipmaps are required or FALSE if they
 * are not.
 *
 * \return Returns TRUE.
 *
 * \see RwTextureGetMipmapping
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureSetMipmapping(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWTEXTUREGLOBAL(haveTexMipmaps) = enable;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetMipmapping is used to enquire whether mipmaps will
 * be available when reading textures.
 *
 * The default setting is FALSE, i.e. mipmaps are not available.
 *
 * \return Returns TRUE if mipmaps are available or FALSE if they are not.
 *
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGetMipmapping(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(haveTexMipmaps));
}

/**
 * \ingroup rwtexture
 * \ref RwTextureSetAutoMipmapping is used to indicate whether mipmaps
 * (if enabled) should have their contents generated automatically whenever a
 * texture is read from a file, or whether the mipmaps should be read from separate
 * files. If reading from separate files the default naming convention is to postfix
 * the texture and mask name with "mn" where n is the index of the mipmap level, i.e.
 * m0, m1, m2 etc. \ref RwTextureSetMipmapNameCallBack can be called to change the
 * way the mipmap names are generated.
 * Use this function to choose between automatically generating mipmaps (TRUE)
 * or reading mipmaps (FALSE).
 *
 * The default setting is FALSE, i.e. the mipmaps will be read.
 *
 * This setting is only used if mipmaps are available.
 *
 * \param enable  A RwBool value equal to TRUE if mipmaps are to be generated or FALSE
 * if they are to be read from separate files.
 *
 * \return Returns TRUE .
 *
 * \see RwTextureGetAutoMipmapping
 * \see RwTextureSetMipmapping
 * \see RwTextureGetMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureSetAutoMipmapping(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwTextureSetAutoMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWTEXTUREGLOBAL(generateTexMipmaps) = enable;
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexture
 * \ref RwTextureGetAutoMipmapping is used to enquire whether mipmaps are
 * automatically generated when reading a texture or also read in from
 * separate files.
 *
 * The default setting is FALSE, i.e. the mipmaps will be read in also.
 *
 * This setting is only used if mipmaps are available.
 *
 * \return Returns TRUE if mipmaps are automatically generated and FALSE if they
 * are to be read in from separate files.
 *
 * \see RwTextureSetAutoMipmapping
 * \see RwTextureSetMipmapping
 * \see RwTextureGetMipmapping
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGetAutoMipmapping(void)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGetAutoMipmapping"));
    RWASSERT(textureModule.numInstances);

    RWRETURN(RWTEXTUREGLOBAL(generateTexMipmaps));
}




















/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Texture dictionaries

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryCreate is used to create a new texture dictionary
 * for storing textures.
 *
 * \param   None.
 *
 * \return Returns pointer to newly created texture dictionary if successful
 * or NULL if there is an error.
 *
 * \see RwTexDictionaryDestroy
 * \see RwTexDictionaryAddTexture
 * \see RwTexDictionaryRemoveTexture
 *
 */
RwTexDictionary *
RwTexDictionaryCreate(void)
{
    RwTexDictionary    *dict;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryCreate"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(RWTEXTUREGLOBAL(texDictFreeList));

    dict =
        (RwTexDictionary *)
        RwFreeListAlloc(RWTEXTUREGLOBAL(texDictFreeList));
    if (!dict)
    {
        RWRETURN((RwTexDictionary *)NULL);
    }

    rwObjectInitialize(dict, rwTEXDICTIONARY, 0);

    rwLinkListAddLLLink(&RWTEXTUREGLOBAL(texDictList),
                        &dict->lInInstance);
    rwLinkListInitialize(&dict->texturesInDict);

    /* Initialize the plugin memory */
    _rwPluginRegistryInitObject(&texDictTKList, dict);

    RWRETURN(dict);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryDestroy is used to destroy the specified texture
 * dictionary. When the dictionary is destroyed, \ref RwTextureDestroy is
 * called for all of the textures in that dictionary. If the dictionary
 * is the current one, the current dictionary is set to NULL.
 *
 * \param dict  Pointer to the texture dictionary to destroy.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwTexDictionaryCreate
 * \see RwTexDictionaryRemoveTexture
 * \see RwTextureDestroy
 *
 */
RwBool
RwTexDictionaryDestroy(RwTexDictionary *dict)
{
    RWAPIFUNCTION(RWSTRING("RwTexDictionaryDestroy"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);

    int x = offsetof(rwTextureGlobals, currentTexDict);
    /* TODO: RwTexDictionaryDestroy */

    if (RWTEXTUREGLOBAL(currentTexDict) == dict)
    {
        /* It was the current, so make it not */
        RWTEXTUREGLOBAL(currentTexDict) = (RwTexDictionary *)NULL;
    }

    /* Destroy all the textures */
    RwTexDictionaryForAllTextures(dict, 
                                  (RwTextureCallBack)RwTextureDestroy, 
                                  NULL);

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&texDictTKList, dict);

    /* Remove from the free list */
    rwLinkListRemoveLLLink(&dict->lInInstance);

    /* No longer need this entry */
    RwFreeListFree(RWTEXTUREGLOBAL(texDictFreeList), dict);

    /* Success */
    RWRETURN(TRUE);
}

/**
 * \ingroup rwtexdict
 * \ref RwTexDictionaryForAllTextures is used to execute the given
 * callback function on all textures in the specified texture dictionary.
 *
 * The format of the callback function is:
 *
 * RwTexture * (*RwTextureCallBack) (RwTexture *texture, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure status 
 * the iteration is terminated.  However, \ref RwTexDictionaryForAllTextures
 * will still return successfully.
 *
 * \param dict  Pointer to the texture dictionary.
 * \param fpCallBack  Pointer to the function to call with each texture in the dictionary.
 * \param pData  User data pointer to pass to callback function.
 *
 * \return Returns pointer to the texture dictionary.
 *
 * \see RwTexDictionaryForAllTexDictionaries
 * \see RwTexDictionaryFindNamedTexture
 */
const RwTexDictionary *
RwTexDictionaryForAllTextures(const RwTexDictionary* dict,
                              RwTextureCallBack fpCallBack, void *pData)
{
    RwLLLink           *cur, *next;
    const RwLLLink     *end;

    RWAPIFUNCTION(RWSTRING("RwTexDictionaryForAllTextures"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(dict);
    RWASSERTISTYPE(dict, rwTEXDICTIONARY);
    RWASSERT(fpCallBack);

    end = rwLinkListGetTerminator(&dict->texturesInDict);
    cur = rwLinkListGetFirstLLLink(&dict->texturesInDict);

    while (cur != end)
    {
        RwTexture          *texture;

        next = rwLLLinkGetNext(cur);

        texture = rwLLLinkGetData(cur, RwTexture, lInDictionary);
        RWASSERT(texture);
        RWASSERT(0 < texture->refCount);



    }

    /* TODO: RwTexDictionaryForAllTextures */

    RWRETURN(NULL);
}











/**
 * \ingroup rwtexture
 * \ref RwTextureDestroy is used to destroy the specified texture. If
 * the texture is defined in a texture dictionary the texture is removed from
 * that dictionary. The raster associated with the texture is also destroyed
 * by this function.
 *
 * Note that only textures whose reference count is zero are actually removed
 * from the system. Reference counts greater than one indicate that this
 * particular texture is still being used by other objects. Hence, this
 * function will only decrement the texture's reference count if this is
 * the case.
 *
 * \param texture  Pointer to the texture to destroy.
 *
 * \return Returns TRUE if the texture has been destroyed or FALSE if
 * there is an error.
 *
 * \see RwTextureCreate
 * \see RwTextureRead
 * \see RwTextureAddRef
 *
 */
RwBool
RwTextureDestroy(RwTexture *texture)
{
    RwBool              result = TRUE;

    RWAPIFUNCTION(RWSTRING("RwTextureDestroy"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(texture);
    RWASSERT(0 < texture->refCount);

    --texture->refCount;

    if (texture->refCount <= 0)
        result = TextureAnnihilate(texture);

    /* All done */
    RWRETURN(result);
}















/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Reading textures

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwtexture
 * \ref RwTextureGenerateMipmapName is used to generate the name of a
 * mipmap level file.
 *
 * It is possible to supply a user defined procedure for naming mipmap
 * levels. The API function \ref RwTextureSetMipmapNameCallBack should
 * be used before reading of textures.
 *
 * \param name  Pointer to a string containing the root name of the texture. This
 * must have at least rwTEXTUREMIPMAPNAMECHARS free storage of size
 * RwChar free after the name itself. The mipmap level name is put here.
 * \param maskName  Pointer to a string containing the root mask name of the texture or
 * NULL if no mask name is required. This must have at least
 * rwTEXTUREMIPMAPNAMECHARS free storage of size RwChar free after the name
 * itself. The mipmap level mask name is put here.
 * \param mipLevel  A value equal to the mipmap level for which the name is required.
 * \param format  A value describing the mipmapping mode. A combination of the bit
 * flags rwRASTERFORMATMIPMAP and rwRASTERFORMATAUTOMIPMAP.
 *
 * \return Returns TRUE if the operation was successful and FALSE if there was
 * a failure. If the return is FALSE the names are not altered.
 *
 * \see RwTextureSetMipmapNameCallBack
 * \see RwTextureGetMipmapNameCallBack
 * \see RwTextureRead
 *
 */
RwBool
RwTextureGenerateMipmapName(RwChar *name, RwChar *maskName,
                            RwUInt8 mipLevel, RwInt32 format)
{
    RWAPIFUNCTION(RWSTRING("RwTextureGenerateMipmapName"));
    RWASSERT(textureModule.numInstances);
    RWASSERT(name);

}




















/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwTextureClose

 On entry   :
 On exit    :
 */

void *
_rwTextureClose(void *instance,
                RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwTextureClose"));

    /* TODO: _rwTextureClose */

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwTextureOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwTextureOpen(void *instance, RwInt32 offset,
               RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwTextureOpen"));

    /* Save offset of global data (same for all instances) */
    textureModule.globalsOffset = offset;

    RWTEXTUREGLOBAL(textureFreeList) =
        RwFreeListCreate(textureTKList.sizeOfStruct, 20, 0);
    if (!RWTEXTUREGLOBAL(textureFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    RWTEXTUREGLOBAL(texDictFreeList) =
        RwFreeListCreate(texDictTKList.sizeOfStruct, 5, 0);
    if (!RWTEXTUREGLOBAL(texDictFreeList))
    {
        /* Failure */
        RwFreeListDestroy(RWTEXTUREGLOBAL(textureFreeList));
        RWTEXTUREGLOBAL(textureFreeList) = (RwFreeList *)NULL;

        RWRETURN(NULL);
    }

    rwLinkListInitialize(&RWTEXTUREGLOBAL(texDictList));

    /* One more module instance - do this before API calls */
    textureModule.numInstances++;

    /* Create a new dictionary */
    dummyTexDict = RwTexDictionaryCreate();
    RWTEXTUREGLOBAL(currentTexDict) = dummyTexDict;
    if (!RWTEXTUREGLOBAL(currentTexDict))
    {
        RwFreeListDestroy(RWTEXTUREGLOBAL(texDictFreeList));
        RWTEXTUREGLOBAL(texDictFreeList) = (RwFreeList *)NULL;

        RwFreeListDestroy(RWTEXTUREGLOBAL(textureFreeList));
        RWTEXTUREGLOBAL(textureFreeList) = (RwFreeList *)NULL;

        RWRETURN(NULL);
    }

    /* Set default texture reading mode */
    RWTEXTUREGLOBAL(haveTexMipmaps) = FALSE;
    RWTEXTUREGLOBAL(generateTexMipmaps) = FALSE;

    /* Get on with the setup */
    RwTextureSetReadCallBack(TextureDefaultRead);

    /* TODO: _rwTextureOpen */


    // RwGlobals *RwEngineInstance

    /* Success */
    RWRETURN(instance);
}
