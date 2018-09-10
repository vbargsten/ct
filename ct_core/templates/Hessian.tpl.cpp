/**********************************************************************************************************************
This file is part of the Control Toolbox (https://adrlab.bitbucket.io/ct), copyright by ETH Zurich, Google Inc.
Licensed under Apache2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

// clang-format off

#include "DERIVATIVE_NAME.h"

namespace ct {
namespace NS1 {
namespace NS2 {


DERIVATIVE_NAME::HES_TYPE DERIVATIVE_NAME::hessian(const Eigen::VectorXd& x_in, const Eigen::VectorXd& w_in)
{
    double* hes = hessian_.data();

    AUTOGENERATED_CODE_PLACEHOLDER

    return hessian_;
}

} // namespace NS2
} // namespace NS1
} // namespace ct

// clang-format on
