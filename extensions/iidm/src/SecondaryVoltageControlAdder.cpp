/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControlAdder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/extensions/iidm/ControlZoneAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SecondaryVoltageControlAdder::SecondaryVoltageControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> SecondaryVoltageControlAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<Network>(extendable)) {
        return std::unique_ptr<SecondaryVoltageControl>(new SecondaryVoltageControl(dynamic_cast<Network&>(extendable), m_controlZones));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Network>()));
}

ControlZoneAdder SecondaryVoltageControlAdder::newControlZone() {
    return ControlZoneAdder(*this);
}

SecondaryVoltageControlAdder& SecondaryVoltageControlAdder::addControlZone(const ControlZone& controlZone){
    m_controlZones.emplace_back(controlZone); //std::move?

    return *this;
}


}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl