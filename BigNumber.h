#ifndef _BIG_NUMBER_
#define _BIG_NUMBER_
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <type_traits>
#include <climits>
#include <cctype>
#include <regex>
#include <map>
#include <stdexcept>
#include <utility>
#include <cstdint>
#include "../helper.h"
using namespace std;


#ifndef _DYNAMIC_
#define _DYNAMIC_ 1
#endif

#if _DYNAMIC_ == 0
#define _BIG_NUMBER_STATIC_
#include "BigNumber_static.h"
#else
#define _BIG_NUMBER_DYNAMIC_
#include "BigNumber_dynamic.h"
#endif

#endif