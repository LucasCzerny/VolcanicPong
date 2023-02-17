#pragma once

#ifdef CONFIGURATION_DEBUG
	#define ASSERT(x, msg) if (!(x)) { __debugbreak(); std::cout << "Assertion Failed: " << msg << "\n"; }
#else
	#define ASSERT(x, msg)
#endif