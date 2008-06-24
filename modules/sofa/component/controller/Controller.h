/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
//
// C++ Interface: Controller
//
// Description:
//
//
// Author: Pierre-Jean Bensoussan, Digital Trainers (2008)
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SOFA_COMPONENT_CONTROLLER_CONTROLLER_H
#define SOFA_COMPONENT_CONTROLLER_CONTROLLER_H

#include <sofa/core/componentmodel/behavior/BaseController.h>

namespace sofa
{
namespace core
{
namespace objectmodel
{

class Event;
class MouseEvent;
class OmniEvent;
class KeypressedEvent;
class KeyreleasedEvent;
class JoystickEvent;

}
}
}

namespace sofa
{

namespace component
{

namespace controller
{

/**
 * @brief Controller Class.
 * Interface of user interaction on SOFA Components.
 * Provides also an interface for BeginAnimation and EndAnimation events
 * launched at the beginning and the end of a time step.
 */
class Controller : public virtual core::componentmodel::behavior::BaseController
{

public:
    /**
    * @brief Default constructor.
    */
    Controller();

    /**
    * @brief Mouse event callback.
    */
    virtual void onMouseEvent(core::objectmodel::MouseEvent *) {};

    /**
    * @brief Omni event callback.
    */
    virtual void onOmniEvent(core::objectmodel::OmniEvent *) {};

    /**
    * @brief Key Press event callback.
    */
    virtual void onKeyPressedEvent(core::objectmodel::KeypressedEvent *) {};

    /**
    * @brief Key Release event callback.
    */
    virtual void onKeyReleasedEvent(core::objectmodel::KeyreleasedEvent *) {};

    /**
    * @brief Joystick event callback.
    */
    virtual void onJoystickEvent(core::objectmodel::JoystickEvent *) {};

    /**
    * @brief Begin Animation event callback.
    */
    virtual void onBeginAnimationStep(void) {};

    /**
    * @brief End Animation event callback.
    */
    virtual void onEndAnimationStep(void) {};

protected:

    Data< bool > handleEventTriggersUpdate; ///< Event reception triggers object update ?

private:

    void handleEvent(core::objectmodel::Event *);
};

} // namespace controller

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_CONTROLLER_CONTROLLER_H
