/***************************************************************************
 *                                                                         *
 * Module  : ostypes.h                                                     *
 *                                                                         *
 * Purpose : Types                                                         *
 *                                                                         *
 **************************************************************************/

/* RWPUBLIC */
#ifndef WIN_OSTYPES_H
#define WIN_OSTYPES_H
/* RWPUBLICEND */

/****************************************************************************
 Includes
 */

/****************************************************************************
 Library version number
 */

/* RWPUBLIC */

#define rwLITTLEENDIAN         /* This is a little endian machine */

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

/* RWPUBLICEND */
