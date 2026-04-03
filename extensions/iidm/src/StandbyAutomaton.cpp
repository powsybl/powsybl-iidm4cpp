/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/StandbyAutomaton.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/StaticVarCompensator.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

const std::string& StandbyAutomaton::getName() const {
    static std::string s_name = "standbyAutomaton";
    return s_name;
}

const std::type_index& StandbyAutomaton::getType() const {
    static std::type_index s_type = typeid(StandbyAutomaton);
    return s_type;
}

StandbyAutomaton::StandbyAutomaton(StaticVarCompensator& svc, double b0, bool standby,
                                double lowVoltageSetpoint, double highVoltageSetpoint,
                                double lowVoltageThreshold, double highVoltageThreshold) :
    AbstractMultiVariantIdentifiableExtension(svc) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_standby.resize(variantArraySize, standby);
    m_b0 = checkB0(b0, svc);
    checkVoltageConfig(lowVoltageSetpoint, highVoltageSetpoint, lowVoltageThreshold, highVoltageThreshold, svc, standby);
    m_lowVoltageSetpoint.resize(variantArraySize, lowVoltageSetpoint);
    m_highVoltageSetpoint.resize(variantArraySize, highVoltageSetpoint);
    m_lowVoltageThreshold.resize(variantArraySize, lowVoltageThreshold);
    m_highVoltageThreshold.resize(variantArraySize, highVoltageThreshold);
}

void StandbyAutomaton::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<StaticVarCompensator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<StaticVarCompensator>()));
    }
}

double StandbyAutomaton::checkB0(double b0, const StaticVarCompensator& svc) {
    if (std::isnan(b0)) {
        throw ValidationException(svc, stdcxx::format("b0 (%1%) is invalid", b0));
    }
    return b0;
}

void StandbyAutomaton::checkVoltageConfig(double lowVoltageSetpoint, double highVoltageSetpoint, double lowVoltageThreshold, double highVoltageThreshold, const StaticVarCompensator& svc, bool standby) {
    if (std::isnan(lowVoltageSetpoint)) {
        throw ValidationException(svc, stdcxx::format("lowVoltageSetpoint (%1%) is invalid", lowVoltageSetpoint));
    }
    if (std::isnan(highVoltageSetpoint)) {
        throw ValidationException(svc, stdcxx::format("highVoltageSetpoint (%1%) is invalid", highVoltageSetpoint));
    }
    if (std::isnan(lowVoltageThreshold)) {
        throw ValidationException(svc, stdcxx::format("lowVoltageThreshold (%1%) is invalid", lowVoltageThreshold));
    }
    if (std::isnan(highVoltageThreshold)) {
        throw ValidationException(svc, stdcxx::format("highVoltageThreshold (%1%) is invalid", highVoltageThreshold));
    }
    if (lowVoltageThreshold >= highVoltageThreshold) {
        if(standby) {
            throw ValidationException(svc, stdcxx::format("Inconsistent low (%1%) and high (%2%) voltage thresholds", lowVoltageThreshold, highVoltageThreshold));
        } else {
            logging::Logger& logger = logging::LoggerFactory::getLogger<StandbyAutomaton>();
            logger.warn(stdcxx::format("%1%Inconsistent low %2% and high (%3%) voltage thresholds", svc.getMessageHeader(),  lowVoltageSetpoint, lowVoltageThreshold));
        }
    }
    if (lowVoltageSetpoint < lowVoltageThreshold) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<StandbyAutomaton>();
        logger.warn(stdcxx::format("%1%Invalid low voltage setpoint %2% < threshold %3%", svc.getMessageHeader(),  lowVoltageSetpoint, lowVoltageThreshold));
    }
    if (highVoltageSetpoint > highVoltageThreshold) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<StandbyAutomaton>();
        logger.warn(stdcxx::format("%1%Invalid high voltage setpoint %2% > threshold %3%", svc.getMessageHeader(), highVoltageSetpoint, highVoltageThreshold));
    }
}

bool StandbyAutomaton::isStandby() const {
    return m_standby[getVariantIndex()];
}
StandbyAutomaton& StandbyAutomaton::setStandby(bool standby) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    unsigned long varIndex = getVariantIndex();
    checkVoltageConfig(m_lowVoltageSetpoint[varIndex], m_highVoltageSetpoint[varIndex],
                        m_lowVoltageThreshold[varIndex], m_highVoltageThreshold[varIndex],
                        svc, standby);
    m_standby[varIndex] = standby;
    return *this;
}

