/*
 * Plugin registry
 * Plugin registries keep track of allocated memory in objects for toolkits
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */





/****************************************************************************
 Includes
 */

#include "batypes.h"

#include "balibtyp.h"
#include "badevice.h"
#include "bamemory.h"

#include "batkreg.h"





/****************************************************************************
 Local (static) Globals
 */

static RwFreeList *toolkitRegEntries;

/****************************************************************************
 Functions
 */

/****************************************************************************
 _rwPluginRegistryOpen

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwPluginRegistryOpen(void)
{
    RWFUNCTION(RWSTRING("_rwPluginRegistryOpen"));

    toolkitRegEntries = RwFreeListCreate(sizeof(RwPluginRegEntry), 20, 0);
    if (!toolkitRegEntries)
    {
        /* Failed to create free list */
        RWRETURN(FALSE);
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwDestroyEntry

 Destroy's freelist entry, and resets head of list to original size
 (so that we can shut and re-open the library)

 On entry   : object to destroy, freelist object is in
 On exit    : TRUE on success
 */

static void
rwDestroyEntry(void *object, void *freelist)
{
    RwPluginRegEntry *entry = (RwPluginRegEntry *) object;

    RWFUNCTION(RWSTRING("rwDestroyEntry"));

    /* Reset the size of the structure */
    if (entry->parentRegistry->firstRegEntry != NULL)
    {
        entry->parentRegistry->sizeOfStruct = entry->parentRegistry->origSizeOfStruct;
        entry->parentRegistry->firstRegEntry = (RwPluginRegEntry *)NULL;
        entry->parentRegistry->lastRegEntry = (RwPluginRegEntry *)NULL;
    }

    /* Free the entry up from the free list */
    RwFreeListFree((RwFreeList *) freelist, entry);

    RWRETURNVOID();
}

/****************************************************************************
 _rwPluginRegistryClose

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwPluginRegistryClose(void)
{
    RWFUNCTION(RWSTRING("_rwPluginRegistryClose"));

    /* Shut down the list of plugin registries.  Also reset their sizes */
    if (toolkitRegEntries)
    {
        /* Destroy all entries in the free list */
        RwFreeListForAllUsed(toolkitRegEntries, rwDestroyEntry, toolkitRegEntries);

        /* Then shut down the free list */
        RwFreeListDestroy(toolkitRegEntries);
        toolkitRegEntries = (RwFreeList *)NULL;
    }

    RWRETURN(TRUE);
}

/****************************************************************************
 PluginDefaultConstructor

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultConstructor(void *object,
                         RwInt32 __RWUNUSED__ offsetInObject,
                         RwInt32 __RWUNUSED__ sizeInObject)
{
    RWFUNCTION(RWSTRING("PluginDefaultConstructor"));

    /* Do nothing */
    RWRETURN(object);
}

/****************************************************************************
 PluginDefaultDestructor

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultDestructor(void *object,
                           RwInt32 __RWUNUSED__ offsetInObject,
                           RwInt32 __RWUNUSED__ sizeInObject)
{
    RWFUNCTION(RWSTRING("PluginDefaultDestructor"));

    /* Do nothing */
    RWRETURN(object);
}

/****************************************************************************
 PluginDefaultCopy

 On entry   : All the usual - it's not too important since we do the dumb thing
 On exit    : stream pointer
 */

static void    *
PluginDefaultCopy(void *dstObject,
                     const void * __RWUNUSED__ srcObject,
                     RwInt32 __RWUNUSED__ offsetInObject,
                     RwInt32 __RWUNUSED__ sizeInObject)
{
    RWFUNCTION(RWSTRING("PluginDefaultCopy"));

    /* Do nothing */
    RWRETURN(dstObject);
}







/****************************************************************************
 _rwPluginRegistryGetPluginOffset

 On entry   : Registry to serach for pluginID
            : Plugin ID to search for
 On exit    : Offset if plugin is found, or -1 if not found
 */

RwBool
_rwPluginRegistryGetPluginOffset(const RwPluginRegistry * reg,
                                 RwUInt32 pluginID)
{
    const RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryGetPluginOffset"));
    RWASSERT(reg);

    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            RWRETURN(entry->offset);
        }

        entry = entry->nextRegEntry;
    }

    RWRETURN(-1);
}













/****************************************************************************
 _rwPluginRegistryAddPlugin

 On entry   : Registry to add to
            : Size of block to reserve, 
            : Plugin ID (32 bit number)
            : constructor, destructor and copy callbacks
 On exit    : Offset into structure of reserved memory
 */

