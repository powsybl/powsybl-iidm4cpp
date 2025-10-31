/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/GroundAdder.hpp>

#include <powsybl/iidm/Ground.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

GroundAdder::GroundAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

Ground& GroundAdder::add() {
    Network& network = getNetwork();

    std::unique_ptr<Ground> ptrGround = stdcxx::make_unique<Ground>(checkAndGetUniqueId(), getName());
    auto& ground = network.checkAndAdd<Ground>(std::move(ptrGround));

    Terminal& terminal = ground.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return ground;
}

const std::string& GroundAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Ground";
    return s_typeDescription;
}


}  // namespace iidm

}  // namespace powsybl
