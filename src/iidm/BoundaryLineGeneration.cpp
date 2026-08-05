/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BoundaryLineGeneration.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/BoundaryLine.hpp>
#include <powsybl/iidm/MinMaxReactiveLimits.hpp>
#include <powsybl/iidm/Network.hpp>
#include <powsybl/iidm/ReactiveLimitsHolder.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace boundary_line {

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

Generation& Generation::attach(BoundaryLine& boundaryLine) {
    if (m_boundaryLine) {
        throw AssertionError(stdcxx::format("BoundaryLine.Generation already attached to %1%", m_boundaryLine.get().getId()));
    }
    m_boundaryLine = boundaryLine;
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
    return m_boundaryLine.get().getMessageHeader();
}

double Generation::getTargetP() const {
    return m_targetP[m_boundaryLine.get().getNetwork().getVariantIndex()];
}

double Generation::getTargetQ() const {
    return m_targetQ[m_boundaryLine.get().getNetwork().getVariantIndex()];
}

double Generation::getTargetV() const {
    return m_targetV[m_boundaryLine.get().getNetwork().getVariantIndex()];
}

bool Generation::isVoltageRegulationOn() const {
    return m_voltageRegulationOn[m_boundaryLine.get().getNetwork().getVariantIndex()];
}

void Generation::reduceVariantArraySize(unsigned long number) {
    m_targetP.resize(m_targetP.size() - number);
    m_targetQ.resize(m_targetQ.size() - number);
    m_voltageRegulationOn.resize(m_voltageRegulationOn.size() - number);
    m_targetV.resize(m_targetV.size() - number);
}

Generation& Generation::setMaxP(double maxP) {
    checkMaxP(m_boundaryLine, maxP);
    checkActivePowerLimits(m_boundaryLine, m_minP, maxP);
    m_maxP = maxP;
    return *this;
}

Generation& Generation::setMinP(double minP) {
    checkMinP(m_boundaryLine, minP);
    checkActivePowerLimits(m_boundaryLine, minP, m_maxP);
    m_minP = minP;
    return *this;
}

Generation& Generation::setTargetP(double targetP) {
    Network& n = m_boundaryLine.get().getNetwork();
    checkActivePowerSetpoint(m_boundaryLine, targetP, n.getMinimumValidationLevel());
    unsigned long variantIndex = n.getVariantIndex();
    m_targetP[variantIndex] = targetP;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setTargetQ(double targetQ) {
    Network& n = m_boundaryLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_boundaryLine, m_voltageRegulationOn[variantIndex], m_targetV[variantIndex], targetQ, n.getMinimumValidationLevel());
    m_targetQ[variantIndex] = targetQ;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setTargetV(double targetV) {
    Network& n = m_boundaryLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_boundaryLine, m_voltageRegulationOn[variantIndex], targetV, m_targetQ[variantIndex], n.getMinimumValidationLevel());
    m_targetV[variantIndex] = targetV;
    n.invalidateValidationLevel();
    return *this;
}

Generation& Generation::setVoltageRegulationOn(bool voltageRegulationOn) {
    Network& n = m_boundaryLine.get().getNetwork();
    unsigned long variantIndex = n.getVariantIndex();
    checkVoltageControl(m_boundaryLine, voltageRegulationOn, m_targetV[variantIndex], m_targetQ[variantIndex], n.getMinimumValidationLevel());
    m_voltageRegulationOn[variantIndex] = voltageRegulationOn;
    n.invalidateValidationLevel();
    return *this;
}

}  // namespace boundary_line

}  // namespace iidm

}  // namespace powsybl
