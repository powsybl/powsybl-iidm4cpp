/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/StandbyAutomaton.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/logging/LoggerFactory.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

StandbyAutomaton::StandbyAutomaton(StaticVarCompensator& svc, double b0, bool standby, double lowVoltageSetpoint, double highVoltageSetpoint,
                                   double lowVoltageThreshold, double highVoltageThreshold) :
    Extension(svc),
    m_b0(checkB0(b0)),
    m_standby(standby),
    m_lowVoltageSetpoint(lowVoltageSetpoint),
    m_highVoltageSetpoint(highVoltageSetpoint),
    m_lowVoltageThreshold(lowVoltageThreshold),
    m_highVoltageThreshold(highVoltageThreshold) {
    checkVoltageConfig(lowVoltageSetpoint, highVoltageSetpoint, lowVoltageThreshold, highVoltageThreshold);
}

void StandbyAutomaton::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<StaticVarCompensator>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<StaticVarCompensator>()));
    }
}

double StandbyAutomaton::checkB0(double v) {
    if (std::isnan(v)) {
        throw PowsyblException(logging::format("b0 is invalid"));
    }
    return v;
}

void StandbyAutomaton::checkVoltageConfig(double lowVoltageSetpoint, double highVoltageSetpoint, double lowVoltageThreshold, double highVoltageThreshold) {
    if (std::isnan(lowVoltageSetpoint)) {
        throw PowsyblException(logging::format("lowVoltageSetpoint is invalid"));
    }
    if (std::isnan(highVoltageSetpoint)) {
        throw PowsyblException(logging::format("highVoltageSetpoint is invalid"));
    }
    if (std::isnan(lowVoltageThreshold)) {
        throw PowsyblException(logging::format("lowVoltageThreshold is invalid"));
    }
    if (std::isnan(highVoltageThreshold)) {
        throw PowsyblException(logging::format("highVoltageThreshold is invalid"));
    }
    if (lowVoltageThreshold >= highVoltageThreshold) {
        // FIXME(mathbagu) bug in Java code error message
        throw PowsyblException(logging::format("Inconsistent low (%1%) and high (%2%) voltage thresholds", lowVoltageThreshold, highVoltageThreshold));
    }
    if (lowVoltageSetpoint < lowVoltageThreshold) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<StandbyAutomaton>();
        logger.warn(logging::format("Invalid low voltage setpoint %1% < threshold %2%", lowVoltageSetpoint, lowVoltageThreshold));
    }
    if (highVoltageSetpoint > highVoltageThreshold) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<StandbyAutomaton>();
        logger.warn(logging::format("Invalid high voltage setpoint %1% > threshold %2%", highVoltageSetpoint, highVoltageThreshold));
    }
}

double StandbyAutomaton::getB0() const {
    return m_b0;
}

double StandbyAutomaton::getHighVoltageSetpoint() const {
    return m_highVoltageSetpoint;
}

double StandbyAutomaton::getHighVoltageThreshold() const {
    return m_highVoltageThreshold;
}

double StandbyAutomaton::getLowVoltageSetpoint() const {
    return m_lowVoltageSetpoint;
}

double StandbyAutomaton::getLowVoltageThreshold() const {
    return m_lowVoltageThreshold;
}

const std::string& StandbyAutomaton::getName() const {
    static std::string s_name = "standbyAutomaton";
    return s_name;
}

const std::type_index& StandbyAutomaton::getType() const {
    static std::type_index s_type = typeid(StandbyAutomaton);
    return s_type;
}

bool StandbyAutomaton::isStandby() const {
    return m_standby;
}

StandbyAutomaton& StandbyAutomaton::setB0(double b0) {
    m_b0 = checkB0(b0);
    return *this;
}

StandbyAutomaton& StandbyAutomaton::setHighVoltageSetpoint(double highVoltageSetpoint) {
    checkVoltageConfig(m_lowVoltageSetpoint, highVoltageSetpoint, m_lowVoltageThreshold, m_highVoltageThreshold);
    m_highVoltageSetpoint = highVoltageSetpoint;
    return *this;
}

StandbyAutomaton& StandbyAutomaton::setHighVoltageThreshold(double highVoltageThreshold) {
    checkVoltageConfig(m_lowVoltageSetpoint, m_highVoltageSetpoint, m_lowVoltageThreshold, highVoltageThreshold);
    m_highVoltageThreshold = highVoltageThreshold;
    return *this;
}

StandbyAutomaton& StandbyAutomaton::setLowVoltageSetpoint(double lowVoltageSetpoint) {
    checkVoltageConfig(lowVoltageSetpoint, m_highVoltageSetpoint, m_lowVoltageThreshold, m_highVoltageThreshold);
    m_lowVoltageSetpoint = lowVoltageSetpoint;
    return *this;
}

StandbyAutomaton& StandbyAutomaton::setLowVoltageThreshold(double lowVoltageThreshold) {
    checkVoltageConfig(m_lowVoltageSetpoint, m_highVoltageSetpoint, lowVoltageThreshold, m_highVoltageThreshold);
    m_lowVoltageThreshold = lowVoltageThreshold;
    return *this;
}

StandbyAutomaton& StandbyAutomaton::setStandby(bool standby) {
    m_standby = standby;
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

