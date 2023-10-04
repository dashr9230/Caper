/*
 * Memory handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwfreelist
 * \page rwfreelistoverview RwFreeList Overview
 *
 * This object provides a platform-neutral memory allocation API.
 *
 * A free list can be used to allocate memory for
 * lists of identical datatypes.
 * As the list is filled, the free list mechanism can be set to automatically
 * allocate more memory in developer-defined chunks.
 * Getting the most out of this system can require some fine-tuning work,
 * but the result should be an optimal trade-off between
 * memory usage and speed.
 */

/**
 * \ingroup rwmem
 * \page rwmemoverview RwMem Overview
 *
 * A set of utility functions intended to ease cross-platform compatibility of
 * RenderWare Graphics data structures.
 *
 * These functions include conversions from RenderWare Graphics' intrinsic
 * types - RwInt32, RwInt16, RwReal, etc. - into platform-neutral forms.
 *
 */




/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"

#include "bamemory.h"





/****************************************************************************
 Local (static) Globals
 */

static RwLinkList   llGAllFreeLists;

#ifndef calloc

/* Make our own fake version of calloc for systems that don't have it */
static void *
FakeCalloc(size_t numObj, size_t sizeObj)
{
    size_t              totalSize = numObj * sizeObj;
    void               *newMem;

    RWFUNCTION(RWSTRING("FakeCalloc"));

    /* Allocate it */
    newMem = malloc(totalSize);

    /* Clear it if allocated */
    if (newMem)
    {
        memset(newMem, 0, totalSize);
    }

    /* Return the memory if allocated */
    RWRETURN(newMem);
}
#endif /* calloc */

















/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Free Lists

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwfreelist
 * \ref RwFreeListDestroy is used to release the block (or blocks) of
 * memory used by the specified free list.
 *
 * \param freelist  Pointer to the free list.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwBool
RwFreeListDestroy(RwFreeList * freelist)
{
    RwFreeBlock        *fbpCur;

    RWAPIFUNCTION(RWSTRING("RwFreeListDestroy"));
    RWASSERT(freelist);

    if (freelist->freeListStack)
    {
        RwFree(freelist->freeListStack);
    }

    /* Free all of the blocks */
    fbpCur = freelist->firstBlock;

    while (fbpCur)
    {
        RwFreeBlock        *nextBlock;

        nextBlock = fbpCur->nextBlock;
        RwFree(fbpCur);
        fbpCur = nextBlock;
    }

    /* Remove the free list from the list of all freelists */
    rwLinkListRemoveLLLink(&freelist->lFreeList);

    /* Done */
    RwFree(freelist);
    RWRETURN(TRUE);
}

/*
 * TODO: Possible crash. Will need to remove error handing in FreeListCreate,
 * or maybe bring some stuff from RW 3.7
 * 
 * _rwPluginRegistryOpen calls RwFreeListCreate to initiate toolkitRegEntries
 * but CorePluginAttach at that point that not been called initializing
 * error handling module.
 */
static RwFreeList *
FreeListCreate(RwInt32 entrySize, RwInt32 blockEntries, RwInt32 alignment)
{
    RwFreeList         *freelist;

    RWFUNCTION(RWSTRING("FreeListCreate"));
    RWASSERT(entrySize);
    RWASSERT(blockEntries);
    RWASSERT(!(alignment & (alignment - 1))); /* Assert power of 2 */

    freelist = (RwFreeList *) RwMalloc(sizeof(RwFreeList));
    if (freelist)
    {

        /* Deal with don't care case (align to 1 byte :-) */
        alignment = (alignment > 1) ? alignment : 1;

        /* Enforce alignment of packed elements */
        freelist->alignmentMinusOne = alignment - 1;
        entrySize += freelist->alignmentMinusOne;
        entrySize &= (~freelist->alignmentMinusOne);

        /* Set characteristics of the free list */
        freelist->entrySize = entrySize;
        freelist->entriesPerBlock = blockEntries;

        /* The block size */
        freelist->blockSize =
            (entrySize * blockEntries) + sizeof(RwFreeBlock);

        /* Allow enough space for alignment of first element */
        freelist->blockSize += freelist->alignmentMinusOne;

        /* Say that the stack is empty */
        freelist->freeListStack = (void **) RwMalloc(sizeof(void *));

        if (freelist->freeListStack)
        {

            freelist->freeListStackTop = freelist->freeListStack - 1;
            freelist->entriesAllocated = 0;
            freelist->firstBlock = (RwFreeBlock *) NULL;

            /* Add the freelist the link list of all free lists */
            rwLinkListAddLLLink(&llGAllFreeLists, &freelist->lFreeList);
        }
        else
        {
            RWERROR((E_RW_NOMEM, sizeof(void *)));
            RwFree(freelist);
            freelist = ((RwFreeList *) NULL);
        }
    }
    else
    {
        RWERROR((E_RW_NOMEM, sizeof(RwFreeList)));
        freelist = ((RwFreeList *) NULL);
    }

    /* Done */
    RWRETURN(freelist);
}


