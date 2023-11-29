/******************************************/
/*                                        */
/*    RenderWare(TM) Graphics Library     */
/*                                        */
/******************************************/

/*
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * or Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. and Canon Inc. will not, under any
 * circumstances, be liable for any lost revenue or other damages
 * arising from the use of this file.
 *
 * Copyright (c) 1999. Criterion Software Ltd.
 * All Rights Reserved.
 */

/*************************************************************************
 *
 * Filename: <D:\Repositories\Caper\src\rwsdk\include\d3d8\rwplcore.h>
 * Automatically Generated on: Wed Nov 29 19:14:49 2023
 *
 ************************************************************************/

#ifndef RWPLCORE_H
#define RWPLCORE_H

/*--- System Header Files ---*/
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\os/win/ostypes.h ---*/
#ifndef WIN_OSTYPES_H
#define WIN_OSTYPES_H

typedef int  RwInt32;
typedef unsigned int RwUInt32;

typedef unsigned char RwUInt8;

#ifdef RWUNICODE
typedef wchar_t RwChar;
#else /* RWUNICODE */
typedef char RwChar;
#endif /* RWUNICODE */
typedef float RwReal;
typedef RwInt32 RwBool;



/* Structure alignment */
#define RWALIGN(type, x)   type /* nothing */
#define rwMATRIXALIGNMENT sizeof(RwUInt32)
#define rwFRAMEALIGNMENT sizeof(RwUInt32)
#define rwCAMERAALIGNMENT sizeof(RwUInt32)


#endif /* WIN_OSTYPES_H */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bamath.h ---*/

#if (!defined(RwPow))
#define RwPow(_x, _y)           pow(_x, _y)
#endif /* (!defined(RwPow)) */



#if (!defined(rwSqrt))
#define rwSqrt(_x)              sqrt(_x)
#endif /* (!defined(rwSqrt)) */




/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/batypes.h ---*/










/****************************************************************************
 Attributes
 */

#if (!defined(__RWUNUSED__))
#define __RWUNUSED__           /* No op */
#endif /* (!defined(__RWUNUSED__)) */


/****************************************************************************
 Calling conventions
 */

#if (!defined(RWASMCALL))
#define RWASMCALL              /* No op */
#endif /* (!defined(RWASMCALL)) */

#if (!defined(RWASMAPI))
#define RWASMAPI(TYPE) TYPE
#endif /* (!defined(RWASMAPI)) */



/****************************************************************************
 Macro wrappers. These are needed everywhere.
 */

#ifndef MACRO_START
#define MACRO_START do
#endif /* MACRO_START */

#ifndef MACRO_STOP
#define MACRO_STOP while(0)
#endif /* MACRO_STOP */

/****************************************************************************
 Types needed everywhere
 */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE !FALSE








/*******************/
/* Primitive types */
/*******************/

/* String construction stuff (gets us UNICODE support) */
#ifdef RWUNICODE
#define _RWSTRING(x) L ## x
#else /* RWUNICODE */
#define _RWSTRING(x) x
#endif /* RWUNICODE */
#define RWSTRING(x) _RWSTRING(x)

/* NB volatile keyword required for VC5.0 to ensure a reload - AMB */
typedef union RwSplitBits RwSplitBits;
union RwSplitBits
{
    RwReal nReal;
    volatile RwInt32 nInt;
    volatile RwUInt32 nUInt;
};













/*****************/

/* Complex types */

/*****************/

/**
 * \ingroup datatypes
 * \typedef RwV2d
 * typedef for struct RwV2d
 */
typedef struct RwV2d RwV2d;
/**
 * \ingroup datatypes
 * \struct RwV2d
 * This type represents points in a 2D space, such as device
 * space, specified by the (x, y) coordinates of the point.
 */
struct RwV2d
{
    RwReal x;   /**< X value*/
    RwReal y;   /**< Y vlaue */
};

/**
 * \ingroup datatypes
 * \typedef RwV3d
 * typedef for struct RwV3d
 */
typedef struct RwV3d RwV3d;
/**
 * \ingroup datatypes
 * \struct RwV3d
 *  This type represents 3D points and vectors specified by
 * the (x, y, z) coordinates of a 3D point or the (x, y, z) components of a
 * 3D vector.
 */
struct RwV3d
{
    RwReal x;   /**< X value */
    RwReal y;   /**< Y value */
    RwReal z;   /**< Z value */
};


