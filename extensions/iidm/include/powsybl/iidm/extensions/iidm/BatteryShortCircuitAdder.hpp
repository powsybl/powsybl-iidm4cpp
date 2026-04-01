/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_BATTERYSHORTCIRCUITADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_BATTERYSHORTCIRCUITADDER_HPP

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuitAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class BatteryShortCircuitAdder : public AbstractShortCircuitAdder<BatteryShortCircuitAdder> {
public:
    /**
     * Constructor
     */
    explicit BatteryShortCircuitAdder(Extendable& extendable);

protected:
    /**
     * Creates the BatteryShortCircuit extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_BATTERYSHORTCIRCUITADDER_HPP
