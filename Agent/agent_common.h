//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : Agent_common.h
// Brief    : common header for UGV1 agents
//==============================================================================
#ifndef UGV1_AGENT_COMMON_H
#define	UGV1_AGENT_COMMON_H

#ifdef UGV1AGENTLIB_DLL
#	ifdef UGV1AGENTLIB_DLL_EXPORT
#		define UGV1AGENTLIB_DLL_API __declspec(dllexport)
#	else
#		define UGV1AGENTLIB_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1AGENTLIB_DLL_API
#endif

namespace Ugv1
{

// All project classes are contained in this namespace



} // Ugv1

#endif // UGV1_AGENT_COMMON_H