/**
 * \ingroup datatypes
 * \typedef RwRect
 * typedef for struct RwRect
 */
typedef struct RwRect RwRect;
/**
 * \ingroup datatypes
 * \struct RwRect
 * This type represents a 2D device space rectangle specified
 * by the position of the top-left corner (the offset x, y) and its width (w)
 * and height (h).
 */
struct RwRect
{
    char _gap0;
    // TODO: RwRect
};




/* Doubly linked list. End marked as start (its a ring) */

typedef struct RwLLLink  RwLLLink;                     /*** RwLLLink ***/

#if (!defined(DOXYGEN))
/**
 * \ingroup fundtypesdatatypes
 * \struct RwLLLink
 * RwLLLink is an internal two way linked list pointer.  It contains 
 * links to the previous and next RwLLLink's.  It is usually used in a 
 * ring list fashion.
 */
struct RwLLLink
{
    RwLLLink *next;
    RwLLLink *prev;
};
#endif /* (!defined(DOXYGEN)) */


#define rwLLLinkGetData(linkvar,type,entry)                             \
    ((type *)(((RwUInt8 *)(linkvar))-offsetof(type,entry)))
#define rwLLLinkGetNext(linkvar)                                        \
    ((linkvar)->next)



typedef struct RwLinkList RwLinkList;

#if (!defined(DOXYGEN))
/**
 * \ingroup fundtypesdatatypes
 * \struct RwLLLink
 * Both the next and previous directions of an RwLinkList are linked in a 
 * ring list.  That is, to iterate over all the elements of this structure 
 * store the starting link and iterate over the elements until 
 * returning back to this first element. 
 */
struct RwLinkList
{
    RwLLLink link;
};
#endif /* (!defined(DOXYGEN)) */

#define rwLinkListInitialize(list)                                      \
    MACRO_START                                                         \
    {                                                                   \
        (list)->link.next = ((RwLLLink *)(list));                       \
        (list)->link.prev = ((RwLLLink *)(list));                       \
    }                                                                   \
    MACRO_STOP


#define rwLinkListAddLLLink(list, linkvar)                              \
    MACRO_START                                                         \
    {                                                                   \
        (linkvar)->next = (list)->link.next;                            \
        (linkvar)->prev = (&(list)->link);                              \
        ((list)->link.next)->prev = (linkvar);                          \
        (list)->link.next = (linkvar);                                  \
    }                                                                   \
    MACRO_STOP
#define rwLinkListRemoveLLLink(linkvar)                                 \
    MACRO_START                                                         \
    {                                                                   \
        ((linkvar)->prev)->next = (linkvar)->next;                      \
        ((linkvar)->next)->prev = (linkvar)->prev;                      \
    }                                                                   \
    MACRO_STOP
#define rwLinkListGetFirstLLLink(list)                                  \
    ((list)->link.next)
#define rwLinkListGetLastLLLink(list)                                   \
    ((list)->link.prev)
#define rwLinkListGetTerminator(list)                                   \
    (&((list)->link))




/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/batype.h ---*/

#define MAKECHUNKID(vendorID, chunkID) (((vendorID & 0xFFFFFF) << 8) | (chunkID & 0xFF))

enum RwPluginVendor
{
    rwVENDORID_CRITERIONINT     = 0x000004L,
};
typedef enum RwPluginVendor RwPluginVendor;


/***
 *** These are the Criterion internal plugin extensions.  Use with rwVENDORID_CRITERIONINT.
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***/

enum RwCriterionPluginID
{
    rwID_COREPLUGIN = 0x01,
};
typedef enum RwCriterionPluginID RwCriterionPluginID;


typedef struct RwObject RwObject;
/* CAPER: RwObject size is unknown, only we know is type at offset 0
    RwObject is 8 bytes according to RwTexDictionary */
struct RwObject
{
    RwUInt8 type;                /**< Internal Use */
    RwUInt8 subType;             /**< Internal Use */
    RwUInt8 flags;               /**< Internal Use */
    RwUInt8 privateFlags;        /**< Internal Use */
    void   *parent;              /**< Internal Use */
                                 /* Often a Frame  */
};




/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */



#define rwObjectInitialize(o, t, s)                     \
MACRO_START                                             \
{                                                       \
    ((RwObject *)(o))->type = (RwUInt8)(t);             \
    ((RwObject *)(o))->subType = (RwUInt8)(s);          \
    ((RwObject *)(o))->flags = 0;                       \
    ((RwObject *)(o))->privateFlags = 0;                \
    ((RwObject *)(o))->parent = NULL;                   \
}                                                       \
MACRO_STOP


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\os/win/osintf.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/rwdbgerr.h ---*/
#define RWECODE(a,b) a,

