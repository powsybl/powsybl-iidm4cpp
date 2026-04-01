/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/BatteryShortCircuit.hpp>

#include <powsybl/iidm/Battery.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

BatteryShortCircuit::BatteryShortCircuit(Battery& battery, double directSubtransX, double directTransX, double stepUpTransformerX) :
    AbstractShortCircuit(battery, directSubtransX, directTransX, stepUpTransformerX) {

}

void BatteryShortCircuit::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>()));
    }
}

const std::string& BatteryShortCircuit::getName() const {
    static std::string s_name = "batteryShortCircuit";
    return s_name;
}

const std::type_index& BatteryShortCircuit::getType() const {
    static std::type_index s_type = typeid(BatteryShortCircuit);
    return s_type;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

