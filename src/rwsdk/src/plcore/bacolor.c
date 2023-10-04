/*
 *
 * Color space functionality.
 * Functions for manipulating RGB color space.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwrgba
 * \page rwrgbaoverview RwRGBA Overview
 *
 * This object represents color and alpha components.
 * 
 * The data is usually stored combined into a single 32-bit integer, with 8 bits per component, 
 * although this should not be assumed.
 *
 * Functions are provided to convert to and from the RwRGBAReal format, which uses individual RwReals 
 * to store each component. (Other RenderWare Graphics objects usually use the RwRGBA form, but a real 
 * number variant is useful for some color processing tasks.) 
 *
 * Further functionality is provided for tasks like gamma correction, storing and retrieving RGBA data 
 * directly from a pixel. 
 * 
*/


/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"

#include "bacolor.h"



/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo colorModule;

/****************************************************************************
 _rwColorOpen

 On entry   : Instance, offset, size
 On exit    : Instance on success
 */

void           *
_rwColorOpen(void *instance,
             RwInt32 __RWUNUSED__ offset , 
             RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwColorOpen"));
    RWASSERT(instance);

    /* One more instance */
    colorModule.numInstances++;

    /* All done */
    RWRETURN(instance);
}

/****************************************************************************
 _rwColorClose

 On entry   : Instance, offset, size
 On exit    : Instance on success
 */

void           *
_rwColorClose(void *instance,
              RwInt32 __RWUNUSED__ offset,
              RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwColorClose"));
    RWASSERT(instance);

    /* One less instance */
    colorModule.numInstances--;

    /* All done */
    RWRETURN(instance);
}













