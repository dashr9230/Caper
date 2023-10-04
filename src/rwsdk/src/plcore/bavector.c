/* 
 * 2D / 3D Vector maths.
 * Performs all Vector based maths within library
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/***************************************************************************
 *                                                                         *
 * Module  : bavector.c                                                    *
 *                                                                         *
 * Purpose : Vector handling                                               *
 *                                                                         *
 **************************************************************************/

/**
 * \ingroup rwv3d
 * \page rwv3doverview RwV3d Overview
 *
 * This is RenderWare Graphics' 3D vector object.
 * 
 * The API covers the 3D vector basics, including: addition, subtraction, 
 * cross and dot products, length determination, negation, normalization 
 * and more. 
 * It is primarily used in the context of RenderWare's 3D 
 * Immediate Mode API \ref rwim3d 
 * and the scene graph API \ref rpworld .
 * It is also used to represent a 3D coordinate
 * or point in other parts of the API, such as \ref RwBBox.
 *
*/

/**
 * \ingroup rwv2d
 * \page rwv2doverview RwV2d Overview
 *
 * This is RenderWare's 2D vector object.
 * 
 * RwV2d's API covers most 2D vector needs, including: addition, dot product, 
 * length, assignment, normalization, scaling and subtraction. The object is 
 * primarily used in the context of RenderWare's 2D Immediate Mode API 
 * \ref rwim2d .
 * It is also used to represent a 2D coordinate or point in other parts 
 * of the API.
 *
 */


/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"

#include "bamath.h"

#include "bavector.h"



/****************************************************************************
 Local Defines
 */

#define rwSQRTTABLEPRECISION           11L /* bits of precision */
#define rwSQRTTABLESIZE                (1L << rwSQRTTABLEPRECISION)
#define rwSQRTTABLESHIFT               (23L - rwSQRTTABLEPRECISION)
#define rwSQRTTABLEBYTES               (sizeof(RwUInt32) << (rwSQRTTABLEPRECISION + 1))

#define RWVECTORGLOBAL(var) (RWPLUGINOFFSET(rwVectorGlobals, RwEngineInstance, vectorModule.globalsOffset)->var)





/****************************************************************************
 Local (static) Globals
 */

/* This is the same for all instances */
static RwModuleInfo vectorModule;

/****************************************************************************
 SqrtTableDestroy

 On entry   :
 On exit    :
 */

