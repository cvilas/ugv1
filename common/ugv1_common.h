//==============================================================================
// Project  : UGV1
// Module   : common
// File     : ugv1_common.h
// Brief    : common header
//==============================================================================
#ifndef UGV1_COMMON_H
#define	UGV1_COMMON_H

// Windows only:
// Define UGV1_DLL and UGV1_DLL_EXPORT when creating UGV1 libraries
// Define only UGV1_DLL when linking against UGV1 libraries.
#ifdef UGV1_DLL
#	ifdef UGV1_DLL_EXPORT
#		define UGV1_DLL_API __declspec(dllexport)
#	else
#		define UGV1_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1_DLL_API
#endif

namespace Ugv1
{
    // All project classes are contained in this namespace

} // Ugv1

#endif // UGV1_COMMON_H
