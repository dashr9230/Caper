
/***************************************************************************
 *                                                                         *
 * Module  : bavector.h                                                    *
 *                                                                         *
 * Purpose : Matrix handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWVECTOR_H
#define RWVECTOR_H

/****************************************************************************
 Includes
 */
#include "batypes.h"            /* picks up rwPI amongst others */

#include "bamatrix.h"

/* RWPUBLIC */

/*
 * Typedef for pointer to Vector multiplication by Matrix function
 */

typedef RwV3d *(*rwVectorMultFn) (RwV3d * pointsOut, 
                                  const RwV3d * pointsIn,
                                  RwInt32 numPoints,
                                  const RwMatrix * matrix);

/* RWPUBLICEND */

typedef struct rwVectorGlobals rwVectorGlobals;
struct rwVectorGlobals
{
    RwSplitBits *SqrtTab;
    RwSplitBits *InvSqrtTab;
    rwVectorMultFn multPoint;
    rwVectorMultFn multVector;
};

/* RWPUBLIC */







/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif         /* __cplusplus */

/* RWPUBLICEND */


/* Opening and closing */
extern void *_rwVectorOpen(void *instance,
                           RwInt32 offset, RwInt32 size);
extern void *_rwVectorClose(void *instance,
                            RwInt32 offset, RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif         /* __cplusplus */


/* RWPUBLICEND */

#endif         /* RWVECTOR_H */
