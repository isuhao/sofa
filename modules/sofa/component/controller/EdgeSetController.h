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
// C++ Interface: EdgeSetController
//
// Description:
//
//
// Author: Pierre-Jean Bensoussan, Digital Trainers (2008)
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SOFA_COMPONENT_CONTROLLER_EDGESETCONTROLLER_H
#define SOFA_COMPONENT_CONTROLLER_EDGESETCONTROLLER_H

#include <sofa/component/controller/MechanicalStateController.h>


namespace sofa { namespace component { namespace topology { template < class DataTypes> class EdgeSetTopology; } } }


namespace sofa
{

namespace component
{

namespace controller
{

/**
 * @brief EdgeSetController Class
 *
 * Provides a Mouse & Keyboard user control on an EdgeSet Topology.
 */
template<class DataTypes>
class EdgeSetController : public MechanicalStateController<DataTypes>
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord    Coord   ;
    typedef typename DataTypes::Deriv    Deriv   ;
    typedef typename Coord::value_type   Real    ;

    typedef MechanicalStateController<DataTypes> Inherit;

    /**
     * @brief Default Constructor.
     */
    EdgeSetController();

    /**
     * @brief Default Destructor.
     */
    virtual ~EdgeSetController() {};

    /**
     * @brief SceneGraph callback initialization method.
     */
    void init();

    /**
     * @name Controller Interface
     */
    //@{

    /**
     * @brief Mouse event callback.
     */
    void onMouseEvent(core::objectmodel::MouseEvent *);

    void onKeyPressedEvent(core::objectmodel::KeypressedEvent *);


    /**
     * @brief Begin Animation event callback.
     */
    void onBeginAnimationStep();

    //@}

    /**
     * @name Accessors
     */
    //@{


    //@}

    /**
     * @brief Apply the controller modifications to the controlled MechanicalState.
     */
    void applyController(void);

    /**
     * @brief
     */
    void modifyTopology(void);

    /**
     * @brief
     */
    void draw();

protected:
    Real step; ///<

    sofa::component::topology::EdgeSetTopology<DataTypes> *edges; ///<

    Real edge0RestedLength;

};

} // namespace controller

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_CONTROLLER_EDGESETCONTROLLER_H
