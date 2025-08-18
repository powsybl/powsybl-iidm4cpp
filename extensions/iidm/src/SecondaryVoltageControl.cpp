/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/SecondaryVoltageControl.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Network.hpp>

#include <powsybl/iidm/extensions/iidm/ControlZone.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

SecondaryVoltageControl::SecondaryVoltageControl(Network& network, const std::vector<ControlZone>& controlZones) : 
    Extension(network) {
    if(controlZones.empty()) {
        throw PowsyblException("Empty control zone list");
    }
    m_controlZones = controlZones;
}

const std::string& SecondaryVoltageControl::getName() const {
    static std::string s_name = "secondaryVoltageControl";
    return s_name;
}

const std::type_index& SecondaryVoltageControl::getType() const {
    static std::type_index s_type = typeid(SecondaryVoltageControl);
    return s_type;
}

const std::vector<ControlZone>& SecondaryVoltageControl::getControlZones() const {
    return m_controlZones;
}

void SecondaryVoltageControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Network>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Network>()));
    }
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl