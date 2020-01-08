/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_VOLTAGELEVELSHORTCIRCUITS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_VOLTAGELEVELSHORTCIRCUITS_HPP

#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

class VoltageLevelShortCircuits : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    VoltageLevelShortCircuits(VoltageLevel& voltageLevel, double minShortCircuitsCurrent, double maxShortCircuitsCurrent);

    ~VoltageLevelShortCircuits() noexcept override = default;

    double getMaxShortCircuitsCurrent() const;

    double getMinShortCircuitsCurrent() const;

    VoltageLevelShortCircuits& setMaxShortCircuitsCurrent(double maxShortCircuitsCurrent);

    VoltageLevelShortCircuits& setMinShortCircuitsCurrent(double minShortCircuitsCurrent);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    static double checkLimit(double limit);

    static void checkLimits(double min, double max);

private:
    double m_minShortCircuitsCurrent;

    double m_maxShortCircuitsCurrent;
};

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_VOLTAGELEVELSHORTCIRCUITS_HPP

