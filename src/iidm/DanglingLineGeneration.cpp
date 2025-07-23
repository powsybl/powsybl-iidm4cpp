/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DanglingLineGeneration.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/DanglingLine.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace dangling_line {

Generation::Generation(VariantManagerHolder& network, double minP, double maxP, double targetP, double targetQ, double targetV, bool voltageRegulationOn) :
    ReactiveLimitsHolder(stdcxx::make_unique<MinMaxReactiveLimits>(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max())),
    m_minP(std::isnan(minP) ? -std::numeric_limits<double>::max() : minP),
    m_maxP(std::isnan(maxP) ? std::numeric_limits<double>::max() : maxP) {
    unsigned long variantArraySize = network.getVariantManager().getVariantArraySize();
    m_targetP.resize(variantArraySize, targetP);
    m_targetQ.resize(variantArraySize, targetQ);
    m_targetV.resize(variantArraySize, targetV);
    m_voltageRegulationOn.resize(variantArraySize, voltageRegulationOn);
}

void Generation::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_targetP[index] = m_targetP[sourceIndex];
        m_targetQ[index] = m_targetQ[sourceIndex];
        m_voltageRegulationOn[index] = m_voltageRegulationOn[sourceIndex];
        m_targetV[index] = m_targetV[sourceIndex];
    }
}

Generation& Generation::attach(DanglingLine& danglineLine) {
    if (m_danglingLine) {
        throw AssertionError(stdcxx::format("DanglingLine.Generation already attached to %1%", m_danglingLine.get().getId()));
    }
    m_danglingLine = danglineLine;
    return *this;
}

void Generation::extendVariantArraySize(unsigned long number, unsigned long sourceIndex) {
    m_targetP.resize(m_targetP.size() + number, m_targetP[sourceIndex]);
    m_targetQ.resize(m_targetQ.size() + number, m_targetQ[sourceIndex]);
    m_voltageRegulationOn.resize(m_voltageRegulationOn.size() + number, m_voltageRegulationOn[sourceIndex]);
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
}

double Generation::getMaxP() const {
    return m_maxP;
}

double Generation::getMinP() const {
    return m_minP;
}

std::string Generation::getMessageHeader() const {
    return m_danglingLine.get().getMessageHeader();
}

double Generation::getTargetP() const {
    return m_targetP[m_danglingLine.get().getNetwork().getVariantIndex()];
}

double Generation::getTargetQ() const {
    return m_targetQ[m_danglingLine.get().getNetwork().getVariantIndex()];
}

double Generation::getTargetV() const {
    return m_targetV[m_danglingLine.get().getNetwork().getVariantIndex()];
}

bool Generation::isVoltageRegulationOn() const {
    return m_voltageRegulationOn[m_danglingLine.get().getNetwork().getVariantIndex()];
}

void Generation::reduceVariantArraySize(unsigned long number) {
    m_targetP.resize(m_targetP.size() - number);
    m_targetQ.resize(m_targetQ.size() - number);
    m_voltageRegulationOn.resize(m_voltageRegulationOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
}

Generation& Generation::setMaxP(double maxP) {
    checkMaxP(m_danglingLine, maxP);
    checkActivePowerLimits(m_danglingLine, m_minP, maxP);
    m_maxP = maxP;
    return *this;
}

Generation& Generation::setMinP(double minP) {
    checkMinP(m_danglingLine, minP);
    checkActivePowerLimits(m_danglingLine, minP, m_maxP);
    m_minP = minP;
    return *this;
}

Generation& Generation::setTargetP(double targetP) {
    Network& n = m_danglingLine.get().getNetwork();
    checkActivePowerSetpoint(m_danglingLine, targetP, n.getMinimumValidationLevel());
    unsigned long variantIndex = n.getVariantIndex();
    m_targetP[variantIndex] = targetP;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setTargetQ(double targetQ) {
    Network& n = m_danglingLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_danglingLine, m_voltageRegulationOn[variantIndex], m_targetV[variantIndex], targetQ, n.getMinimumValidationLevel());
    m_targetQ[variantIndex] = targetQ;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setTargetV(double targetV) {
    Network& n = m_danglingLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_danglingLine, m_voltageRegulationOn[variantIndex], targetV, m_targetQ[variantIndex], n.getMinimumValidationLevel());
    m_targetV[variantIndex] = targetV;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setVoltageRegulationOn(bool voltageRegulationOn) {
    Network& n = m_danglingLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_danglingLine, voltageRegulationOn, m_targetV[variantIndex], m_targetQ[variantIndex], n.getMinimumValidationLevel());
    m_voltageRegulationOn[variantIndex] = voltageRegulationOn;
    n.invalidateValidationLevel();
    return *this;
}

}  // namespace dangling_line

}  // namespace iidm

}  // namespace powsybl
