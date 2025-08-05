/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorStartupAdder.hpp>

#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/GeneratorStartup.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorStartupAdder::GeneratorStartupAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> GeneratorStartupAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return stdcxx::make_unique<GeneratorStartup>(dynamic_cast<Generator&>(extendable), m_predefinedActivePowerSetpoint, m_startupCost, m_marginalCost, m_plannedOutageRate, m_forcedOutageRate);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Generator>()));
}

GeneratorStartupAdder& GeneratorStartupAdder::withForcedOutageRate(double forcedOutageRate) {
    m_forcedOutageRate = forcedOutageRate;
    return *this;
}

GeneratorStartupAdder& GeneratorStartupAdder::withStartupCost(double startUpCost) {
    m_startupCost = startUpCost;
    return *this;
}


GeneratorStartupAdder& GeneratorStartupAdder::withMarginalCost(double marginalCost) {
    m_marginalCost = marginalCost;
    return *this;
}

GeneratorStartupAdder& GeneratorStartupAdder::withPlannedOutageRate(double plannedOutageRate) {
    m_plannedOutageRate = plannedOutageRate;
    return *this;
}

GeneratorStartupAdder& GeneratorStartupAdder::withPredefinedActivePowerSetpoint(double predefinedActivePowerSetpoint) {
    m_predefinedActivePowerSetpoint = predefinedActivePowerSetpoint;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
