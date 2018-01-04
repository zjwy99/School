#ifndef __TEST_PUBLIC
#define __TEST_PUBLIC

#include <stdint.h>

//functions to change the functionality of the test module
/**
 * Changes the test timer to hit on the new period
 * @param newPeriodms - Period in ms
 */
void changeTestPeriod(const size_t newPeriodms);

#endif