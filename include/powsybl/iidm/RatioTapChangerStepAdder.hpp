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

template <typename Parent>
class RatioTapChangerStepAdder : public TapChangerStepAdder<RatioTapChangerStepAdder<Parent>, Parent> {
public:
    ~RatioTapChangerStepAdder() noexcept = default;

    RatioTapChangerStepAdder<Parent>& setRho(double rho) override;

    RatioTapChangerStepAdder<Parent>& setR(double r) override;

    RatioTapChangerStepAdder<Parent>& setX(double x) override;

    RatioTapChangerStepAdder<Parent>& setG(double g) override;

    RatioTapChangerStepAdder<Parent>& setB(double b) override;

    Parent& endStep() override;

private:
    explicit RatioTapChangerStepAdder(Parent &parent);
    friend class RatioTapChangerAdder;
    friend class RatioTapChangerStepsReplacer;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/RatioTapChangerStepAdder.hxx>

#endif  // POWSYBL_IIDM_RATIOTAPCHANGERSTEPADDER_HPP
