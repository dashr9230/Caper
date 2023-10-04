/*
 * Debug handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwdebug
 * \page rwdebugoverview RwDebug  Overview
 *
 * (DEBUG BUILD ONLY.)
 *
 * This object represents a debug handler and stream. Error messages can be sent 
 * to this object and it will pass them onto the currently registered debug handler. 
 * A default handler is registered by default. This handler sends messages to a file 
 * called 'rwdebug.log'.
 * 
 * Developers should note that this API may degrade performance if too many messages
 * are being sent to the handler.
 *
*/

/****************************************************************************
 Includes
 */

#include "batypes.h"

#include "badebug.h"

/****************************************************************************
 _rwDebugOpen

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwDebugOpen(void)
{
#if (0)
    RWFUNCTION(RWSTRING("_rwDebugOpen"));
#endif /* (0) */

    /* TODO: _rwDebugOpen */

    return (TRUE);
}

/****************************************************************************
 _rwDebugClose

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwDebugClose(void)
{
#if (0)
    RWFUNCTION(RWSTRING("_rwDebugClose"));
#endif /* (0) */

    /* TODO: _rwDebugClose */

    return TRUE;

}



