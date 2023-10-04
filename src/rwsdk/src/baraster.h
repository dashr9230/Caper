/***************************************************************************
 *                                                                         *
 * Module  : baraster.h                                                    *
 *                                                                         *
 * Purpose : Device dependent bitmaps                                      *
 *                                                                         *
 **************************************************************************/

#ifndef RWRASTER_H
#define RWRASTER_H



enum RwRasterType
{
    rwRASTERTYPENORMAL = 0x00,          /**<Normal */

    rwRASTERDONTALLOCATE = 0x80,        /**<If set the raster is not allocated */
};
typedef enum RwRasterType RwRasterType;


/* How big is my stack!!! */
#define rwRASTERCONTEXTSTACKSIZE 10



typedef struct RwRaster RwRaster;

struct RwRaster
{
    RwRaster           *parent; /* Top level raster if a sub raster */
    RwUInt8            *cpPixels; /* Pixel pointer when locked */
    RwUInt8            *palette; /* Raster palette */
    RwInt32             width, height, depth; /* Dimensions of raster */

    char _pad1[8];

    RwUInt8             cType;  /* Type of raster */
    RwUInt8             cFlags; /* Raster flags */

    char _pad3[18];

    

    /* TODO: RwRaster (size: 52 bytes) */
};


typedef struct rwRasterGlobals rwRasterGlobals;
struct rwRasterGlobals
{
    RwRaster           *rasterStack[rwRASTERCONTEXTSTACKSIZE];
    RwInt32             rasterSP;
    RwRaster            dummyRaster;
    RwFreeList         *rasterFreeList;
};




/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */



extern RwBool       RwRasterDestroy(RwRaster * raster);



/* Opening and closing */
extern void        *_rwRasterClose(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwRasterOpen(void *instance, RwInt32 offset,
                                      RwInt32 size);


#ifdef    __cplusplus
}
#endif                          /* __cplusplus */


#endif                          /* RWRASTER_H */

