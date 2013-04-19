//==============================================================================
// Project  : UGV1
// Module   : hmi
// File     : hmi_common.h
// Brief    : common header for HMI library
//==============================================================================
#ifndef UGV1_HMI_COMMON_H
#define	UGV1_HMI_COMMON_H

#ifdef UGV1HMI_DLL
#	ifdef UGV1HMI_DLL_EXPORT
#		define UGV1HMI_DLL_API __declspec(dllexport)
#	else
#		define UGV1HMI_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1HMI_DLL_API
#endif

namespace Ugv1
{


} // Ugv1

#endif // UGV1_HMI_COMMON_H
