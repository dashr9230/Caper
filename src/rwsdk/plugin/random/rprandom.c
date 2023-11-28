/*
 * Random number generation
 */

/**
 * \ingroup rprandom
 * \page rprandomoverview RpRandom Plugin Overview
 *
 * This plugin provides a 31-bit pseudo-random number generator using 
 * a linear feedback shift register approach using trinomials. It is 
 * based on research performed at the University of California, Berkeley. 
 * (1983, 1993). All 31 bits may be considered random.
 *
 * It is provided as a RenderWare Graphics Plugin as a replacement for generators 
 * used in the standard C libraries. This allows consistent random number 
 * generation across all our supported platforms.
 */

// TODO: Remove these
#include <ostypes.h>
#include <balibtyp.h>

#include "rprandom.h"

/**
 * \ingroup rprandom
 * \ref RpRandomSeed is used to initalize the random number generator 
 * using the specified seed value. 
 *
 * Note that this function does not return the old seed.  Therefore, if it 
 * is required to reproduce a sequence of random numbers, the seed must be 
 * set with a user-defined value before any numbers are generated.  The seed 
 * can then be used again to generate the same numbers.
 *
 * The random number plugin must be attached before using this function.
 *
 * \param seed  The seed value for random number generation.
 *
 * \see RpRandom
 *
 */
void
RpRandomSeed(RwUInt32 seed)
{
	// TODO: RpRandomSeed
}
