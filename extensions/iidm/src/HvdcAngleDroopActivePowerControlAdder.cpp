/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControlAdder.hpp>

#include <powsybl/iidm/HvdcLine.hpp>
#include <powsybl/iidm/extensions/iidm/HvdcAngleDroopActivePowerControl.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

HvdcAngleDroopActivePowerControlAdder::HvdcAngleDroopActivePowerControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> HvdcAngleDroopActivePowerControlAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<HvdcLine>(extendable)) {
        return std::unique_ptr<HvdcAngleDroopActivePowerControl>(new HvdcAngleDroopActivePowerControl(dynamic_cast<HvdcLine&>(extendable), m_p0, m_droop, m_enabled));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<HvdcLine>()));
}

HvdcAngleDroopActivePowerControlAdder& HvdcAngleDroopActivePowerControlAdder::withDroop(double droop) {
    m_droop = droop;
    return *this;
}

HvdcAngleDroopActivePowerControlAdder& HvdcAngleDroopActivePowerControlAdder::withEnabled(bool enabled) {
    m_enabled = enabled;
    return *this;
}

HvdcAngleDroopActivePowerControlAdder& HvdcAngleDroopActivePowerControlAdder::withP0(double p0) {
    m_p0 = p0;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
