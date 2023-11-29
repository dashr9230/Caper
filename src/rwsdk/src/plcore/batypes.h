/***************************************************************************
 *                                                                         *
 * Module  : batypes.h                                                     *
 *                                                                         *
 * Purpose : Types                                                         *
 *                                                                         *
 **************************************************************************/

#ifndef RWTYPES_H
#define RWTYPES_H


/****************************************************************************
 Includes
 */

#include "ostypes.h"


/* RWPUBLIC */










/****************************************************************************
 Attributes
 */

#if (!defined(__RWUNUSED__))
#define __RWUNUSED__           /* No op */
#endif /* (!defined(__RWUNUSED__)) */




#if (!defined(RWASMCALL))
#define RWASMCALL              /* No op */
#endif /* (!defined(RWASMCALL)) */

#if (!defined(RWASMAPI))
#define RWASMAPI(TYPE) TYPE
#endif /* (!defined(RWASMAPI)) */

/* RWPUBLICEND */

/* RWPUBLIC */

/* RWPUBLICEND */

/* RWPUBLIC */

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

/* RWPUBLICEND */

/* RWPUBLIC */






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

/* RWPUBLICEND */

/* RWPUBLIC */

/* RWPUBLICEND */

#endif /* RWTYPES_H */

