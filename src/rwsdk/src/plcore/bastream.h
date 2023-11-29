/***************************************************************************
 * Module  : bastream.h                                                    *
 *                                                                         *
 * Purpose : Stream handling                                               *
 *                                                                         *
 **************************************************************************/

#ifndef RWSTREAM_H
#define RWSTREAM_H

/* RWPUBLIC */

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

/* RWPUBLICEND */

typedef struct rwStreamGlobals rwStreamGlobals;
struct rwStreamGlobals
{
    RwFreeList         *streamFreeList;
};

/* RWPUBLIC */

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

/* RWPUBLICEND */

/* Opening and closing stream module */
extern void *
_rwStreamModuleOpen(void *instance,
                    RwInt32 offset,
                    RwInt32 size);

extern void *
_rwStreamModuleClose(void *instance,
                     RwInt32 offset,
                     RwInt32 size);


/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif                          /* RWSTREAM_H */

