/***************************************************************************
 *                                                                         *
 * Module  : bamemory.h                                                    *
 *                                                                         *
 * Purpose : Memory handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWMEMORY_H
#define RWMEMORY_H

/****************************************************************************
 Includes
 */

#include <stdlib.h>


/* RWPUBLIC */

/****************************************************************************
 Defines
 */




#define RWFREELISTALIGNED(_pData, _freelist) \
  (! (((RwUInt32)(_pData)) & ((_freelist)->alignmentMinusOne)) )










#if (!defined(RwMalloc))
#define RwMalloc(_s)   ((RWSRCGLOBAL(memoryFuncs).rwmalloc)((_s)))
#endif /* (!defined(RwMalloc)) */

#if (!defined(RwFree))
#define RwFree(_p)     ((RWSRCGLOBAL(memoryFuncs).rwfree)((_p)))
#endif /* (!defined(RwFree)) */





#if (!defined(RwRealloc))
#define RwRealloc(_p, _s)  ((RWSRCGLOBAL(memoryFuncs).rwrealloc)((_p),(_s)))
#endif /* (!defined(RwRealloc)) */


#if (!defined(RWCRTCHECKMEMORY))
#define RWCRTCHECKMEMORY()     /* No Op */
#endif /* (!defined(RWCRTCHECKMEMORY)) */


#if (!defined(RWCRTHEAPDIFFERENCESINCE))
#define RWCRTHEAPDIFFERENCESINCE(_Then)           /* No Op */
#endif /* (!defined(RWCRTHEAPDIFFERENCESINCE)) */


/****************************************************************************
 Global Types
 */

typedef struct RwMemoryFunctions RwMemoryFunctions;
/**
 * \ingroup datatypes
 * \struct RwMemoryFunctions
 * This type represents the memory functions used
 * by RenderWare. By default, the standard ANSI functions are used. The
 * application may install an alternative interface providing that it is ANSI
 * compliant (see API function \ref RwEngineInit):
 */
struct RwMemoryFunctions
{
    /* c.f.
     * Program Files/Microsoft Visual Studio/VC98/Include/MALLOC.H
     */
    void *(*rwmalloc)(size_t size);                   /**< rwmalloc   malloc */
    void  (*rwfree)(void *mem);                       /**< rwfree     free */
    void *(*rwrealloc)(void *mem, size_t newSize);    /**< rwrealloc  realloc */
    void *(*rwcalloc)(size_t numObj, size_t sizeObj); /**< calloc     calloc */
};

typedef struct RwFreeBlock RwFreeBlock;
/*
 * Freelists -- from Page 131
 * Advanced Animation and Rendering Techniques
 * Alan Watt and Mark Watt
 * Addison-Wesley 1993,
 * ISBN 0-201-54412-1:
 *
 * "Lastly, on a more general note concerning speedups for renderers, the
 *  implementor should be aware that a lot of suggestions for improving
 *  efficiency fall into the category of ingenious, but complex,
 *  algorithms for very specific contexts that may save a few microseconds
 *  but which make your code unreadable.  A more general computer science
 *  perspective that takes a `global view' of the renderer can be more
 *  fruitful.  For example, the renderer devotes a lot of time to
 *  allocating and deallocating chunks of memory for storing data. A lot
 *  of these chunks are always the same size - such as those that are
 *  continually required to store the data structure for fragment lists.
 *  Using memory management techniques that recognize this fact can yield
 *  considerable dividends.  One such scheme would be to hold a series of
 *  empty lists in memory for all the commonly used data structures.  An
 *  empty list for fragments, say, would contain a list of previously
 *  allocated, but no longer needed, fragment structures.  When the
 *  renderer needs memory for a new fragment, it looks first at this empty
 *  list.  If there is nothing there it allocates space directly,
 *  otherwise it takes a fragments off the end of the list and uses that.
 *  Conversely, when the renderer no longer needs a fragment, instead of
 *  freeing it, it goes onto the end of the empty list.  In the authors'
 *  experience, replacing the naive allocate/deallocate scheme with this
 *  way of managing memory can result in 100% speedup. "
 */
struct RwFreeBlock
{
    RwFreeBlock        *nextBlock;
};

typedef struct RwFreeList RwFreeList;
struct RwFreeList
{
    void              **freeListStack; /* Stack of unused entries */
    void              **freeListStackTop; /* Pointer to the top of the stack */

    RwFreeBlock        *firstBlock; /* Data start */

    RwInt32             blockSize; /* Size of block in bytes */
    RwInt32             entrySize; /* Entry size */
    RwInt32             alignmentMinusOne; /* Entry alignment minus 1 */
    RwInt32             entriesPerBlock; /* Amount of space in a block */

    RwInt32             entriesAllocated; /* Total slots allocated
                                           * (but not necessarily being used */

    /* All freelists */
    RwLLLink            lFreeList;

    /* TODO: RwFreeList (size: 40 bytes) */
};

/**
 * \ingroup datatypes
 * \ref RwFreeListCallBack represents
 * the function called from \ref RwFreeListForAllUsed for all used entries in a
 * given free list.
 *
 * \param  pMem   Pointer to the start of the current entries.
 *
 * \param  pData   User-defined data pointer.
 *
 * \see RwFreeListForAllUsed
 *
 */
typedef void        (*RwFreeListCallBack) (void *pMem, void *pData);
typedef void       *(*RwMemoryAllocFn)    (RwFreeList * fl);
typedef RwFreeList *(*RwMemoryFreeFn)     (RwFreeList * fl, void *pData);

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */





extern RwFreeList  *RwFreeListCreate(RwInt32 entrySize,
                                     RwInt32 entriesPerBlock,
                                     RwInt32 alignment);


extern RwBool       RwFreeListDestroy(RwFreeList * freelist);
/* RWPUBLICEND */

extern void        *_rwFreeListAllocReal(RwFreeList * freelist);
extern RwFreeList  *_rwFreeListFreeReal(RwFreeList * freelist,
                                       void *pData);

/* RWPUBLIC */

extern RwFreeList  *RwFreeListForAllUsed(RwFreeList * freelist,
                                         RwFreeListCallBack
                                         fpCallBack, void *pData);

/* RWPUBLICEND */

/* Opening/Closing */
extern void         _rwMemoryClose(void);
extern RwBool       _rwMemoryOpen(RwMemoryFunctions * memFuncs);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#if (defined(RWDEBUG) && defined(RWNOFREELISTS) && !defined(RWKEEPFREELISTS))



#define RwFreeListAlloc(_f)     RwMalloc((_f)->entrySize)



#endif                          /* (defined(RWDEBUG) && defined(RWNOFREELISTS) && !defined(RWKEEPFREELISTS)) */

#if (!defined(RwFreeListAlloc))
#define RwFreeListAlloc(_f)     RWSRCGLOBAL(memoryAlloc)(_f)
#endif                          /* (!defined(RwFreeListAlloc)) */

#if (!defined(RwFreeListFree))
#define RwFreeListFree(_f, _p)  RWSRCGLOBAL(memoryFree)(_f, _p)
#endif                          /* (!defined(RwFreeListFree)) */

/* RWPUBLICEND */

#endif                          /* RWMEMORY_H */

