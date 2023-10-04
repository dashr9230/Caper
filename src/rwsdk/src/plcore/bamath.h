/***************************************************************************
 *                                                                         *
 * Module  : rwmath.h                                                      *
 *                                                                         *
 * Purpose : Abstraction API for math functions                            *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATH_H
#define RWMATH_H



#if (!defined(RwPow))
#define RwPow(_x, _y)           pow(_x, _y)
#endif /* (!defined(RwPow)) */



#if (!defined(rwSqrt))
#define rwSqrt(_x)              sqrt(_x)
#endif /* (!defined(rwSqrt)) */



#endif /* RWMATH_H */
