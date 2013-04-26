//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : Controller.h
// Brief    : UGV1 controller class
//==============================================================================

#ifndef UGV1_CONTROLLER_H
#define UGV1_CONTROLLER_H

#include "IoBoard.h"

namespace Ugv1
{

/// \class Controller
/// \ingroup controller
/// \brief UGV1 controller class.
/*
 * Design notes:
 * Robot will have one object derived from controller class
 * setup() to configure devices and attach them to IO
 * loop() called at a specific frequency in a separate thread.
 * before entering loop(), io input state is updated and device states are updated as configured
 * after exiting loop(), io outputs get written from devices
 * cleanup() to shutdown bits before exit
 */
class UGV1CONTROLLER_DLL_API IController
{
public:
    IController();
    ~IController();
    virtual bool setup() = 0;
    virtual bool loop() = 0;
    virtual bool cleanup() = 0;
private:
};// Controller

} // namespace Ugv1

#endif // UGV1_CONTROLLER_H