static void
SqrtTableDestroy(void)
{
    RWFUNCTION(RWSTRING("SqrtTableDestroy"));

    if (NULL != RWVECTORGLOBAL(SqrtTab))
    {
        RwFree(RWVECTORGLOBAL(SqrtTab));
        RWVECTORGLOBAL(SqrtTab) = (RwSplitBits *)NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 InvSqrtTableDestroy

 On entry   :
 On exit    :
 */

static void
InvSqrtTableDestroy(void)
{
    RWFUNCTION(RWSTRING("InvSqrtTableDestroy"));

    if (NULL != RWVECTORGLOBAL(InvSqrtTab))
    {
        RwFree(RWVECTORGLOBAL(InvSqrtTab));
        RWVECTORGLOBAL(InvSqrtTab) = (RwSplitBits *)NULL;
    }

    RWRETURNVOID();
}

/****************************************************************************
 SqrtTableCreate

 On entry   :
 On exit    : FALSE on error
 */

static              RwBool
SqrtTableCreate(void)
{
    RwUInt32            i;
    RwSplitBits        *SqrtTab2to4;
    RwSplitBits        *SqrtTab1to2;
    RwSplitBits         spIn, spOut;

    RWFUNCTION(RWSTRING("SqrtTableCreate"));

    /*
     * X^0.5
     */

    SqrtTab2to4 = (RwSplitBits *) RwMalloc(rwSQRTTABLEBYTES);
    if (NULL == SqrtTab2to4)
    {
        RWERROR((E_RW_NOMEM, rwSQRTTABLEBYTES));
        RWRETURN(FALSE);
    }

    SqrtTab1to2 = &SqrtTab2to4[rwSQRTTABLESIZE];

    spIn.nReal = ((RwReal)1);

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
        spOut.nReal = (RwReal) rwSqrt(spIn.nReal);
        spOut.nUInt = spOut.nUInt + 0x00400000UL - 0x20000000UL;
        SqrtTab1to2[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
        spOut.nReal = (RwReal) rwSqrt(spIn.nReal);
        spOut.nUInt = spOut.nUInt - 0x20000000UL;
        SqrtTab2to4[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    RWVECTORGLOBAL(SqrtTab) = SqrtTab2to4;

    RWRETURN(TRUE);
}

/****************************************************************************
 InvSqrtTableCreate

 On entry   :
 On exit    : FALSE on error
 */

static              RwBool
InvSqrtTableCreate(void)
{
    RwUInt32            i;
    RwSplitBits        *InvSqrtTab1to2;
    RwSplitBits        *InvSqrtTab2to4;
    RwSplitBits         spIn, spOut;

    RWFUNCTION(RWSTRING("InvSqrtTableCreate"));

    /*
     * X^-0.5
     */

    InvSqrtTab2to4 = (RwSplitBits *) RwMalloc(rwSQRTTABLEBYTES);
    if (NULL == InvSqrtTab2to4)
    {
        RWERROR((E_RW_NOMEM, rwSQRTTABLEBYTES));
        RWRETURN(FALSE);
    }

    InvSqrtTab1to2 = &InvSqrtTab2to4[rwSQRTTABLESIZE];

    spIn.nReal = ((RwReal)1);
    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
        spOut.nReal = (RwReal) (((RwReal)1) / rwSqrt(spIn.nReal));
        spOut.nUInt = spOut.nUInt - 0x20000000UL;
        InvSqrtTab1to2[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    for (i = 0; i < rwSQRTTABLESIZE; i++)
    {
        spOut.nReal = (RwReal) (((RwReal)1) / rwSqrt(spIn.nReal));
        spOut.nUInt = spOut.nUInt + 0x00400000UL - 0x20000000UL;
        InvSqrtTab2to4[i].nUInt = spOut.nUInt;
        spIn.nUInt += (1L << rwSQRTTABLESHIFT);
    }

    RWVECTORGLOBAL(InvSqrtTab) = InvSqrtTab2to4;

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening/Closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 VectorMultPoint

 On entry   : out array, in array, num of points, matrix
 On exit    : out array of success
 */

static RwV3d       *
VectorMultPoint(RwV3d * pointsOut,
                   const RwV3d * pointsIn,
                   RwInt32 numPoints, const RwMatrix * matrix)
{
    RwV3d              *cachedOut = pointsOut;

    RWFUNCTION(RWSTRING("VectorMultPoint"));
    RWASSERT(pointsOut);
    RWASSERT(pointsIn);
    RWASSERT(matrix);

    while (--numPoints >= 0)
    {
        RwReal              scale;
        RwReal              imageX, imageY, imageZ;

        scale = pointsIn->x;   /* X */
        imageX = ((scale) * (matrix->right.x));
        imageY = ((scale) * (matrix->right.y));
        imageZ = ((scale) * (matrix->right.z));

        scale = pointsIn->y;   /* Y */
        imageX = ((imageX) + (((scale) * (matrix->up.x))));
        imageY = ((imageY) + (((scale) * (matrix->up.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

        scale = pointsIn->z;   /* Z */
        imageX = ((imageX) + (((scale) * (matrix->at.x))));
        imageY = ((imageY) + (((scale) * (matrix->at.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->at.z))));

        /* W */
        pointsOut->x = ((imageX) + (matrix->pos.x));
        pointsOut->y = ((imageY) + (matrix->pos.y));
        pointsOut->z = ((imageZ) + (matrix->pos.z));

        /* Next point */
        pointsIn++;
        pointsOut++;
    }

    RWRETURN(cachedOut);
}

/****************************************************************************
 VectorMultVector

 On entry   : out array, in array, num of points, matrix
 On exit    : point on success
 */

static RwV3d       *
VectorMultVector(RwV3d * pointsOut,
                    const RwV3d * pointsIn,
                    RwInt32 numPoints, const RwMatrix * matrix)
{
    RwV3d              *cachedOut = pointsOut;

    RWFUNCTION(RWSTRING("VectorMultVector"));
    RWASSERT(pointsOut);
    RWASSERT(pointsIn);
    RWASSERT(matrix);

    while (--numPoints >= 0)
    {
        RwReal              scale;
        RwReal              imageX, imageY, imageZ;

        scale = pointsIn->x;   /* X */
        imageX = ((scale) * (matrix->right.x));
        imageY = ((scale) * (matrix->right.y));
        imageZ = ((scale) * (matrix->right.z));

        scale = pointsIn->y;   /* Y */
        imageX = ((imageX) + (((scale) * (matrix->up.x))));
        imageY = ((imageY) + (((scale) * (matrix->up.y))));
        imageZ = ((imageZ) + (((scale) * (matrix->up.z))));

        scale = pointsIn->z;   /* Z */
        pointsOut->x = ((((scale) * (matrix->at.x))) + (imageX));
        pointsOut->y = ((((scale) * (matrix->at.y))) + (imageY));
        pointsOut->z = ((((scale) * (matrix->at.z))) + (imageZ));

        /* Next vector */
        pointsIn++;
        pointsOut++;
    }

    RWRETURN(cachedOut);
}

/****************************************************************************
 _rwVectorSetMultFn

 On entry   : Vector for multiplying points (with translation),
              Vector for multiplying vectors (without translation),
 On exit    : point on success
 */

RwBool
_rwVectorSetMultFn(rwVectorMultFn multPoint, rwVectorMultFn multVector)
{
    RWFUNCTION(RWSTRING("_rwVectorSetMultFn"));

    if (NULL == multPoint)
    {
        multPoint = VectorMultPoint;
    }

    RWVECTORGLOBAL(multPoint) = multPoint;

    if (NULL == multVector)
    {
        multVector = VectorMultVector;
    }

    RWVECTORGLOBAL(multVector) = multVector;

    RWRETURN(TRUE);
}






/****************************************************************************
 _rwVectorClose

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwVectorClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwVectorClose"));

    InvSqrtTableDestroy();
    SqrtTableDestroy();

    /* One less module instance */
    --vectorModule.numInstances;

    RWRETURN(instance);
}

/****************************************************************************
 _rwVectorOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwVectorOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwVectorOpen"));

    /* Save the global data offset (same for all instances) */
    vectorModule.globalsOffset = offset;

    /* Set default vector operation functions */
    _rwVectorSetMultFn((rwVectorMultFn)NULL, (rwVectorMultFn)NULL);

    if (!SqrtTableCreate())
    {
        /* Failure */
        RWRETURN(NULL);
    }

    if (!InvSqrtTableCreate())
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* This is another module instance */
    vectorModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

