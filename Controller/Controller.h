//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : Controller.h
// Brief    : UGV1 controller class
//==============================================================================

#ifndef UGV1_CONTROLLER_H
#define UGV1_CONTROLLER_H

#include "common/ugv1_common.h"

namespace Ugv1
{

class UGV1_DLL_API Configuration
{
public:
    // servo pins
    // digital output pins
    // encoder ppr
    // gear ratio
    // wheel perimeter
    // pid gains
    // drive mode
};

class UGV1_DLL_API DesiredState
{
public:
    // digital outputs
    // servo outputs
    // motor drive (speed/power)
};

class UGV1_DLL_API CurrentState
{
public:
    // digital inputs
    // analog inputs
    // speed + timestamp
    // current + timestamp
    // encoder + timestamp
};

/// \class Controller
/// \ingroup vehicle
/// \brief UGV1 controller class.
class UGV1_DLL_API Controller
{
public:
    Controller();
    ~Controller();

};// Controller

} // namespace Ugv1

#endif // UGV1_CONTROLLER_H
