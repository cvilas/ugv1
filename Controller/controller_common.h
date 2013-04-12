//==============================================================================
// Project  : UGV1
// Module   : controller
// File     : controller_common.h
// Brief    : common header for controller library
//==============================================================================
#ifndef UGV1_CONTROLLER_COMMON_H
#define	UGV1_CONTROLLER_COMMON_H

// Windows only:
// Define UGV1_DLL and UGV1_DLL_EXPORT when creating UGV1 libraries
// Define only UGV1_DLL when linking against UGV1 libraries.
#ifdef UGV1CONTROLLER_DLL
#	ifdef UGV1CONTROLLER_DLL_EXPORT
#		define UGV1CONTROLLER_DLL_API __declspec(dllexport)
#	else
#		define UGV1CONTROLLER_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1CONTROLLER_DLL_API
#endif

namespace Ugv1
{

// All project classes are contained in this namespace



} // Ugv1

#endif // UGV1_CONTROLLER_COMMON_H