/* Construct an enum type with all the plugin error codes (for the app to use) */
enum RwErrorCodePlugin_errcore
{
#include "errcore.def"
};
typedef enum RwErrorCodePlugin_errcore RwErrorCodePlugin_errcore;

#undef RWECODE



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/resmem.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bamemory.h ---*/

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

extern RwFreeList  *RwFreeListForAllUsed(RwFreeList * freelist,
                                         RwFreeListCallBack
                                         fpCallBack, void *pData);

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



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bastream.h ---*/

/****************************************************************************
 Global Types
 */

/**
 * \ingroup datatypes
 * \ref RwStreamType 
 * This type represents the different types of stream that 
 * can be used. 
 * See API section \ref rwstream 
 */
enum RwStreamType
{
    RwStreamType_pad0,

    rwSTREAMFILE,       /**<File */
    rwSTREAMFILENAME,   /**<File name */
    rwSTREAMMEMORY,     /**<Memory*/
    rwSTREAMCUSTOM,     /**<Custom */

    /* TODO: RwStreamType */
};
typedef enum RwStreamType RwStreamType;

/**
 * \ingroup datatypes
 * \ref RwStreamAccessType 
 * This type represents the options available for 
 * accessing a stream when it is opened.
 * See API section \ref rwstream */
enum RwStreamAccessType
{
    RwStreamAccessType_pad0,

    rwSTREAMREAD,           /**<Read */
    rwSTREAMWRITE,          /**<Write */
    rwSTREAMAPPEND,         /**<Append */

    /* TODO: RwStreamAccessType */
};
typedef enum RwStreamAccessType RwStreamAccessType;





typedef struct RwStreamMemory RwStreamMemory;

struct RwStreamMemory
{
    RwUInt32            position; /* Current 'memory' position 0 is first byte */
    RwUInt32            nSize;  /* Space allocated currently */
    RwUInt8            *memBlock; /* Current memory block pointer */
};


typedef union RwStreamFile RwStreamFile;

union RwStreamFile
{
    void    *fpFile;               /**< file pointer */
    const void    *constfpFile;    /**< const file pointer */
};


/* Custom stream function pointer types */
typedef             RwBool(*rwCustomStreamFnClose) (void *data);
typedef             RwUInt32(*rwCustomStreamFnRead) (void *data, void *buffer,
                                                     RwUInt32 length);
typedef             RwBool(*rwCustomStreamFnWrite) (void *data,
                                                    const void *buffer,
                                                    RwUInt32 length);
typedef             RwBool(*rwCustomStreamFnSkip) (void *data,
                                                   RwUInt32 offset);

/* Custom stream */

typedef struct RwStreamCustom RwStreamCustom;

struct RwStreamCustom
{
    rwCustomStreamFnClose sfnclose;
    rwCustomStreamFnRead sfnread;
    rwCustomStreamFnWrite sfnwrite;
    rwCustomStreamFnSkip sfnskip;
    void               *data;
};





typedef union RwStreamUnion RwStreamUnion;

union RwStreamUnion
{
    RwStreamMemory      memory; /**< memory */
    RwStreamFile        file; /**< file */
    RwStreamCustom      custom; /**< custom */
};




/**
 * \ingroup datatypes
 * \typedef RwStream 
 * Binary stream for reading or writing object data. 
 * This should be considered an opaque type.
 * Use the RwStream API functions to access.
 */
typedef struct RwStream RwStream;
struct RwStream
{
    RwStreamType        type;
    RwStreamAccessType  accessType;

    char _pad0[4];

    RwStreamUnion       Type;
    RwBool              rwOwned;

    /* TODO: RwStream (size=36 bytes) */
};



typedef struct RwMemory RwMemory;

struct RwMemory
{
    RwUInt8     *start; /**< starting address */
    RwUInt32    length; /**< length in bytes*/
};


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Open/Close streams */

extern RwStream *
_rwStreamInitialize(RwStream *stream,
                    RwBool rwOwned,
                    RwStreamType type,
                    RwStreamAccessType accessType,
                    const void *pData);

extern RwStream *
RwStreamOpen(RwStreamType type,
             RwStreamAccessType accessType,
             const void *pData);

