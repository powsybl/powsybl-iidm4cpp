/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ActivePowerControlAdder.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ActivePowerControlAdder::ActivePowerControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ActivePowerControlAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Battery>(extendable)) {
        return std::unique_ptr<ActivePowerControl>(new ActivePowerControl(dynamic_cast<Battery&>(extendable), m_participate, m_droop));
    }
    if (stdcxx::isInstanceOf<Generator>(extendable)) {
        return std::unique_ptr<ActivePowerControl>(new ActivePowerControl(dynamic_cast<Generator&>(extendable), m_participate, m_droop));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% or %3% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>(), stdcxx::demangle<Generator>()));
}

ActivePowerControlAdder& ActivePowerControlAdder::withDroop(double droop) {
    m_droop = droop;
    return *this;
}

ActivePowerControlAdder& ActivePowerControlAdder::withParticipate(bool participate) {
    m_participate = participate;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
