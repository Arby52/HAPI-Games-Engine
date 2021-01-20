#pragma once

//Precompiled header drastically reduces compile time. Also, every class has access to all these includes.
#include <algorithm>
#include <cassert>
#include <math.h>
#include <fstream>
#include <time.h>
#include <HAPI_lib.h> //includes string, vector, iostream
#include <stdlib.h>
#include <unordered_map>
#include <deque>


#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif 

