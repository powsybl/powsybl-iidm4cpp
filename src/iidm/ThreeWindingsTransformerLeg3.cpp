/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ThreeWindingsTransformerLeg3.hpp"

#include <powsybl/iidm/RatioTapChanger.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ThreeWindingsTransformerLeg3::ThreeWindingsTransformerLeg3(double r, double x, double ratedU) :
    ThreeWindingsTransformer::Leg2or3(r, x, ratedU) {
}

const std::string& ThreeWindingsTransformerLeg3::getLegAttribute() const {
    static std::string s_legAttribute = "leg 3";

    return s_legAttribute;
}

stdcxx::CReference<Terminal> ThreeWindingsTransformerLeg3::getTerminal() const {
    return LegBase::getTerminal(2UL);
}

stdcxx::Reference<Terminal> ThreeWindingsTransformerLeg3::getTerminal() {
    return LegBase::getTerminal(2UL);
}

}  // namespace iidm

}  // namespace powsybl
