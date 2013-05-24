//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : Agent_common.h
// Brief    : common header for UGV1 agents
//==============================================================================
#ifndef UGV1_AGENT_H
#define	UGV1_AGENT_H

#ifdef UGV1AGENT_DLL
#	ifdef UGV1AGENT_DLL_EXPORT
#		define UGV1AGENT_DLL_API __declspec(dllexport)
#	else
#		define UGV1AGENT_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1AGENT_DLL_API
#endif

namespace Ugv1
{

// All project classes are contained in this namespace



} // Ugv1

#endif // UGV1_AGENT_H
