/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/VoltagePerReactivePowerControl.hpp>

#include <powsybl/iidm/StaticVarCompensator.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

VoltagePerReactivePowerControl::VoltagePerReactivePowerControl(StaticVarCompensator& svc, double slope) :
    Extension(svc),
    m_slope(checkSlope(slope)) {
}

void VoltagePerReactivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<StaticVarCompensator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<StaticVarCompensator>()));
    }
}

double VoltagePerReactivePowerControl::checkSlope(double slope) {
    if (std::isnan(slope)) {
        throw PowsyblException("Undefined value for slope");
    }
    if (slope < 0) {
        throw PowsyblException(stdcxx::format("Slope value of SVC %1% must be positive: %2%", getExtendable<StaticVarCompensator>().get().getId(), slope));
    }
    return slope;
}

const std::string& VoltagePerReactivePowerControl::getName() const {
    static std::string s_name = "voltagePerReactivePowerControl";
    return s_name;
}

double VoltagePerReactivePowerControl::getSlope() const {
    return m_slope;
}

const std::type_index& VoltagePerReactivePowerControl::getType() const {
    static std::type_index s_type = typeid(VoltagePerReactivePowerControl);
    return s_type;
}

VoltagePerReactivePowerControl& VoltagePerReactivePowerControl::setSlope(double slope) {
    m_slope = checkSlope(slope);
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
