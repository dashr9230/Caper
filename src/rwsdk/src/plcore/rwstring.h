/***************************************************************************
 *                                                                         *
 * Module  : rwstring.h                                                    *
 *                                                                         *
 * Purpose : Abstraction API for string functions                          *
 *                                                                         *
 **************************************************************************/

#ifndef RWSTRING_H
#define RWSTRING_H


/****************************************************************************
 Defines
 */



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



/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwBool _rwStringOpen(void);
extern void _rwStringClose(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* RWSTRING_H */