RwFreeList *
_rwFreeListFreeReal(RwFreeList * freelist, void *pBlock)
{
    RWFUNCTION(RWSTRING("_rwFreeListFreeReal"));
    RWASSERT(freelist);
    RWASSERT(pBlock);

    /* Free the chunk of memory */
    (*(++(freelist->freeListStackTop))) = pBlock;

    RWRETURN(freelist);
}










void *
_rwFreeListAllocReal(RwFreeList * freelist)
{
    void               *pBlock;

    RWFUNCTION(RWSTRING("_rwFreeListAllocReal"));
    RWASSERT(freelist);

    if (freelist->freeListStackTop < freelist->freeListStack)
    {
        RwFreeBlock        *fbpBlock;
        void              **freeListStack;
        RwUInt32            bytes;

        /* Create a block */
        fbpBlock = (RwFreeBlock *) RwMalloc(freelist->blockSize);

        if (!fbpBlock)
        {
            RWERROR((E_RW_NOMEM, (freelist->blockSize)));
            RWRETURN(NULL);
        }

        /* Create the stack */

        bytes = ( sizeof(void *) *
                  (freelist->entriesAllocated + freelist->entriesPerBlock) );

        freeListStack = (void **) RwMalloc(bytes);

        if (!freeListStack)
        {
            RWERROR((E_RW_NOMEM, (bytes)));

            RwFree(fbpBlock);
            RWRETURN(NULL);
        }

        RwFree(freelist->freeListStack);

        freelist->freeListStack = freeListStack;

        /* We now have this many entries stored */
        freelist->entriesAllocated += freelist->entriesPerBlock;

        /* Add the pBlock to the list */
        fbpBlock->nextBlock = freelist->firstBlock;
        freelist->firstBlock = fbpBlock;

        /* Add the new pBlock entries */
        {
            const RwInt32       offset = (freelist->blockSize -
                                          freelist->entrySize);
            RwUInt8            *cpData = (((RwUInt8 *) (fbpBlock)) + offset);
            RwInt32             nI;

            /* Align last pBlock */
            cpData = ((RwUInt8 *) ((RwUInt32) cpData &
                                   (~freelist->alignmentMinusOne)));

            /* Then work backwards adding to the freelist stack */
            nI = freelist->entriesPerBlock;

            RWASSERT(0 < nI);

            while (nI--)
            {
                (*freeListStack) = (void *) cpData;
                freeListStack++;
                cpData -= freelist->entrySize;
            }
        }

        freelist->freeListStackTop = (freelist->freeListStack) +
            (freelist->entriesPerBlock - 1);
    }

    /* Do the alloc */
    pBlock = *(freelist->freeListStackTop--);

    /* Assert desired alignment has occured */
    RWASSERT(((RwUInt32) pBlock & freelist->alignmentMinusOne) == 0);

    /* Return the pBlock */
    RWRETURN(pBlock);
}
















