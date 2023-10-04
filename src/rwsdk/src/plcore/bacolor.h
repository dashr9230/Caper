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




/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */



/* Init/Term */
extern void *_rwColorOpen(void *instance, RwInt32 offset, RwInt32 size);
extern void *_rwColorClose(void *instance, RwInt32 offset, RwInt32 size);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



#endif /* RWCOLOR_H */

