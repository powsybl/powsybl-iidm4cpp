/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/StandbyAutomatonAdder.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/extensions/iidm/StandbyAutomaton.hpp>

#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

StandbyAutomatonAdder::StandbyAutomatonAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> StandbyAutomatonAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<StaticVarCompensator>(extendable)) {
        return std::unique_ptr<StandbyAutomaton>(new StandbyAutomaton(dynamic_cast<StaticVarCompensator&>(extendable), m_b0, m_standby, m_lowVoltageSetpoint, m_highVoltageSetpoint, m_lowVoltageThreshold, m_highVoltageThreshold));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<StaticVarCompensator>()));
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withB0(double b0) {
    m_b0 = b0;
    return *this;
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withHighVoltageSetpoint(double highVoltageSetpoint) {
    m_highVoltageSetpoint = highVoltageSetpoint;
    return *this;
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withHighVoltageThreshold(double highVoltageThreshold) {
    m_highVoltageThreshold = highVoltageThreshold;
    return *this;
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withLowVoltageSetpoint(double lowVoltageSetpoint) {
    m_lowVoltageSetpoint = lowVoltageSetpoint;
    return *this;
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withLowVoltageThreshold(double lowVoltageThreshold) {
    m_lowVoltageThreshold = lowVoltageThreshold;
    return *this;
}

StandbyAutomatonAdder& StandbyAutomatonAdder::withStandby(bool standby) {
    m_standby = standby;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
