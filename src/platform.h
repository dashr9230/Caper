
#ifndef PLATFORM_H
#define PLATFORM_H

/* temporary including from rwsdk. TODO: remove this */
#include "rwsdk/src/plcore/bamemory.h"

#include "skeleton.h"

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void psErrorMessage(const RwChar *text);
extern void psWarningMessage(const RwChar *text);

extern RwUInt32 psTimer(void);

extern RwBool psInitialize(void);
extern void   psTerminate(void);
extern RwBool psAlwaysOnTop( RwBool AlwaysOnTop );

extern void psMouseSetVisibility(RwBool visible);
extern void psMouseSetPos(RwV2d *pos);


/* return memory function overload if required */
extern RwMemoryFunctions *psGetMemoryFunctions(void);

/* install the platform specific file system */
extern RwBool psInstallFileSystem(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* PLATFORM_H */
