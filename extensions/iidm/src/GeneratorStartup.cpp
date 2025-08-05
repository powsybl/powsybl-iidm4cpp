/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorStartup.hpp>

#include <powsybl/iidm/Generator.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorStartup::GeneratorStartup(Generator& generator, double predefinedActivePowerSetpoint, double startupCost, double marginalCost, double plannedOutageRate, double forcedOutageRate) :
    Extension(generator),
    m_predefinedActivePowerSetpoint(predefinedActivePowerSetpoint),
    m_startupCost(startupCost),
    m_marginalCost(marginalCost),
    m_plannedOutageRate(plannedOutageRate),
    m_forcedOutageRate(forcedOutageRate) {
}

void GeneratorStartup::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

double GeneratorStartup::getForcedOutageRate() const {
    return m_forcedOutageRate;
}

double GeneratorStartup::getMarginalCost() const {
    return m_marginalCost;
}

const std::string& GeneratorStartup::getName() const {
    static std::string s_name = "startup";
    return s_name;
}

double GeneratorStartup::getPlannedOutageRate() const {
    return m_plannedOutageRate;
}

double GeneratorStartup::getPredefinedActivePowerSetpoint() const {
    return m_predefinedActivePowerSetpoint;
}

double GeneratorStartup::getStartupCost() const {
    return m_startupCost;
}

const std::type_index& GeneratorStartup::getType() const {
    static std::type_index s_type = typeid(GeneratorStartup);
    return s_type;
}

GeneratorStartup& GeneratorStartup::setForcedOutageRate(double forcedOutageRate) {
    m_forcedOutageRate = forcedOutageRate;
    return *this;
}

GeneratorStartup& GeneratorStartup::setMarginalCost(double marginalCost) {
    m_marginalCost = marginalCost;
    return *this;
}

GeneratorStartup& GeneratorStartup::setPlannedOutageRate(double plannedOutageRate) {
    m_plannedOutageRate = plannedOutageRate;
    return *this;
}

GeneratorStartup& GeneratorStartup::setPredefinedActivePowerSetpoint(double predefinedActivePowerSetpoint) {
    m_predefinedActivePowerSetpoint = predefinedActivePowerSetpoint;
    return *this;
}

GeneratorStartup& GeneratorStartup::setStartupCost(double startUpCost) {
    m_startupCost = startUpCost;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