double StandbyAutomaton::getB0() const {
    return m_b0;
}
StandbyAutomaton& StandbyAutomaton::setB0(double b0) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    m_b0 = checkB0(b0, svc);
    return *this;
}

double StandbyAutomaton::getHighVoltageSetpoint() const {
    return m_highVoltageSetpoint[getVariantIndex()];
}
StandbyAutomaton& StandbyAutomaton::setHighVoltageSetpoint(double highVoltageSetpoint) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    unsigned long varIndex = getVariantIndex();
    checkVoltageConfig(m_lowVoltageSetpoint[varIndex], highVoltageSetpoint,
                        m_lowVoltageThreshold[varIndex], m_highVoltageThreshold[varIndex],
                        svc, m_standby[varIndex]);
    m_highVoltageSetpoint[varIndex] = highVoltageSetpoint;
    return *this;
}

double StandbyAutomaton::getHighVoltageThreshold() const {
    return m_highVoltageThreshold[getVariantIndex()];
}
StandbyAutomaton& StandbyAutomaton::setHighVoltageThreshold(double highVoltageThreshold) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    unsigned long varIndex = getVariantIndex();
    checkVoltageConfig(m_lowVoltageSetpoint[varIndex], m_highVoltageSetpoint[varIndex],
                        m_lowVoltageThreshold[varIndex], highVoltageThreshold,
                        svc, m_standby[varIndex]);
    m_highVoltageThreshold[varIndex] = highVoltageThreshold;
    return *this;
}

double StandbyAutomaton::getLowVoltageSetpoint() const {
    return m_lowVoltageSetpoint[getVariantIndex()];
}
StandbyAutomaton& StandbyAutomaton::setLowVoltageSetpoint(double lowVoltageSetpoint) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    unsigned long varIndex = getVariantIndex();
    checkVoltageConfig(lowVoltageSetpoint, m_highVoltageSetpoint[varIndex],
                        m_lowVoltageThreshold[varIndex], m_highVoltageThreshold[varIndex],
                        svc, m_standby[varIndex]);
    m_lowVoltageSetpoint[varIndex] = lowVoltageSetpoint;
    return *this;
}

double StandbyAutomaton::getLowVoltageThreshold() const {
    return m_lowVoltageThreshold[getVariantIndex()];
}
StandbyAutomaton& StandbyAutomaton::setLowVoltageThreshold(double lowVoltageThreshold) {
    const auto& svc = getExtendable<StaticVarCompensator>().get();
    unsigned long varIndex = getVariantIndex();
    checkVoltageConfig(m_lowVoltageSetpoint[varIndex], m_highVoltageSetpoint[varIndex],
                        lowVoltageThreshold, m_highVoltageThreshold[varIndex],
                        svc, m_standby[varIndex]);
    m_lowVoltageThreshold[varIndex] = lowVoltageThreshold;
    return *this;
}

void StandbyAutomaton::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_standby[index] = m_standby[sourceIndex];
        m_lowVoltageSetpoint[index] = m_lowVoltageSetpoint[sourceIndex];
        m_highVoltageSetpoint[index] = m_highVoltageSetpoint[sourceIndex];
        m_lowVoltageThreshold[index] = m_lowVoltageThreshold[sourceIndex];
        m_highVoltageThreshold[index] = m_highVoltageThreshold[sourceIndex];
    }
}

void StandbyAutomaton::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void StandbyAutomaton::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_standby.resize(m_standby.size() + number, m_standby[sourceIndex]);
    m_lowVoltageSetpoint.resize(m_lowVoltageSetpoint.size() + number, m_lowVoltageSetpoint[sourceIndex]);
    m_highVoltageSetpoint.resize(m_highVoltageSetpoint.size() + number, m_highVoltageSetpoint[sourceIndex]);
    m_lowVoltageThreshold.resize(m_lowVoltageThreshold.size() + number, m_lowVoltageThreshold[sourceIndex]);
    m_highVoltageThreshold.resize(m_highVoltageThreshold.size() + number, m_highVoltageThreshold[sourceIndex]);
}

void StandbyAutomaton::reduceVariantArraySize(unsigned long number) {
    m_standby.resize(m_standby.size() - number);
    m_lowVoltageSetpoint.resize(m_lowVoltageSetpoint.size() - number);
    m_highVoltageSetpoint.resize(m_highVoltageSetpoint.size() - number);
    m_lowVoltageThreshold.resize(m_lowVoltageThreshold.size() - number);
    m_highVoltageThreshold.resize(m_highVoltageThreshold.size() - number);
}

} // namespace iidm

} // namespace extensions

} // namespace iidm

} // namespace powsybl
