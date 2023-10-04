/*                                                                         *
 * Module  : batkreg.h                                                     *
 *                                                                         *
 * Purpose : plugin regsitry                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWTKREG_H
#define RWTKREG_H


/**
 * \ingroup datatypes
 * \ref RwPluginObjectConstructor represents the callback
 * registered by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc.
 * as the function that initializes either the global extension data (in the
 * case of \ref RwEngineRegisterPlugin) or the object extension data (in all
 * other cases). Registered by \ref RwCameraSetStreamAlwaysCallBack, etc.
 * 
 * \param  object   Pointer to the object (global or otherwise)
 * that contains the extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the
 * byte offset of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object
 */
typedef void *(*RwPluginObjectConstructor)(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);

/**
 * \ingroup datatypes
 * \ref RwPluginObjectCopy represents the callback registered by
 * \ref RwCameraRegisterPlugin, etc. as the function that copies the object
 * extension data when an object is duplicated.
 * 
 * \param  dstObject   Pointer to the destination object that will
 * receive the extension data.
 *
 * \param  srcObject   Pointer to the source object containing
 * extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the byte offset
 * of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object
 */
typedef void *(*RwPluginObjectCopy)(void *dstObject, const void *srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject);

/**
 * \ingroup datatypes
 * \ref RwPluginObjectDestructor represents the callback registered
 * by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc. as the
 * function that destroys either the global extension data (in the case of
 * \ref RwEngineRegisterPlugin) or the object extension data (in all other
 * cases).
 * 
 * \param  object   Pointer to the object (global or otherwise)
 * containing the extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the byte
 * offset of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object.
 */
typedef void *(*RwPluginObjectDestructor)(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);



typedef struct RwPluginRegistry RwPluginRegistry;
typedef struct RwPluginRegEntry RwPluginRegEntry;

struct RwPluginRegistry
{
	RwInt32          sizeOfStruct;
	RwInt32          origSizeOfStruct;
	RwInt32          maxSizeOfStruct;

	char _pad8[4];

	RwPluginRegEntry *firstRegEntry;
    RwPluginRegEntry *lastRegEntry;

	/* TODO: RwPluginRegistry (size: ? bytes) */
};

struct RwPluginRegEntry
{
	RwInt32         offset;
	RwInt32         size;
	RwUInt32        pluginID;

	char _pad[20];
	/* TODO: RwPluginRegEntry (size: 60 bytes) */

	RwPluginObjectConstructor constructCB;
	RwPluginObjectDestructor destructCB;
	RwPluginObjectCopy copyCB;

	char _pad1[4];

	RwPluginRegEntry *nextRegEntry;
	RwPluginRegEntry *prevRegEntry;
	RwPluginRegistry *parentRegistry;
};



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern          "C"
{
#endif         /* __cplusplus */

/* Opening/Closing */
extern RwBool
_rwPluginRegistryOpen(void);
extern RwBool
_rwPluginRegistryClose(void);


extern RwInt32
_rwPluginRegistryAddPlugin(RwPluginRegistry * reg,
                           RwInt32 size,
                           RwUInt32 pluginID,
                           RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB,
                           RwPluginObjectCopy copyCB);
extern RwInt32
_rwPluginRegistryGetPluginOffset(const RwPluginRegistry *reg, 
                                RwUInt32 pluginID);

/* Initializing/De-initializing instances */
extern const
RwPluginRegistry *_rwPluginRegistryInitObject(const RwPluginRegistry * reg, 
                                             void *object);
extern const
RwPluginRegistry *_rwPluginRegistryDeInitObject(const RwPluginRegistry * reg, 
                                               void *object);
extern const
RwPluginRegistry *_rwPluginRegistryCopyObject(const RwPluginRegistry * reg,
                                             void *dstObject, 
                                             const void *srcObject);



#ifdef    __cplusplus
}
#endif         /* __cplusplus */

#endif         /* RWTKREG_H */

