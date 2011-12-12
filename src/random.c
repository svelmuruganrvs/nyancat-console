#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "random.h"

/**
 * Initializes the random number generator with time based seed.
 */
void init_random(void)
{
    srand(time(0));
}

/**
 * @limit: Maximum generated number.
 *
 * Retreives a random number in range 0 to limit.
 */
int limited_random(unsigned limit)
{
    assert(limit > 1);
    return (int)((double)limit * rand() / (RAND_MAX + 1.0));
}
