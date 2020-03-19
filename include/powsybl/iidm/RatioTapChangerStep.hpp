/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERSTEP_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERSTEP_HPP

#include <powsybl/iidm/TapChangerStep.hpp>
#include <powsybl/stdcxx/export.hpp>

namespace powsybl {

namespace iidm {

class IIDM_DECLSPEC RatioTapChangerStep : public TapChangerStep<RatioTapChangerStep> {
public:
    RatioTapChangerStep(double rho, double r, double x, double g, double b);

    ~RatioTapChangerStep() noexcept override = default;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERSTEP_HPP