extern RwBool       
RwStreamClose(RwStream * stream,
              void *pData);


/* Stream read/write */
extern RwUInt32     
RwStreamRead(RwStream * stream,
             void *buffer,
             RwUInt32 length);

extern RwStream *
RwStreamWrite(RwStream * stream,
              const void *buffer,
              RwUInt32 length);


/* Stream skipping */
extern RwStream *
RwStreamSkip(RwStream * stream,
             RwUInt32 offset);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/rwstring.h ---*/

/****************************************************************************
 Defines
 */


#define rwstrcpy    RWSRCGLOBAL(stringFuncs).vecStrcpy
#define rwstrchr    RWSRCGLOBAL(stringFuncs).vecStrchr
#define rwstrlen    RWSRCGLOBAL(stringFuncs).vecStrlen


/****************************************************************************
 Global Types
 */

typedef int (*vecSprintfFunc)(RwChar *buffer,
                              const RwChar *format,
                              ...) /* __RWFORMAT__(printf, 2, 3) */; 
typedef int (*vecVsprintfFunc)(RwChar *buffer,
                               const RwChar *format,
                               va_list argptr); 
typedef RwChar *(*vecStrcpyFunc)(RwChar *dest,
                                 const RwChar *srce);
typedef RwChar *(*vecStrncpyFunc)(RwChar *dest,
                                  const RwChar *srce,
                                  size_t size);
typedef RwChar *(*vecStrcatFunc)(RwChar *dest,
                                 const RwChar *srce);
typedef RwChar *(*vecStrncatFunc)(RwChar *dest,
                                  const RwChar *srce,
                                  size_t size);
typedef RwChar *(*vecStrrchrFunc)(const RwChar *string,
                                  int findThis);
typedef RwChar *(*vecStrchrFunc)(const RwChar *string,
                                 int findThis);
typedef RwChar *(*vecStrstrFunc)(const RwChar *string,
                                 const RwChar *findThis);
typedef int (*vecStrcmpFunc)(const RwChar *string1,
                             const RwChar *string2);
typedef int (*vecStricmpFunc)(const RwChar *string1,
                              const RwChar *string2);
typedef size_t (*vecStrlenFunc)(const RwChar *string);
typedef RwChar *(*vecStruprFunc)(RwChar *string);
typedef RwChar *(*vecStrlwrFunc)(RwChar *string);
typedef RwChar *(*vecStrtokFunc)(RwChar *string, const RwChar *delimit);
typedef int (*vecSscanfFunc)(const RwChar *buffer,
                             const RwChar *format,
                             ...) /* __RWFORMAT__(scanf, 2, 3) */;

typedef struct RwStringFunctions RwStringFunctions;
struct RwStringFunctions
{
    vecSprintfFunc vecSprintf ;
    vecVsprintfFunc vecVsprintf;
    vecStrcpyFunc vecStrcpy;
    vecStrncpyFunc vecStrncpy;
    vecStrcatFunc vecStrcat;
    vecStrncatFunc vecStrncat;
    vecStrrchrFunc vecStrrchr;
    vecStrchrFunc vecStrchr;
    vecStrstrFunc vecStrstr;
    vecStrcmpFunc vecStrcmp;
    vecStricmpFunc vecStricmp;
    vecStrlenFunc vecStrlen;
    vecStruprFunc vecStrupr;
    vecStrlwrFunc vecStrlwr;
    vecStrtokFunc vecStrtok;
    vecSscanfFunc vecSscanf;
};



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/rwgrp.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/batkreg.h ---*/

/**
 * \ingroup datatypes
 * \ref RwPluginObjectConstructor represents the callback
 * registered by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc.
 * as the function that initializes either the global extension data (in the
 * case of \ref RwEngineRegisterPlugin) or the object extension data (in all
 * other cases). Registered by \ref RwCameraSetStreamAlwaysCallBack, etc.
 * 
 * \param  object   Pointer to the object (global or otherwise)
 * that contains the extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the
 * byte offset of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object
 */
typedef void *(*RwPluginObjectConstructor)(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);

/**
 * \ingroup datatypes
 * \ref RwPluginObjectCopy represents the callback registered by
 * \ref RwCameraRegisterPlugin, etc. as the function that copies the object
 * extension data when an object is duplicated.
 * 
 * \param  dstObject   Pointer to the destination object that will
 * receive the extension data.
 *
 * \param  srcObject   Pointer to the source object containing
 * extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the byte offset
 * of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object
 */
