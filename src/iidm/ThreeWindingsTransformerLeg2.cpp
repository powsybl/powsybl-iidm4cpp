/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreeWindingsTransformerLeg2.hpp"

#include <powsybl/iidm/RatioTapChanger.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerLeg2::ThreeWindingsTransformerLeg2(double r, double x, double ratedU) :
    ThreeWindingsTransformer::Leg2or3(r, x, ratedU) {
}

stdcxx::CReference<Terminal> ThreeWindingsTransformerLeg2::getTerminal() const {
    return LegBase::getTerminal(1UL);
}

stdcxx::Reference<Terminal> ThreeWindingsTransformerLeg2::getTerminal() {
    return LegBase::getTerminal(1UL);
}

const std::string& ThreeWindingsTransformerLeg2::getTypeDescription() const {
    static std::string s_typeDescription = "3 windings transformer leg 2";

    return s_typeDescription;
}

std::string ThreeWindingsTransformerLeg2::toString() const {
    return logging::format("%1% leg 2", getRefTransformer().get().getId());
}

}  // namespace iidm

}  // namespace powsybl
