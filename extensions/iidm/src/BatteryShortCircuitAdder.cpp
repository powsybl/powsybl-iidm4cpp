/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BatteryShortCircuitAdder.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/extensions/iidm/BatteryShortCircuit.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BatteryShortCircuitAdder::BatteryShortCircuitAdder(Extendable& extendable) :
    AbstractShortCircuitAdder(extendable) {
}

std::unique_ptr<Extension> BatteryShortCircuitAdder::createExtension(Extendable& extendable) const {
    BatteryShortCircuit::checkTransX(m_directTransX);
    if (stdcxx::isInstanceOf<Battery>(extendable)) {
        return std::unique_ptr<BatteryShortCircuit>(new BatteryShortCircuit(dynamic_cast<Battery&>(extendable), m_directSubtransX, m_directTransX, m_stepUpTransformerX));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Battery>()));
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
