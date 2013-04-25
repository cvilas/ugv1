//==============================================================================
// Project  : UGV1
// Module   : Vehicle
// File     : VehicleException.h
// Brief    : IO board interface
//==============================================================================

#ifndef UGV1_VEHICLEEXCEPTION_H
#define UGV1_VEHICLEEXCEPTION_H

#include <core/Exception.h>
#include "vehiclelib_common.h"

namespace Ugv1
{
	/// \brief common base class for exceptions thrown by vehicle software
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API VehicleException : public Grape::Exception
	{
	public:
		VehicleException() throw() : Exception(0, "Unspecified Exception") {}
		VehicleException(int code, const std::string& msg) throw() : Exception(code, msg.c_str()) {}
		VehicleException(const VehicleException& ve) throw() : Exception(ve) {}
		VehicleException& operator=(const VehicleException& other) throw() { Exception::operator=(other); return *this; }
	}; // VehicleException

	/// \brief Invalid message ID
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API InvalidMessageIdException : public VehicleException
	{
	public:
		InvalidMessageIdException() throw() : VehicleException(0, "Invalid message ID") {}
		InvalidMessageIdException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		InvalidMessageIdException(const InvalidMessageIdException& ve) throw() : VehicleException(ve) {}
		InvalidMessageIdException& operator=(const InvalidMessageIdException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // InvalidMessageIdException

	/// \brief Invalid message checksum
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API InvalidMessageChecksumException : public VehicleException
	{
	public:
		InvalidMessageChecksumException() throw() : VehicleException(0, "Invalid message checksum") {}
		InvalidMessageChecksumException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		InvalidMessageChecksumException(const InvalidMessageChecksumException& ve) throw() : VehicleException(ve) {}
		InvalidMessageChecksumException& operator=(const InvalidMessageChecksumException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // InvalidMessageChecksumException

	/// \brief Invalid message header
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API InvalidMessageHeaderException : public VehicleException
	{
	public:
		InvalidMessageHeaderException() throw() : VehicleException(0, "Invalid message header") {}
		InvalidMessageHeaderException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		InvalidMessageHeaderException(const InvalidMessageHeaderException& ve) throw() : VehicleException(ve) {}
		InvalidMessageHeaderException& operator=(const InvalidMessageHeaderException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // InvalidMessageHeaderException

	/// \brief Invalid message header
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API InvalidMessageLengthException : public VehicleException
	{
	public:
		InvalidMessageLengthException() throw() : VehicleException(0, "Invalid message length") {}
		InvalidMessageLengthException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		InvalidMessageLengthException(const InvalidMessageLengthException& ve) throw() : VehicleException(ve) {}
		InvalidMessageLengthException& operator=(const InvalidMessageLengthException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // InvalidMessageLengthException

	/// \brief IO operation didn't complete during assigned wait period
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API IoWaitException : public VehicleException
	{
	public:
		IoWaitException() throw() : VehicleException(0, "IO operation didn't complete within wait period") {}
		IoWaitException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		IoWaitException(const IoWaitException& ve) throw() : VehicleException(ve) {}
		IoWaitException& operator=(const IoWaitException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // IoWaitException
	
	/// \brief IO write operation failed
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API IoWriteException : public VehicleException
	{
	public:
		IoWriteException() throw() : VehicleException(0, "IO write operation failed") {}
		IoWriteException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		IoWriteException(const IoWriteException& ve) throw() : VehicleException(ve) {}
		IoWriteException& operator=(const IoWriteException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // IoWriteException

	/// \brief IO read operation failed
	/// \ingroup exceptions
	class UGV1VEHICLELIB_DLL_API IoReadException : public VehicleException
	{
	public:
		IoReadException() throw() : VehicleException(0, "IO read operation failed") {}
		IoReadException(int code, const std::string& msg) throw() : VehicleException(code, msg) {}
		IoReadException(const IoReadException& ve) throw() : VehicleException(ve) {}
		IoReadException& operator=(const IoReadException& other) throw() { VehicleException::operator=(other); return *this; }
	}; // IoReadException
	
} // Ugv1

#endif // UGV1_VEHICLEEXCEPTION_H