/***************************************************************************
 *                                                                         *
 * Module  : bamatrix.h                                                    *
 *                                                                         *
 * Purpose : Matrix handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATRIX_H
#define RWMATRIX_H



/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#define RWMATRIXALIGNMENT(_mat) \
   (! (((rwMATRIXALIGNMENT)-1) & ((RwUInt32)(_mat))))


 /* Internal flags (bits 16-31) */
enum RwMatrixType
{
    rwMATRIXTYPEORTHONORMAL = 0x00000003,
};
typedef enum RwMatrixType RwMatrixType;

enum RwMatrixFlag
{
    rwMATRIXINTERNALIDENTITY = 0x00020000,
};
typedef enum RwMatrixFlag RwMatrixFlag;

/* Flags describing what will optimize for */
enum RwMatrixOptimizations
{
    rwMATRIXOPTIMIZE_IDENTITY = 0x00020000,
};
typedef enum RwMatrixOptimizations RwMatrixOptimizations;

/****************************************************************************
 Global Types
 */


struct RwMatrix
{
    /* These are padded to be 16 byte quantities per line */
    RwV3d               right;
    RwUInt32            flags;
    RwV3d               up;
    RwUInt32            pad1;
    RwV3d               at;
    RwUInt32            pad2;
    RwV3d               pos;
    RwUInt32            pad3;
};


/**
 * \ingroup datatypes
 * \typedef RwMatrix 
 * Matrix to define transformations. 
 * This should be considered an opaque type.
 * Use the RwMatrix API functions to access.
 */
typedef struct RwMatrix RWALIGN(RwMatrix, rwMATRIXALIGNMENT);






#if (!defined(RwMatrixSetIdentityMacro))
#define RwMatrixSetIdentityMacro(m)                                     \
MACRO_START                                                             \
{                                                                       \
    (m)->right.x = (m)->up.y    = (m)->at.z    = (RwReal)((1.0));       \
    (m)->right.y = (m)->right.z = (m)->up.x    = (RwReal)((0.0));       \
    (m)->up.z    = (m)->at.x    = (m)->at.y    = (RwReal)((0.0));       \
    (m)->pos.x   = (m)->pos.y   = (m)->pos.z   = (RwReal)((0.0));       \
    rwMatrixSetFlags((m),                                               \
                     rwMatrixGetFlags(m) |                              \
                     (rwMATRIXINTERNALIDENTITY |                        \
                      rwMATRIXTYPEORTHONORMAL));                        \
}                                                                       \
MACRO_STOP
#endif /* (!defined(RwMatrixSetIdentityMacro)) */

typedef void        (RWASMCALL * rwMatrixMultFn) (RwMatrix * dstMat,
                                                  const RwMatrix * matA,
                                                  const RwMatrix *
                                                  matB);




typedef struct RwMatrixTolerance RwMatrixTolerance;
struct RwMatrixTolerance
{
    RwReal              Normal;
        /**< Tolerance within which matrix is deemed to be normal   */
    RwReal              Orthogonal;
        /**< Tolerance within which matrix is deemed to be orthogonal */
    RwReal              Identity;
        /**< Tolerance within which matrix is deemed to be identity */
};

/* RWPUBLICEND */

typedef struct rwMatrixGlobals rwMatrixGlobals;
struct rwMatrixGlobals
{
    RwFreeList         *matrixFreeList;
    RwInt32             matrixOptimizations;
    rwMatrixMultFn      multMatrix;
    RwMatrixTolerance   tolerance;
};

/* RWPUBLIC */




#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwBool
RwEngineSetMatrixTolerances(const RwMatrixTolerance * const tolerance);

/* Update */
#define rwMatrixSetFlags(m, flagsbit)     ((m)->flags = (flagsbit))
#define rwMatrixGetFlags(m)               ((m)->flags)

#define RwMatrixSetIdentity(m)   RwMatrixSetIdentityMacro(m)

/* RWPUBLICEND */

extern void *
_rwMatrixOpen(void *instance,
              RwInt32 offset,
              RwInt32 size);

extern void *
_rwMatrixClose(void *instance,
               RwInt32 offset,
               RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */





/* Initialisation/deinitialisation */
#define rwMatrixInitialize(m, t)                \
MACRO_START                                     \
{                                               \
    rwMatrixSetFlags((m), (t));                 \
}                                               \
MACRO_STOP




/* RWPUBLICEND */

#define rwMatrixInitializeIdentity(m, t)        \
MACRO_START                                     \
{                                               \
    rwMatrixInitialize((m), (t));               \
    RwMatrixSetIdentity((m));                   \
}                                               \
MACRO_STOP

#endif                          /* RWMATRIX_H */
