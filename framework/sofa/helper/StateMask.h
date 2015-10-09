/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_HELPER_PARTICLEMASK_H
#define SOFA_HELPER_PARTICLEMASK_H

#include <sofa/helper/vector.h>

namespace sofa
{

namespace helper
{

/**
 *  \brief Utility class to handle the mechanism of masks.
 *
 *  One of the most time-consuming process in Sofa is the transmission of forces and velocities through the mappings.
 *  If only a little subset of particles are used, we would like to propagate those forces (applyJT), and velocities (applyJ) to this subset only.
 *
 *  This class is used inside the BaseMechanicalState.
 *
 *  USAGE:
 *
 *     - Forcefields, Constraints
 *              which acts only on a little number of dofs should use the mask by adding entries in the mask
 *              This can be done in the utility fonction updateForceMask() (the default implementation adds every dofs in the mask)
 *              Note that for optimization considerations, dofs can be inserted in the mask directly in the ForceField::addForce and Constraint::buildConstraintMatrix functions.
 *              In that case the updateForceMask() should be overloaded not to insert any dofs. (e.g. GearSpringForceField)
 *
 *     - (Multi)Mappings
 *              they must propagate the mask from their child (tomodel) to their parents (frommodels)
 *              ApplyJ shoud use getActivatedEntry to check if a child dof is active (as in some case, every dofs must be updated, do not use unsafe getEntry)
 *              ApplyJT shoud use getEntry to check if a child dof is active and CAN insert parent dofs in the parent mask.
 *              ApplyDJT, getJ/getJs shoud use getEntry to check if a child dof is active
 *              updateForceMask() must insert only active parent dofs in the parent mask (or should add nothing if parents have already been added in ApplyJT)
 *
 */
class StateMask
{

public:

    typedef helper::vector<bool> InternalStorage; // note this should be space-optimized (a bool = a bit) in the STL

    StateMask() : activated(false) {}

    void assign( size_t size, bool value ) { mask.assign( size, value ); }

    void insertEntry( size_t index ) { mask[index]=true; }
    bool getEntry( size_t index ) const { return mask[index]; } // unsafe to be use where we do not care if the mapping in deactivated
    bool getActivatedEntry( size_t index ) const { return activated ? mask[index] : true; } // a if at each check rather that a single if per mapping function is the price to pay no to have duplicated code in mappings
    const InternalStorage& getEntries() const { return mask; }

    void resize( size_t size ) { mask.resize( size ); }
    void clear() { mask.clear(); }
    size_t size() const { return mask.size(); }

    inline friend std::ostream& operator<< ( std::ostream& os, const StateMask& sm )
    {
        return os << sm.mask;
    }

    void activate( bool a ) { activated = a; }

protected:

    InternalStorage mask; // note this should be space-optimized (a bool = a bit) in the STL
    bool activated; // automatic switch (the mask is only used for specific operations)

};


} // namespace helper

} // namespace sofa

#endif