typedef void *(*RwPluginObjectCopy)(void *dstObject, const void *srcObject, RwInt32 offsetInObject, RwInt32 sizeInObject);

/**
 * \ingroup datatypes
 * \ref RwPluginObjectDestructor represents the callback registered
 * by \ref RwEngineRegisterPlugin, \ref RwCameraRegisterPlugin, etc. as the
 * function that destroys either the global extension data (in the case of
 * \ref RwEngineRegisterPlugin) or the object extension data (in all other
 * cases).
 * 
 * \param  object   Pointer to the object (global or otherwise)
 * containing the extension data.
 *
 * \param  offsetInObject   A RwInt32 value equal to the byte
 * offset of the extension data in the object.
 *
 * \param  sizeInObject   A RwInt32 value equal to the size
 * (in bytes) of the extension data.
 *
 * \return Pointer to the object.
 */
typedef void *(*RwPluginObjectDestructor)(void *object, RwInt32 offsetInObject, RwInt32 sizeInObject);



typedef struct RwPluginRegistry RwPluginRegistry;
typedef struct RwPluginRegEntry RwPluginRegEntry;

struct RwPluginRegistry
{
	RwInt32          sizeOfStruct;
	RwInt32          origSizeOfStruct;
	RwInt32          maxSizeOfStruct;

	char _pad8[4];

	RwPluginRegEntry *firstRegEntry;
    RwPluginRegEntry *lastRegEntry;

	/* TODO: RwPluginRegistry (size: ? bytes) */
};

struct RwPluginRegEntry
{
	RwInt32         offset;
	RwInt32         size;
	RwUInt32        pluginID;

	char _pad[20];
	/* TODO: RwPluginRegEntry (size: 60 bytes) */

	RwPluginObjectConstructor constructCB;
	RwPluginObjectDestructor destructCB;
	RwPluginObjectCopy copyCB;

	char _pad1[4];

	RwPluginRegEntry *nextRegEntry;
	RwPluginRegEntry *prevRegEntry;
	RwPluginRegistry *parentRegistry;
};



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern          "C"
{
#endif         /* __cplusplus */


extern RwInt32
_rwPluginRegistryAddPlugin(RwPluginRegistry * reg,
                           RwInt32 size,
                           RwUInt32 pluginID,
                           RwPluginObjectConstructor constructCB,
                           RwPluginObjectDestructor destructCB,
                           RwPluginObjectCopy copyCB);
extern RwInt32
_rwPluginRegistryGetPluginOffset(const RwPluginRegistry *reg, 
                                RwUInt32 pluginID);

/* Initializing/De-initializing instances */
extern const
RwPluginRegistry *_rwPluginRegistryInitObject(const RwPluginRegistry * reg, 
                                             void *object);
extern const
RwPluginRegistry *_rwPluginRegistryDeInitObject(const RwPluginRegistry * reg, 
                                               void *object);
extern const
RwPluginRegistry *_rwPluginRegistryCopyObject(const RwPluginRegistry * reg,
                                             void *dstObject, 
                                             const void *srcObject);



#ifdef    __cplusplus
}
#endif         /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/batkbin.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bamatrix.h ---*/

/****************************************************************************
 Defines
 */

#define RWMATRIXALIGNMENT(_mat) \
   (! (((rwMATRIXALIGNMENT)-1) & ((RwUInt32)(_mat))))


 /* Internal flags (bits 16-31) */
enum RwMatrixType
{
    rwMATRIXTYPEORTHONORMAL = 0x00000003,
};
typedef enum RwMatrixType RwMatrixType;

enum RwMatrixFlag
{
    rwMATRIXINTERNALIDENTITY = 0x00020000,
};
typedef enum RwMatrixFlag RwMatrixFlag;

/* Flags describing what will optimize for */
enum RwMatrixOptimizations
{
    rwMATRIXOPTIMIZE_IDENTITY = 0x00020000,
};
typedef enum RwMatrixOptimizations RwMatrixOptimizations;

/****************************************************************************
 Global Types
 */


struct RwMatrix
{
    /* These are padded to be 16 byte quantities per line */
    RwV3d               right;
    RwUInt32            flags;
    RwV3d               up;
    RwUInt32            pad1;
    RwV3d               at;
    RwUInt32            pad2;
    RwV3d               pos;
    RwUInt32            pad3;
};