/**
 * \ingroup rwfreelist
 * \ref RwFreeListCreate is used to create a new free list that will
 * contain the specified number of entries, each of the specified size (in
 * bytes), in a single block of memory. Initially, the free list is empty and
 * no memory is allocated for any entries. Only when one of the entries is
 * requested from a call to \ref RwFreeListAlloc is the whole block allocated.
 *
 * \param entrySize  A RwInt32 value equal to
 *                   the size of each entry (in bytes).
 * \param blockEntries  A RwInt32 value equal to
 *                      the number of entries.
 * \param alignment  A RwInt32 value equal to
 *                   the desired byte alignment of entries.
 *                   The alignment must be a power of two.
 *                  Zero indicates don't care.
 *
 * \return Returns pointer to the new free list if successful
 *         or NULL if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListForAllUsed
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 */
RwFreeList *
RwFreeListCreate(RwInt32 entrySize, RwInt32 blockEntries,  RwInt32 alignment)
{
    RwFreeList         *freelist;

    RWAPIFUNCTION(RWSTRING("RwFreeListCreate"));
    RWASSERT(entrySize);
    RWASSERT(blockEntries);
    RWASSERT(!(alignment & (alignment - 1))); /* Assert power of 2 */

    freelist = FreeListCreate(entrySize, blockEntries, alignment);

    RWRETURN(freelist);
}







/**
 * \ingroup rwfreelist
 * \ref RwFreeListForAllUsed is used to apply the given callback
 * function to all used entries in the specified free list. The format of
 * the callback function is:
 * \verbatim
        void (*RwFreeListCallBack)(void *mem, void *data);
   \endverbatim
 * where data is a user-defined data pointer to pass to the callback function.
 *
 * \param freelist  Pointer to free list.
 * \param fpCallBack  Pointer to the callback to apply to call with each used block.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the free list if successful or NULL
 * if there is an error.
 *
 * \see RwFreeListAlloc
 * \see RwFreeListCreate
 * \see RwFreeListFree
 * \see RwFreeListPurge
 * \see RwFreeListDestroy
 * \see RwFreeListPurgeAllFreeLists
 *
 */
RwFreeList *
RwFreeListForAllUsed(RwFreeList * freelist,
                     RwFreeListCallBack fpCallBack, void *pData)
{
    RwUInt8            *cpCur;
    RwFreeBlock        *fbpCur = freelist->firstBlock;

    RWAPIFUNCTION(RWSTRING("RwFreeListForAllUsed"));
    RWASSERT(freelist);
    RWASSERT(fpCallBack);

    while (fbpCur)
    {
        RwInt32             nI;

        cpCur =
            ((RwUInt8 *) (fbpCur + 1)) + freelist->alignmentMinusOne;

        nI = freelist->entriesPerBlock;

        while (nI--)
        {
            void              **ppPos;

            /* Check if the address is already on the stack */
            ppPos = freelist->freeListStack;
            while (ppPos <= freelist->freeListStackTop)
            {
                if ((*ppPos) == (void *) cpCur)
                {
                    break;
                }

                /* next */
                ppPos++;
            }

            /* Is this used then? */
            if (!(ppPos <= freelist->freeListStackTop))
            {
                fpCallBack((void *) cpCur, pData);
            }

            /* Onto the next entry */
            cpCur += freelist->entrySize;
        }

        /* next */
        fbpCur = fbpCur->nextBlock;
    }

    RWRETURN(freelist);
}











/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Open/close

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwMemoryClose

 On entry   :
 On exit    :
 */

void
_rwMemoryClose(void)
{
    RWFUNCTION(RWSTRING("_rwMemoryClose"));

    RWRETURNVOID();
}

/****************************************************************************
 _rwMemoryOpen

 On entry   : Optional pointer to replacement memory functions
 On exit    : TRUE on success
 */

RwBool
_rwMemoryOpen(RwMemoryFunctions * memFuncs)
{
    RWFUNCTION(RWSTRING("_rwMemoryOpen"));

    rwLinkListInitialize(&llGAllFreeLists);

    if (memFuncs)
    {
        /* Install user specified memory system */
        RWSRCGLOBAL(memoryFuncs) = *memFuncs;
    }
    else
    {
        /* Install an ANSI memory system */
        RWSRCGLOBAL(memoryFuncs).rwmalloc = malloc;
        RWSRCGLOBAL(memoryFuncs).rwfree = free;
        RWSRCGLOBAL(memoryFuncs).rwrealloc = realloc;
#if (defined(calloc))
        RWSRCGLOBAL(memoryFuncs).rwcalloc = calloc;
#else /* (defined(calloc)) */
        RWSRCGLOBAL(memoryFuncs).rwcalloc = FakeCalloc;
#endif /* (defined(calloc)) */
    }

    RWRETURN(TRUE);
}




