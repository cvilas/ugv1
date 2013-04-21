//==============================================================================
// Project  : UGV1
// Module   : ioboard
// File     : ioboard_common.h
// Brief    : common header for ioboard library
//==============================================================================
#ifndef UGV1_VEHICLELIB_COMMON_H
#define	UGV1_VEHICLELIB_COMMON_H

// Windows only:
// Define UGV1_DLL and UGV1_DLL_EXPORT when creating UGV1 libraries
// Define only UGV1_DLL when linking against UGV1 libraries.
#ifdef UGV1VEHICLELIB_DLL
#	ifdef UGV1VEHICLELIB_DLL_EXPORT
#		define UGV1VEHICLELIB_DLL_API __declspec(dllexport)
#	else
#		define UGV1VEHICLELIB_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1VEHICLELIB_DLL_API
#endif

#include <iostream>

namespace Ugv1
{

// All project classes are contained in this namespace



} // Ugv1

#endif // UGV1_IOBOARD_COMMON_H
