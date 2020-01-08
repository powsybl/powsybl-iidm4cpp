/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/shortcircuits/VoltageLevelShortCircuits.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

VoltageLevelShortCircuits::VoltageLevelShortCircuits(VoltageLevel& voltageLevel, double minShortCircuitsCurrent, double maxShortCircuitsCurrent) :
    Extension(voltageLevel),
    m_minShortCircuitsCurrent(minShortCircuitsCurrent),
    m_maxShortCircuitsCurrent(maxShortCircuitsCurrent) {
    checkLimits(minShortCircuitsCurrent, maxShortCircuitsCurrent);
}

void VoltageLevelShortCircuits::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<VoltageLevel>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<VoltageLevel>()));
    }
}

double VoltageLevelShortCircuits::checkLimit(double limit) {
    if (std::isnan(limit)) {
        throw PowsyblException("Short circuit current limit is not defined.");
    }
    if (limit < 0.0) {
        throw PowsyblException("Short circuit current limit may not be negative.");
    }
    return limit;
}

void VoltageLevelShortCircuits::checkLimits(double min, double max) {
    checkLimit(min);
    checkLimit(max);
    if (min > max) {
        throw PowsyblException("Min short circuit current > Max short circuit current.");
    }
}

double VoltageLevelShortCircuits::getMaxShortCircuitsCurrent() const {
    return m_maxShortCircuitsCurrent;
}

double VoltageLevelShortCircuits::getMinShortCircuitsCurrent() const {
    return m_minShortCircuitsCurrent;
}

const std::string& VoltageLevelShortCircuits::getName() const {
    static std::string s_name = "voltageLevelShortCircuits";
    return s_name;
}

const std::type_index& VoltageLevelShortCircuits::getType() const {
    static std::type_index s_type = typeid(VoltageLevelShortCircuits);
    return s_type;
}

VoltageLevelShortCircuits& VoltageLevelShortCircuits::setMaxShortCircuitsCurrent(double maxShortCircuitsCurrent) {
    checkLimits(m_minShortCircuitsCurrent, maxShortCircuitsCurrent);
    m_maxShortCircuitsCurrent = maxShortCircuitsCurrent;

    return *this;

}

VoltageLevelShortCircuits& VoltageLevelShortCircuits::setMinShortCircuitsCurrent(double minShortCircuitsCurrent) {
    checkLimits(minShortCircuitsCurrent, m_maxShortCircuitsCurrent);
    m_minShortCircuitsCurrent = minShortCircuitsCurrent;

    return *this;
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

