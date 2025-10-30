/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HPP
#define POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HPP

#include <powsybl/iidm/TapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

class RatioTapChangerAdder;

class RatioTapChangerStepAdder : public TapChangerStepAdder<RatioTapChangerStepAdder, RatioTapChangerAdder> {
public:
    ~RatioTapChangerStepAdder() noexcept = default;

    RatioTapChangerStepAdder& setRho(double rho) override;

    RatioTapChangerStepAdder& setR(double r) override;

    RatioTapChangerStepAdder& setX(double x) override;

    RatioTapChangerStepAdder& setG(double g) override;

    RatioTapChangerStepAdder& setB(double b) override;

    RatioTapChangerAdder& endStep() override;

private:
    explicit RatioTapChangerStepAdder(RatioTapChangerAdder &parent);
    friend class RatioTapChangerAdder;

};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HPP