/**
 * \ingroup datatypes
 * \typedef RwMatrix 
 * Matrix to define transformations. 
 * This should be considered an opaque type.
 * Use the RwMatrix API functions to access.
 */
typedef struct RwMatrix RWALIGN(RwMatrix, rwMATRIXALIGNMENT);






#if (!defined(RwMatrixSetIdentityMacro))
#define RwMatrixSetIdentityMacro(m)                                     \
MACRO_START                                                             \
{                                                                       \
    (m)->right.x = (m)->up.y    = (m)->at.z    = (RwReal)((1.0));       \
    (m)->right.y = (m)->right.z = (m)->up.x    = (RwReal)((0.0));       \
    (m)->up.z    = (m)->at.x    = (m)->at.y    = (RwReal)((0.0));       \
    (m)->pos.x   = (m)->pos.y   = (m)->pos.z   = (RwReal)((0.0));       \
    rwMatrixSetFlags((m),                                               \
                     rwMatrixGetFlags(m) |                              \
                     (rwMATRIXINTERNALIDENTITY |                        \
                      rwMATRIXTYPEORTHONORMAL));                        \
}                                                                       \
MACRO_STOP
#endif /* (!defined(RwMatrixSetIdentityMacro)) */

typedef void        (RWASMCALL * rwMatrixMultFn) (RwMatrix * dstMat,
                                                  const RwMatrix * matA,
                                                  const RwMatrix *
                                                  matB);




typedef struct RwMatrixTolerance RwMatrixTolerance;
struct RwMatrixTolerance
{
    RwReal              Normal;
        /**< Tolerance within which matrix is deemed to be normal   */
    RwReal              Orthogonal;
        /**< Tolerance within which matrix is deemed to be orthogonal */
    RwReal              Identity;
        /**< Tolerance within which matrix is deemed to be identity */
};





#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwBool
RwEngineSetMatrixTolerances(const RwMatrixTolerance * const tolerance);

/* Update */
#define rwMatrixSetFlags(m, flagsbit)     ((m)->flags = (flagsbit))
#define rwMatrixGetFlags(m)               ((m)->flags)

#define RwMatrixSetIdentity(m)   RwMatrixSetIdentityMacro(m)

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */





/* Initialisation/deinitialisation */
#define rwMatrixInitialize(m, t)                \
MACRO_START                                     \
{                                               \
    rwMatrixSetFlags((m), (t));                 \
}                                               \
MACRO_STOP






/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\driver/d3d8/drvmodel.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bavector.h ---*/

/*
 * Typedef for pointer to Vector multiplication by Matrix function
 */

typedef RwV3d *(*rwVectorMultFn) (RwV3d * pointsOut, 
                                  const RwV3d * pointsIn,
                                  RwInt32 numPoints,
                                  const RwMatrix * matrix);








/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif         /* __cplusplus */

#ifdef    __cplusplus
}
#endif         /* __cplusplus */




/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/balist.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bafsys.h ---*/

/****************************************************************************
 Global Types
 */

/*
 * \typedef rwFnFexist
 * Returns true if file with given name exists, false if it doesn't.
 */
typedef RwBool  (*rwFnFexist)(const RwChar *name);

/*
 * \typedef rwFnFopen
 * Mimics ANSI C Standard Library fopen.
 */
typedef void   *(*rwFnFopen)(const RwChar *name, const RwChar *access);

/*
 * \typedef rwFnFclose
 * Mimics ANSI C Standard Library fclose.
 */
typedef int     (*rwFnFclose)(void *fptr);

/*
 * \typedef rwFnFread
 * Mimics ANSI C Standard Library fread.
 */
typedef size_t  (*rwFnFread)(void *addr, size_t size, size_t count, void *fptr);

/*
 * \typedef rwFnFwrite
 * Mimics ANSI C Standard Library fwrite.
 */
typedef size_t  (*rwFnFwrite)(const void *addr, size_t size, size_t count, void *fptr);

/*
 * \typedef rwFnFgets
 * Mimics ANSI C Standard Library fgets.
 */
typedef RwChar *(*rwFnFgets)(RwChar *buffer, int maxLen, void *fptr);

/*
 * \typedef rwFnFputs
 * Mimics ANSI C Standard Library fputs.
 */
typedef int     (*rwFnFputs)(const RwChar *buffer, void *fptr);

/*
 * \typedef rwFnFeof
 * Mimics ANSI C Standard Library feof.
 */
