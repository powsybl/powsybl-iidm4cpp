/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUITS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUITS_HPP

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/extensions/shortcircuits/AbstractShortCircuits.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

class BatteryShortCircuits : public AbstractShortCircuits {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    BatteryShortCircuits(Battery& battery, double transientReactance, double stepUpTransformerReactance);

    ~BatteryShortCircuits() noexcept override = default;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;
};

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_BATTERYSHORTCIRCUITS_HPP

