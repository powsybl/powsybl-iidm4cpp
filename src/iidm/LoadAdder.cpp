/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadAdder.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/stdcxx/make_unique.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

LoadAdder::LoadAdder(VoltageLevel& voltageLevel) :
    InjectionAdder(voltageLevel) {
}

Load& LoadAdder::add() {
    checkLoadType(*this, m_loadType);
    Network& network = getNetwork();
    network.setValidationLevelIfGreaterThan(checkP0(*this, m_p0, network.getMinimumValidationLevel()));
    network.setValidationLevelIfGreaterThan(checkQ0(*this, m_q0, network.getMinimumValidationLevel()));

    std::unique_ptr<Load> ptrLoad = stdcxx::make_unique<Load>(network, checkAndGetUniqueId(), getName(), isFictitious(), m_loadType, m_p0, m_q0);
    auto& load = network.checkAndAdd<Load>(std::move(ptrLoad));

    Terminal& terminal = load.addTerminal(checkAndGetTerminal());
    getVoltageLevel().attach(terminal, false);

    return load;
}

const std::string& LoadAdder::getTypeDescription() const {
    static std::string s_typeDescription = "Load";

    return s_typeDescription;
}

LoadAdder& LoadAdder::setLoadType(const LoadType& loadType) {
    m_loadType = loadType;
    return *this;
}

LoadAdder& LoadAdder::setP0(double p0) {
    m_p0 = p0;
    return *this;
}

LoadAdder& LoadAdder::setQ0(double q0) {
    m_q0 = q0;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
