/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControlAdder.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControl.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltagePerReactivePowerControlAdder::VoltagePerReactivePowerControlAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> VoltagePerReactivePowerControlAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<StaticVarCompensator>(extendable)) {
        return stdcxx::make_unique<VoltagePerReactivePowerControl>(dynamic_cast<StaticVarCompensator&>(extendable), m_slope);
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<StaticVarCompensator>()));
}

VoltagePerReactivePowerControlAdder& VoltagePerReactivePowerControlAdder::withSlope(double slope) {
    m_slope = slope;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
