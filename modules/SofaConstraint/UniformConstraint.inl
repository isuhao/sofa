#ifndef ISPHYSICS_INTERACTION_UNIFORMCONSTRAINT_INL
#define ISPHYSICS_INTERACTION_UNIFORMCONSTRAINT_INL

#include "UniformConstraint.h"
#include <sofa/core/behavior/Constraint.inl>
#include <sofa/core/objectmodel/Data.h>
#include "GenericConstraintResolution.h"

namespace isphysics
{
namespace interaction
{

template< class DataTypes >
UniformConstraint<DataTypes>::UniformConstraint()
    :d_softW(initData(&d_softW, sofa::helper::vector<Real>(1, Real(0)), "softW", "Local compliance to apply on each dof"))
    , m_constraintIndex(0)
{

}

template< class DataTypes >
void UniformConstraint<DataTypes>::buildConstraintMatrix(const sofa::core::ConstraintParams* cParams, DataMatrixDeriv & c, unsigned int &cIndex, const DataVecCoord &x)
{
    const std::size_t N = Deriv::size(); // MatrixDeriv is a container of Deriv types.

    auto& jacobian = sofa::helper::write(c, cParams).wref();
    auto  xVec     = sofa::helper::read(x, cParams);

    m_constraintIndex = cIndex; // we should not have to remember this, it should be available through the API directly.

    for (std::size_t i = 0; i < xVec.size(); ++i)
    {
        for (std::size_t j = 0; j < N; ++j)
        {
            auto row = jacobian.writeLine(N*i + j);
            Deriv d;
            d[j] = Real(1);
            row.setCol(i, d);
            ++cIndex;
        }
    }
}

template< class DataTypes >
void UniformConstraint<DataTypes>::getConstraintViolation(const sofa::core::ConstraintParams* cParams, sofa::defaulttype::BaseVector *resV, const DataVecCoord &x, const DataVecDeriv &v)
{
    auto xfree = sofa::helper::read(x, cParams);
    auto vfree = sofa::helper::read(v, cParams);

    if (cParams->constOrder() == sofa::core::ConstraintParams::VEL)
    {
        const std::size_t N = Deriv::size();

        for (std::size_t i = 0; i < vfree.size(); ++i)
        {
            for (std::size_t j = 0; j < N; ++j)
            {
                resV->set(m_constraintIndex + i*N + j, vfree[i][j]);
            }
        }
    }
    else
    {
        const std::size_t N = Coord::size();

        for (std::size_t i = 0; i < xfree.size(); ++i)
        {
            for (std::size_t j = 0; j < N; ++j)
            {
                resV->set(m_constraintIndex + i*N + j, xfree[i][j]);
            }
        }
    }
}

template< class DataTypes >
void UniformConstraint<DataTypes>::getConstraintResolution(const sofa::core::ConstraintParams* cParams, std::vector<sofa::core::behavior::ConstraintResolution*>& crVector, unsigned int& offset)
{
    
    auto softW  = sofa::helper::read(d_softW, cParams);

    for (std::size_t i = 0; i < this->getMState()->getSize(); ++i)
    {
        for (std::size_t j = 0; j < Deriv::size(); ++j)
        {
            isphysics::contact::GenericConstraintResolution* cr = new isphysics::contact::GenericConstraintResolution();
            cr->setSoftW(  (softW.size() == this->getMState()->getSize() ? softW[i] : softW[0]) );
            cr->setBilateralOnNormal(true);
            crVector[offset++] = cr;

        }
    }
}

}

}

#endif // ISPHYSICS_INTERACTION_UNIFORMCONSTRAINT_INL