typedef int     (*rwFnFeof)(void *fptr);

/*
 * \typedef rwFnFseek
 * Mimics ANSI C Standard Library fseek.
 */
typedef int     (*rwFnFseek)(void *fptr, long offset, int origin);

/*
 * \typedef rwFnFflush
 * Mimics ANSI C Standard Library fflush.
 */
typedef int     (*rwFnFflush)(void *fptr);

/*
 * \typedef rwFnFtell
 * Mimics ANSI C Standard Library ftell.
 */
typedef int     (*rwFnFtell)(void *fptr);


/**
 * \ingroup datatypes
 * \typedef RwFileFunctions
 * typedef for struct RwFileFunctions
 */
typedef struct RwFileFunctions RwFileFunctions;
/**
 * \ingroup datatypes
 * \struct RwFileFunctions
 * This type is used to specify the file access
 * functions used by RenderWare. The default file system uses the standard
 * ANSI functions. The application may install an alternative file system
 * providing it complies with the ANSI interface -- see API 
 * function \ref RwOsGetFileInterface.
 *
 * The function types associated with this type are defined as follows 
 *
 * \verbatim
   RwBool (*rwFnFexist)(const RwChar *name)
       void *(*rwFnFopen)(const RwChar *name, const RwChar *access) 
       int (*rwFnFclose)(void *fptr)
       size_t (*rwFnFread)(void *addr, size_t size, size_t count, void *fptr)                        
       size_t (*rwFnFwrite)(const void *addr, size_t size, size_t count,
                 void *fptr)
   RwChar *(*rwFnFgets)(RwChar *buffer, int maxLen, void *fptr)
       int (*rwFnFputs)(const RwChar *buffer, void *fptr)
       int (*rwFnFeof)(void *fptr)
       int (*rwFnFseek)(void *fptr, long offset, int origin)
       int (*rwFnFflush)(void *fptr)
       int (*rwFnFtell)(void *fptr)
 \endverbatim
 * Note the function argument lists are consistent with the ANSI
 * standard file access interface:
 */
struct RwFileFunctions
{
    rwFnFexist  rwfexist; /**< Pointer to fexist function */
    rwFnFopen   rwfopen;  /**< Pointer to fopen function */
    rwFnFclose  rwfclose; /**< Pointer to fclose function */
    rwFnFread   rwfread;  /**< Pointer to fread function */
    rwFnFwrite  rwfwrite; /**< Pointer to fwrite function */
    rwFnFgets   rwfgets;  /**< Pointer to fgets function */
    rwFnFputs   rwfputs;  /**< Pointer to puts function */
    rwFnFeof    rwfeof;   /**< Pointer to feof function */
    rwFnFseek   rwfseek;  /**< Pointer to fseek function */
    rwFnFflush  rwfflush; /**< Pointer to fflush function */
    rwFnFtell   rwftell;  /**< Pointer to ftell function */
};


#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */



#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/baimmedi.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/badevice.h ---*/

/****************************************************************************
 Global Types
 */

typedef struct RwEngineOpenParams RwEngineOpenParams;

/**
 * \ingroup datatypes
 * \struct RwEngineOpenParams
 * This type is used to specify device dependent parameters 
 * for use by the API function \ref RwEngineOpen.  
 * For a Windows application the displayID field 
 * should be set to the window's handle (of type HWND).  
 * For NULL and sky libraries displayID=0:
 */
struct RwEngineOpenParams
{
    void    *displayID;     /**< Display Identifier */
};





/**
 * \ingroup datatypes
 * RwEngineInitFlag 
 * Engine initialization flags.  An application can use
 * these to control the memory manager that RenderWare uses for dynamic
 * memory management.  By default RenderWare uses FreeLists.  This is the
 * preferred way of using RenderWare.  If the application does not want
 * RenderWare to use the memory manager, then the application can pass
 * rwENGINEINITNOFREELISTS as the argument to \ref RwEngineInit and
 * RenderWare will replace freelist calls to corresponding calls to
 * RwMalloc and RwFree.  This will result in more memory management
 * related calls.
 */
enum RwEngineInitFlag
{
    RwEngineInitFlag_0, /* TODO: Rename RwEngineInitFlag_0 */
    rwENGINEINITNOFREELISTS = 0x1,  /**<Don't use Freelists */
};
typedef enum RwEngineInitFlag RwEngineInitFlag;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */




