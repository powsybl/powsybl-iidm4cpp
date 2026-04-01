/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUIT_HPP

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuit.hpp>

namespace powsybl {

namespace iidm {

class Battery;

namespace extensions {

namespace iidm {

class BatteryShortCircuit : public AbstractShortCircuit<BatteryShortCircuit> {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    ~BatteryShortCircuit() noexcept override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    BatteryShortCircuit(Battery& battery, double directSubtransX, double directTransX, double stepUpTransformerX);
    friend class BatteryShortCircuitAdder;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUIT_HPP

