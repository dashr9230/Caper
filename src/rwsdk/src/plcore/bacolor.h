/***************************************************************************
 *                                                                         *
 * Module  : bacolor.h                                                     *
 *                                                                         *
 * Purpose : Colour space structures and functionality                     *
 *                                                                         *
 **************************************************************************/

#ifndef RWCOLOR_H
#define RWCOLOR_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/* RWPUBLIC */



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Init/Term */
extern void *_rwColorOpen(void *instance, RwInt32 offset, RwInt32 size);
extern void *_rwColorClose(void *instance, RwInt32 offset, RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWCOLOR_H */

