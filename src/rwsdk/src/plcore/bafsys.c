/*
 * 
 * File system handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/* DEVELOPER NOTE: 
 * This Overview should be updated if either this file or the one containing
 * the memory interface is changed.
 */

/**
 * \ingroup rwos
 * \page rwosoverview RwOs Overview
 *
 * This object exposes the file and memory interfaces to the application.
 *
 * RenderWare Graphics maintains tables of file I/O and memory handling functions. The prototypes are 
 * based on standard ANSI functionality. Developers are allowed to change individual entries 
 * in these tables so that they can provide their own functions. 
 *
 * Extreme care should be taken with these functions as memory leaks or data corruption may 
 * occur if this feature is misused. RwEngineInit() can be used instead of these functions 
 * if you only need to change the functions once, when your application starts.
 *
 */

/****************************************************************************
 Includes
 */




#ifdef FSYS_WIN32

#else /* FSYS_WIN32 */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */
#include <stdio.h>
#endif /* FSYS_WIN32 */

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"











static RwBool
rwfexist(const RwChar * name)
{
    void *handle;
    RwBool fileExists;

    RWFUNCTION(RWSTRING("rwfexist"));

    /* Test for read-ability */
    handle = RwFopen(name, RWSTRING("rb"));
    fileExists = (handle != NULL);
    if (handle)
    {
        RwFclose(handle);
    }

    RWRETURN(fileExists);
}
























/****************************************************************************
 _rwFileSystemOpen

 On entry   : Pointer to optional default filing system overload
 On exit    : TRUE on success
 */

RwBool
_rwFileSystemOpen(void)
{
    RWFUNCTION(RWSTRING("_rwFileSystemOpen"));

    /* A really noddy default implementation of fexist */
    RWSRCGLOBAL(fileFuncs).rwfexist = (rwFnFexist) rwfexist;

    RWSRCGLOBAL(fileFuncs).rwfopen = (rwFnFopen) fopen;
    RWSRCGLOBAL(fileFuncs).rwfclose = (rwFnFclose) fclose;
    RWSRCGLOBAL(fileFuncs).rwfread = (rwFnFread) fread;
    RWSRCGLOBAL(fileFuncs).rwfwrite = (rwFnFwrite) fwrite;
    RWSRCGLOBAL(fileFuncs).rwfgets = (rwFnFgets) fgets;
    RWSRCGLOBAL(fileFuncs).rwfputs = (rwFnFputs) fputs;
    RWSRCGLOBAL(fileFuncs).rwfeof = (rwFnFeof) feof;
    RWSRCGLOBAL(fileFuncs).rwfseek = (rwFnFseek) fseek;
    RWSRCGLOBAL(fileFuncs).rwfflush = (rwFnFflush) fflush;
    RWSRCGLOBAL(fileFuncs).rwftell = (rwFnFtell) ftell;

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwFileSystemClose

 On entry   : void
 On exit    : void
 */

void
_rwFileSystemClose(void)
{
    RWFUNCTION(RWSTRING("_rwFileSystemClose"));

    RWRETURNVOID();
}

