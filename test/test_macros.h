#pragma once
#include <stdio.h>

#define ASSERT(x) if (!(x)) { printf("\tError: Assertion failure, line %d in %s\n", __LINE__, __FILE__); return 0; }