RwInt32
_rwPluginRegistryAddPlugin(RwPluginRegistry * reg,
                           RwInt32 size, RwUInt32 pluginID,
                           RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB,
                           RwPluginObjectCopy copyCB)
{
    RwPluginRegEntry *entry;
    RwInt32 newStructSize;

    RWFUNCTION(RWSTRING("_rwPluginRegistryAddPlugin"));
    RWASSERT(reg);
    RWASSERT(toolkitRegEntries);

    if (!toolkitRegEntries)
    {
        /* Not a good time to do this - there is no free list, we can
         * infer from this that the library has not been initialised.
         */
        RWRETURN(-1);
    }

    /* Make sure that the library has not been opened yet */
    RWASSERT(_rwGetNumEngineInstances() == 0);
    if (_rwGetNumEngineInstances() != 0)
    {
        /* Not good, the engine has been opened */
        RWERROR((E_RW_PLUGININIT));
        RWRETURN(-1);
    }

    /* Make sure it's not in the list yet */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            /* Can't register twice, return the already registered offset */
            RWRETURN(entry->offset);
        }
        entry = entry->nextRegEntry;
    }

    /* Increase structure size, but keep longword alignment */
    newStructSize = reg->sizeOfStruct + ((size + 3) & ~3);

    if (reg->maxSizeOfStruct && (newStructSize > reg->maxSizeOfStruct))
    {
        /* We've exceeded our limit, so fail */
        RWRETURN(-1);
    }

    entry = (RwPluginRegEntry *) RwFreeListAlloc(toolkitRegEntries);
    RWASSERT(RWFREELISTALIGNED(entry, toolkitRegEntries));

    if (entry)
    {
        /* Fill it the entry */

        entry->offset = reg->sizeOfStruct;

        reg->sizeOfStruct = newStructSize;

        entry->size = size;
        entry->pluginID = pluginID;

        /* TODO: _rwPluginRegistryAddPlugin */

        entry->constructCB = constructCB ? constructCB : PluginDefaultConstructor;
        entry->destructCB = destructCB ? destructCB : PluginDefaultDestructor;
        entry->copyCB = copyCB ? copyCB : PluginDefaultCopy;

        entry->nextRegEntry = (RwPluginRegEntry *) NULL;
        entry->prevRegEntry = (RwPluginRegEntry *) NULL;

        /* Point back to the parent */
        entry->parentRegistry = reg;

        /* Append it to the list */
        if (reg->firstRegEntry == NULL)
        {
            /* First in the list, just shove it in */
            reg->firstRegEntry = entry;
            reg->lastRegEntry = entry;
        }
        else
        {
            /* Whack it on the end */
            reg->lastRegEntry->nextRegEntry = entry;
            entry->prevRegEntry = reg->lastRegEntry;

            /* New end of the list */
            reg->lastRegEntry = entry;
        }

        RWRETURN(entry->offset);
    }

    /* Failed to allocate a freelist entry */
    RWRETURN(-1);
}













/****************************************************************************
 _rwPluginRegistryInitObject

 On entry   : Registry defining toolkits registered with object
            : Object to call constructors for
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryInitObject(const RwPluginRegistry * reg, void *object)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryInitObject"));
    RWASSERT(reg);
    RWASSERT(object);

    /* Run through the list forwards */
    entry = reg->firstRegEntry;
    while (entry)
    {
        if (!entry->constructCB(object, entry->offset, entry->size))
        {
            /* Failed, go back along the constructor list */
            entry = entry->prevRegEntry;

            while (entry)
            {
                entry->destructCB(object, entry->offset, entry->size);
                entry = entry->prevRegEntry;
            }

            RWRETURN((const RwPluginRegistry *)NULL);
        }

        entry = entry->nextRegEntry;
    }

    RWRETURN(reg);
}

/****************************************************************************
 _rwPluginRegistryDeInitObject

 On entry   : Registry defining toolkits registered with object
            : Object to call destructors for
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryDeInitObject(const RwPluginRegistry * reg, void *object)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryDeInitObject"));
    RWASSERT(reg);
    RWASSERT(object);

    /* Run through the list backwards */
    for (entry = reg->lastRegEntry;
         entry;
         entry = entry->prevRegEntry)
    {
        const RwPluginObjectDestructor  destructCB =  entry->destructCB;
        const RwInt32 offsetInObject = entry->offset;
        const RwInt32 sizeInObject = entry->size;

        RWASSERT(destructCB);
        RWASSERT(offsetInObject>=0);
        RWASSERT(sizeInObject>=0);

        destructCB(object, offsetInObject, sizeInObject);
    }

    RWRETURN(reg);
}

/****************************************************************************
 _rwPluginRegistryCopyObject

 On entry   : Registry defining toolkits registered with object
            : Object to copy to
            : Object to copy from
 On exit    : Registry pointer on success
 */

const RwPluginRegistry *
_rwPluginRegistryCopyObject(const RwPluginRegistry * reg, void
                            *dstObject, const void *srcObject)
{
    RwPluginRegEntry *entry;

    RWFUNCTION(RWSTRING("_rwPluginRegistryCopyObject"));
    RWASSERT(reg);
    RWASSERT(dstObject);
    RWASSERT(srcObject);

    /* Run through the list forwards */
    entry = reg->firstRegEntry;
    while (entry)
    {
        entry->copyCB(dstObject, srcObject, entry->offset, entry->size);
        entry = entry->nextRegEntry;
    }

    RWRETURN(reg);
}