/* Sequence of events to get RenderWare up and running */
extern RwBool RwEngineInit(RwMemoryFunctions *memFuncs,
                           RwUInt32 initFlags,
                           RwUInt32 resArenaSize);
extern RwInt32 RwEngineRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                  RwPluginObjectConstructor initCB,
                                  RwPluginObjectDestructor termCB);
extern RwInt32 RwEngineGetPluginOffset(RwUInt32 pluginID);
extern RwBool RwEngineOpen(RwEngineOpenParams *initParams);
extern RwBool RwEngineStart(void);
extern RwBool RwEngineStop(void);
extern RwBool RwEngineClose(void);
extern RwBool RwEngineTerm(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/baerr.h ---*/
/****************************************************************************
 Global Types
 */

typedef struct RwError RwError;
/**
 * \ingroup datatypes
 * \struct RwError
 * This type represents a RenderWare error specified by the
 * ID of the plugin that the error was issued from (pluginID) and the error
 * code itself (errorCode) (see API function \ref RwErrorGet).
 * \param pluginID The ID of the plugin that issued the error.
 * \param errorCode A value representing the error code.
 */
struct RwError
{
    RwInt32     pluginID;  /**< Internal Use */
    RwInt32     errorCode; /**< Internal Use */
};

#define RWECODE(a,b) a,

/* common errors have the MSB set */

enum RwErrorCodeCommon
{
    E_RW_NOERROR = (int)0x80000000L,
#include "errcom.def"
};
typedef enum RwErrorCodeCommon RwErrorCodeCommon;

#undef RWECODE

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */


extern RwError *RwErrorSet(RwError *code);
extern RwInt32 _rwerror(RwInt32 code, ...);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/badebug.h ---*/


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/balibtyp.h ---*/

/****************************************************************************
 Defines
 */

/* macro used to access plugin data in objects */
#define RWPLUGINOFFSET(type, base, offset) \
   ((type *)((RwUInt8 *)base + offset))



/* macro used to access global data structure (the root type is RwGlobals) */
#define RWSRCGLOBAL(variable) \
   (((RwGlobals *)RwEngineInstance)->variable)

#define RWASSERTISTYPE(_f, _t) \
   RWASSERT((((const RwObject *)(_f))->type)==(_t))

/****************************************************************************
 Global Types
 */

enum RwEngineStatus
{
    rwENGINESTATUSIDLE = 0,                     /* This needs to be zero */
    rwENGINESTATUSINITED = 1,
};
typedef enum RwEngineStatus RwEngineStatus;

typedef struct RwGlobals RwGlobals;
struct RwGlobals
{
        /* TODO: RwGlobals (size: 340 bytes) */
        char _pad1[188];

        /* All of the frames which have been updated */
        RwLinkList          dirtyFrameList;

        /* The file functions */
        RwFileFunctions     fileFuncs;

        /* The string functions */
        RwStringFunctions   stringFuncs;

        /* The memory allocation functions */
        RwMemoryFunctions   memoryFuncs;

        /* virtual memory alloc/free functions */
        RwMemoryAllocFn         memoryAlloc;
        RwMemoryFreeFn          memoryFree;

        char _pad2[4];

        /* Current engine status */
        RwEngineStatus      engineStatus;

        /* Resource arena init size. */
        RwUInt32            resArenaInitSize;
};

typedef struct RwModuleInfo RwModuleInfo;
struct RwModuleInfo
{
        RwInt32     globalsOffset;
        RwInt32     numInstances;
};



/****************************************************************************
 Program wide globals
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifdef RWGLOBALSIZE

#else /* RWGLOBALSIZE */
extern void         *RwEngineInstance;
#endif /* RWGLOBALSIZE */



#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/baresour.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/bacolor.h ---*/
/****************************************************************************
 Global Types
 */

typedef struct RwRGBA RwRGBA;
/**
 * \ingroup datatypes
 * \struct RwRGBA
 * This structure represents a RGBA color 
 * which has integer components specified in the range 0 to 255. */
struct RwRGBA
{
    RwUInt8 field_0;
    RwUInt8 field_1;
    RwUInt8 field_2;
    RwUInt8 field_3;

    // TODO: struct RwRGBA
};


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */



/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/babinmtx.h ---*/


/*--- Automatically derived from: D:\Repositories\Caper\src\rwsdk\src/plcore/babinary.h ---*/

#endif /* RWPLCORE_H */
