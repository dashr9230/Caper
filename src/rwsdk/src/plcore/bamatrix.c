/*
 * Matrix handling functions.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwmatrix
 * \page rwmatrixoverview RwMatrix Overview
 *
 * This object defines a RenderWare Graphics Matrix object.
 *
 * The Matrix is heavily used throughout the API and a full range of
 * functions are provided, including: rotation, multiplication,
 * concatenation, scaling, translation, creation, destruction, stream
 * read/write functions and a number of access functions to access
 * particular vector data within the matrix.
 *
 * RenderWare uses 4x3, row-major affine matrices.
 */

/****************************************************************************
 Includes
 */

#include "batypes.h"

#include "balibtyp.h"





#include "bamatrix.h"


#define RWMATRIXOPTIMIZETOLERANCE (((RwReal)0.01))


#define RWMATRIXGLOBAL(var)                     \
    (RWPLUGINOFFSET(rwMatrixGlobals,            \
     RwEngineInstance,                          \
     matrixModule.globalsOffset)->var)






/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo matrixModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Matrix handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 MatrixMultiply

 On entry   : Dest matrix pointer, two source matrix pointers
 On exit    : Matrix pointer contains result
 */
static
RWASMAPI(void)
MatrixMultiply(RwMatrix * dstMat,
               const RwMatrix * matA, const RwMatrix * matB)
{
    RWFUNCTION(RWSTRING("MatrixMultiply"));
    RWASSERT(dstMat);
    RWASSERT(matA);
    RWASSERT(RWMATRIXALIGNMENT(matA));
    RWASSERT(matB);
    RWASSERT(RWMATRIXALIGNMENT(matB));

    /* Multiply out right */
    dstMat->right.x = 
        (matA->right.x * matB->right.x) +
        (matA->right.y * matB->up.x) + (matA->right.z * matB->at.x);
    dstMat->right.y =
        (matA->right.x * matB->right.y) +
        (matA->right.y * matB->up.y) + (matA->right.z * matB->at.y);
    dstMat->right.z =
        (matA->right.x * matB->right.z) +
        (matA->right.y * matB->up.z) + (matA->right.z * matB->at.z);


    /* Then up */
    dstMat->up.x = 
        (matA->up.x * matB->right.x) +
        (matA->up.y * matB->up.x) + (matA->up.z * matB->at.x);
    dstMat->up.y =
        (matA->up.x * matB->right.y) +
        (matA->up.y * matB->up.y) + (matA->up.z * matB->at.y);
    dstMat->up.z =
        (matA->up.x * matB->right.z) +
        (matA->up.y * matB->up.z) + (matA->up.z * matB->at.z);
    
    /* Then at */
    dstMat->at.x =
        (matA->at.x * matB->right.x) +
        (matA->at.y * matB->up.x) + (matA->at.z * matB->at.x);
    dstMat->at.y =
        (matA->at.x * matB->right.y) +
        (matA->at.y * matB->up.y) + (matA->at.z * matB->at.y);
    dstMat->at.z =
        (matA->at.x * matB->right.z) +
        (matA->at.y * matB->up.z) + (matA->at.z * matB->at.z);

    /* Then pos - this is different because there is an extra add
     * (implicit 1 (one) in bottom right of matrix)
     */
    dstMat->pos.x =
        (matA->pos.x * matB->right.x) +
        (matA->pos.y * matB->up.x) +
        (matA->pos.z * matB->at.x) + ( /* (1*) */ matB->pos.x);
    dstMat->pos.y =
        (matA->pos.x * matB->right.y) +
        (matA->pos.y * matB->up.y) +
        (matA->pos.z * matB->at.y) + ( /* (1*) */ matB->pos.y);
    dstMat->pos.z =
        (matA->pos.x * matB->right.z) +
        (matA->pos.y * matB->up.z) +
        (matA->pos.z * matB->at.z) + ( /* (1*) */ matB->pos.z);

    /* And that's all folks */
    RWRETURNVOID();
}
















/****************************************************************************
 _rwMatrixSetMultFn

 On entry   : Matrix multiply function (NULL for default)
 On exit    : TRUE on success
 */

