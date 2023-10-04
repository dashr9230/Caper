/*
 * String handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#ifndef RWUNICODE
#include <stdio.h>
#endif /* RWUNICODE */
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"

#include "rwstring.h"


/****************************************************************************
 Local Types
 */

typedef union RwPtrChar RwPtrChar;
union RwPtrChar
{
    RwChar       *ptrChar;
    const RwChar *constptrChar;
};

/* Implement some string functions for platforms which don't have them,
 * or they just plain don't work.
 */
#ifndef RWUNICODE

static int
StrICmp(const RwChar * s1, const RwChar * s2)
{
    RWFUNCTION(RWSTRING("StrICmp"));

    if (s1 && s2)
    {
        RwChar          c1, c2;

        do
        {
            c1 = *s1;
            c2 = *s2;

            /* Convert to lower case for comparison */
            if ((c1 >= 'A') && (c1 <= 'Z'))
            {
                c1 += 'a' - 'A';
            }

            if ((c2 >= 'A') && (c2 <= 'Z'))
            {
                c2 += 'a' - 'A';
            }

            /* Compare - anything beats NUL!!! */
            if (c1 != c2)
            {
                RWRETURN(c1 - c2);
            }

            /* Next char */
            s1++;
            s2++;
        }
        while (c1 && c2);

        if (c1 != c2)
        {
            RWRETURN(c1 - c2);
        }
    }

    /* Same or not got two strings */
    RWRETURN(0);
}

static RwChar  *
StrUpr(RwChar * s)
{
    RWFUNCTION(RWSTRING("StrUpr"));

    if (s)
    {
        RwChar         *p = s;

        while (*p)
        {
            RwChar          c = *p;

            if ((c >= 'a') && (c <= 'z'))
            {
                c += 'A' - 'a';
                *p = c;
            }
            p++;
        }
    }

    RWRETURN(s);
}

static RwChar  *
StrLwr(RwChar * s)
{
    RWFUNCTION(RWSTRING("StrLwr"));

    if (s)
    {
        RwChar         *p = s;

        while (*p)
        {
            RwChar          c = *p;

            if ((c >= 'A') && (c <= 'Z'))
            {
                c -= 'A' - 'a';
                *p = c;
            }
            p++;
        }
    }

    RWRETURN(s);
}

static RwChar *
StrChr(const RwChar *s, int c)
{
    RwPtrChar    result;
    const RwChar match = (RwChar) c;
    RwChar       ch;

    RWFUNCTION(RWSTRING("StrChr"));
    RWASSERT(NULL != s);

    result.constptrChar = (const char *)NULL;

    /*
     * For a parallized/optimized version of strchr(), see
     *    glibc-2.1.2/sysdeps/generic/strchr.c
     * in
     *    ftp://prep.ai.mit.edu/pub/gnu/glibc/glibc-2_1_2.tar.gz
     */

    do
    {
        ch = *s;
        if (ch == match)
        {
            result.constptrChar = s;
            break;
        }
        s++;
    }   while (ch);

    RWRETURN(result.ptrChar);
}

static RwChar  *
StrRChr(const RwChar * s, int c)
{
    RwPtrChar    result;
    const RwChar match = (RwChar) c;
    RwChar       ch;

    RWFUNCTION(RWSTRING("StrRChr"));
    RWASSERT(NULL != s);

    result.constptrChar = (const char *)NULL;

    /*
     * For a parallized/optimized version of strrchr(), see
     *    glibc-2.1.2/sysdeps/generic/strrchr.c
     * in
     *    ftp://prep.ai.mit.edu/pub/gnu/glibc/glibc-2_1_2.tar.gz
     */

    do
    {
        ch = *s;
        if (ch == match)
        {
            result.constptrChar = s;
        }
        s++;
    }   while (ch);


    RWRETURN(result.ptrChar);
}

#endif /* !RWUNICODE */

/****************************************************************************
 _rwStringOpen

 On entry   : None
 On exit    : TRUE on success
 */

RwBool
_rwStringOpen(void)
{
    RWFUNCTION(RWSTRING("_rwStringOpen"));

#ifdef RWUNICODE

#else /* UNICODE */
    /* Install ANSI char string system */
    RWSRCGLOBAL(stringFuncs).vecSprintf = sprintf;
    RWSRCGLOBAL(stringFuncs).vecVsprintf = vsprintf;
    RWSRCGLOBAL(stringFuncs).vecStrcpy = strcpy;
    RWSRCGLOBAL(stringFuncs).vecStrncpy = strncpy;
    RWSRCGLOBAL(stringFuncs).vecStrcat = strcat;
    RWSRCGLOBAL(stringFuncs).vecStrncat = strncat;
    RWSRCGLOBAL(stringFuncs).vecStrrchr = StrRChr;
    RWSRCGLOBAL(stringFuncs).vecStrchr = StrChr;
    RWSRCGLOBAL(stringFuncs).vecStrstr = strstr;
    RWSRCGLOBAL(stringFuncs).vecStrcmp = strcmp;
    RWSRCGLOBAL(stringFuncs).vecStricmp = StrICmp;
    RWSRCGLOBAL(stringFuncs).vecStrlen = strlen;
    RWSRCGLOBAL(stringFuncs).vecStrupr = StrUpr;
    RWSRCGLOBAL(stringFuncs).vecStrlwr = StrLwr;
    RWSRCGLOBAL(stringFuncs).vecStrtok = strtok;
//#ifdef PSP_DRVMODEL_H
//    RWSRCGLOBAL(stringFuncs).vecSscanf = NULL;
//#else /* PSP_DRVMODEL_H */
    RWSRCGLOBAL(stringFuncs).vecSscanf = sscanf;
//#endif /* PSP_DRVMODEL_H */
#endif /* UNICODE */

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwFileSystemClose

 On entry   : void
 On exit    : void
 */

void
_rwStringClose(void)
{
    RWFUNCTION(RWSTRING("_rwStringClose"));

    RWRETURNVOID();
}


