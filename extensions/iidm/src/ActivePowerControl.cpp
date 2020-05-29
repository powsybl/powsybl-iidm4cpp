/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ActivePowerControl.hpp>

#include <powsybl/iidm/Battery.hpp>
#include <powsybl/iidm/Generator.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ActivePowerControl::ActivePowerControl(Battery& battery, bool participate, double droop) :
    Extension(battery),
    m_participate(participate),
    m_droop(droop) {
}

ActivePowerControl::ActivePowerControl(Generator& generator, bool participate, double droop) :
    Extension(generator),
    m_participate(participate),
    m_droop(droop) {
}

void ActivePowerControl::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get()) && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% or %3% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>(), stdcxx::demangle<Generator>()));
    }
}

double ActivePowerControl::getDroop() const {
    return m_droop;
}

const std::string& ActivePowerControl::getName() const {
    static std::string s_name = "activePowerControl";
    return s_name;
}

const std::type_index& ActivePowerControl::getType() const {
    static std::type_index s_type = typeid(ActivePowerControl);
    return s_type;
}

bool ActivePowerControl::isParticipate() const {
    return m_participate;
}

ActivePowerControl& ActivePowerControl::setDroop(double droop) {
    m_droop = droop;
    return *this;
}

ActivePowerControl& ActivePowerControl::setParticipate(bool participate) {
    m_participate = participate;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
