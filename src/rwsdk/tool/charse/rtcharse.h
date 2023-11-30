/*
 *
 * Data structures for the charse toolkit
 */

/***************************************************************************
 *                                                                         *
 * Module  : rtcharse.h                                                    *
 *                                                                         *
 * Purpose : Charset handling                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RTCHARSE_H
#define RTCHARSE_H

/**
 * \defgroup rtcharset RtCharset
 * \ingroup rttool
 *
 * Character Set/Foot Toolkit for Renderware.
 */

/****************************************************************************
 Includes
 */

#include <rwcore.h>

/****************************************************************************
 Global Types
 */

/* RWPUBLIC */

/**
 * \ingroup rtcharset
 * \typedef RtCharset
 * typedef for a structure defining a character set (opaque).
 * \see RtCharsetCreate
 */
typedef RwRaster    RtCharset;

/* RWPUBLICEND */

/* RWPUBLIC */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RtCharset   *RtCharsetCreate(const RwRGBA * foreGround,
                                    const RwRGBA * backGround);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif                          /* RTCHARSE_H */