RwBool
_rwMatrixSetMultFn(rwMatrixMultFn multMat)
{
    RWFUNCTION(RWSTRING("_rwMatrixSetMultFn"));

    if (NULL == multMat)
    {
        multMat = MatrixMultiply;
    }
    RWMATRIXGLOBAL(multMatrix) = multMat;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwMatrixSetOptimizations

 On entry   : Optimize flags (currently only rwMATRIXOPTIMIZE_IDENTITY is supported).
 On exit    : TRUE on success
 */

RwBool
_rwMatrixSetOptimizations(RwInt32 optimizeFlags)
{
    RWFUNCTION(RWSTRING("_rwMatrixSetOptimizations"));

    RWMATRIXGLOBAL(matrixOptimizations) = optimizeFlags;

    RWRETURN(TRUE);
}















/****************************************************************************
 _rwMatrixClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwMatrixClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
	RWFUNCTION(RWSTRING("_rwMatrixClose"));
    if (RWMATRIXGLOBAL(matrixFreeList))
    {
        RwFreeListDestroy(RWMATRIXGLOBAL(matrixFreeList));
        RWMATRIXGLOBAL(matrixFreeList) = (RwFreeList *) NULL;
    }

    /* One less module instance */
    matrixModule.numInstances--;
    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening the matrix library

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwMatrixOpen

 On entry   :
 On exit    : FALSE on error
 */

void               *
_rwMatrixOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
	RWFUNCTION(RWSTRING("_rwMatrixOpen"));

	/* Save data block offset (same for all instances) */
	matrixModule.globalsOffset = offset;

	/* We just happen to know that the size is a mutiple of 16 bytes, so
     * this doesn't hurt on other platforms. :-)
     */
    RWMATRIXGLOBAL(matrixFreeList) =
        RwFreeListCreate(sizeof(RwMatrix), 50, rwMATRIXALIGNMENT);
    if (!RWMATRIXGLOBAL(matrixFreeList))
    {
        /* Failure */
        instance = NULL;
    }
    else
    {
        const RwMatrixTolerance tolerance =
            {
                RWMATRIXOPTIMIZETOLERANCE,
                RWMATRIXOPTIMIZETOLERANCE,
                RWMATRIXOPTIMIZETOLERANCE
            };

        /* Set default optimizations */
        _rwMatrixSetOptimizations(rwMATRIXOPTIMIZE_IDENTITY);
        /* Set default multiply function */
        _rwMatrixSetMultFn((rwMatrixMultFn) NULL);
        /* Set default tolerances */
        RwEngineSetMatrixTolerances(&tolerance);

        /* One more module instance */
        matrixModule.numInstances++;
    }

    /* Success */
    RWRETURN(instance);
}






/**
 * \ingroup rwmatrix
 * \ref RwEngineSetMatrixTolerances
 * Sets current tolerances for
 * matrix optimizations with \ref RwMatrixOptimize
 *
 * This function must be used after the RenderWare engine has been started;
 * a call to RwEngineStart will nullify the effect of any previous call to
 * RwEngineSetMatrixTolerance.
 *
 * \param tolerance The source address from which tolerances are to be read
 *
 * \return Returns TRUE if successful, FALSE if there is an error
 *
 * \see RwEngineGetMatrixTolerances
 * \see RwMatrixOptimize
 */
RwBool
RwEngineSetMatrixTolerances(const RwMatrixTolerance * const tolerance)
{
    const RwBool        result = TRUE;
    RwMatrixTolerance  *const dest = &(RWMATRIXGLOBAL(tolerance));

    RWAPIFUNCTION(RWSTRING("RwEngineSetMatrixTolerances"));

    RWASSERT(tolerance);
    RWASSERT(dest);

    *dest = *tolerance;

    RWRETURN(result);
}













/**
 * \ingroup rwmatrix
 * \ref RwMatrixDestroy is used to delete a transformation matrix previously
 * created by the API function \ref RwMatrixCreate.
 * matrix.
 *
 * \param matrix  Pointer to matrix which is to be destroyed.
 *
 * \return Returns TRUE.
 *
 * \see RwMatrixCreate
 *
 */
RwBool
RwMatrixDestroy(RwMatrix * matrix)
{
    RWAPIFUNCTION(RWSTRING("RwMatrixDestroy"));
    RWASSERT(matrixModule.numInstances);
    RWASSERT(matrix);
    RWASSERT(RWMATRIXALIGNMENT(matrix));

    RwFreeListFree(RWMATRIXGLOBAL(matrixFreeList), matrix);

    RWRETURN(TRUE);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixCreate creates a new transformation matrix initialized to
 * the identity matrix.  Note that the matrix must be destroyed when it is no
 * longer required by an application.
 *
 * \return Returns pointer to created matrix if successful or NULL if
 * there is an error.
 *
 * \see RwMatrixCopy
 * \see RwMatrixDestroy
 *
 */
RwMatrix           *
RwMatrixCreate(void)
{
    RwMatrix           *matrix = (RwMatrix *) NULL;

    RWAPIFUNCTION(RWSTRING("RwMatrixCreate"));
    RWASSERT(matrixModule.numInstances);

    matrix =
        (RwMatrix *) RwFreeListAlloc(RWMATRIXGLOBAL(matrixFreeList));

    if (matrix)
    {
        rwMatrixInitializeIdentity(matrix, rwMATRIXTYPEORTHONORMAL);
    }

    RWRETURN(matrix);
}














