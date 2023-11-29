/***************************************************************************
 *                                                                         *
 * Module  : badebug.h                                                     *
 *                                                                         *
 * Purpose : Debug handling                                                *
 *                                                                         *
 **************************************************************************/

#ifndef RWDEBUG_H
#define RWDEBUG_H

/****************************************************************************
 Includes
 */

#include "baerr.h"
#include "batype.h"


/****************************************************************************
 Defines
 */

#ifdef RWDEBUG

#else /* RWDEBUG */

#define RWRETURN(value) return(value)
#define RWRETURNVOID() return
#define RWERROR(errorcode)                              \
MACRO_START                                             \
{                                                       \
    RwError _rwErrorCode;                               \
                                                        \
    _rwErrorCode.pluginID = rwID_COREPLUGIN;            \
    _rwErrorCode.errorCode = _rwerror errorcode;        \
                                                        \
    RwErrorSet(&_rwErrorCode);                          \
}                                                       \
MACRO_STOP

#define RWFUNCTION(name)       /* No op */
#define RWAPIFUNCTION(name)    /* No op */
#define RWASSERT(condition)    /* No op */

#endif /* RWDEBUG */

/* RWPUBLIC */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Opening and closing */
extern RwBool       _rwDebugOpen(void);
extern RwBool       _rwDebugClose(void);

/* RWPUBLIC */

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */




#endif /* _RWDEBUG_H */

