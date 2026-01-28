/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltageRegulationAdder.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/extensions/iidm/VoltageRegulation.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltageRegulationAdder::VoltageRegulationAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> VoltageRegulationAdder::createExtension(Extendable& extendable) const {
    if (!m_voltageRegulatorOn) {
        throw PowsyblException("Voltage regulator status is not defined");
    }
    if (stdcxx::isInstanceOf<Battery>(extendable)) {
        if(!static_cast<bool>(m_regulatingTerminal)) {
            return std::unique_ptr<VoltageRegulation>(new VoltageRegulation(dynamic_cast<Battery&>(extendable), *m_voltageRegulatorOn, m_targetV));
        } else {
            return std::unique_ptr<VoltageRegulation>(new VoltageRegulation(dynamic_cast<Battery&>(extendable), m_regulatingTerminal, *m_voltageRegulatorOn, m_targetV));
        }
    }

    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
}

VoltageRegulationAdder& VoltageRegulationAdder::withRegulatingTerminal(const stdcxx::Reference<Terminal>& terminal) {
    m_regulatingTerminal = terminal;
    return *this;
}

VoltageRegulationAdder& VoltageRegulationAdder::withTargetV(double targetV) {
    m_targetV = targetV;
    return *this;
}

VoltageRegulationAdder& VoltageRegulationAdder::withVoltageRegulatorOn(bool voltageRegulatorOn) {
    m_voltageRegulatorOn = voltageRegulatorOn;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
