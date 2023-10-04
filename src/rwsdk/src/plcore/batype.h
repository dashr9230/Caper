/***************************************************************************
 *                                                                         *
 * Module  : batype.h                                                      *
 *                                                                         *
 * Purpose : Generic type handling                                         *
 *                                                                         *
 **************************************************************************/

#ifndef RWTYPE_H
#define RWTYPE_H

#define MAKECHUNKID(vendorID, chunkID) (((vendorID & 0xFFFFFF) << 8) | (chunkID & 0xFF))

enum RwPluginVendor
{
    rwVENDORID_CRITERIONINT     = 0x000004L,
};
typedef enum RwPluginVendor RwPluginVendor;


/***
 *** These are the Criterion internal plugin extensions.  Use with rwVENDORID_CRITERIONINT.
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***/

enum RwCriterionPluginID
{
    rwID_COREPLUGIN = 0x01,
};
typedef enum RwCriterionPluginID RwCriterionPluginID;

enum RwCriterionCoreID
{
    /* The valid plugin IDs */
    rwID_VECTORMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x01),
    rwID_MATRIXMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x02),
    rwID_FRAMEMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x03),
    rwID_STREAMMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x04),
    rwID_CAMERAMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x05),
    rwID_IMAGEMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x06),
    rwID_RASTERMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x07),
    rwID_TEXTUREMODULE          = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x08),


    rwID_COLORMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0D),

    rwID_ERRORMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0F),
};
typedef enum RwCriterionCoreID RwCriterionCoreID;


typedef struct RwObject RwObject;
/* CAPER: RwObject size is unknown, only we know is type at offset 0
    RwObject is 8 bytes according to RwTexDictionary */
struct RwObject
{
    RwUInt8 type;                /**< Internal Use */
    RwUInt8 subType;             /**< Internal Use */
    RwUInt8 flags;               /**< Internal Use */
    RwUInt8 privateFlags;        /**< Internal Use */
    void   *parent;              /**< Internal Use */
                                 /* Often a Frame  */
};




/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */



#define rwObjectInitialize(o, t, s)                     \
MACRO_START                                             \
{                                                       \
    ((RwObject *)(o))->type = (RwUInt8)(t);             \
    ((RwObject *)(o))->subType = (RwUInt8)(s);          \
    ((RwObject *)(o))->flags = 0;                       \
    ((RwObject *)(o))->privateFlags = 0;                \
    ((RwObject *)(o))->parent = NULL;                   \
}                                                       \
MACRO_STOP


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#endif /* RWTYPE_H */

