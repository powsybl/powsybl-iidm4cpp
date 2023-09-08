/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Battery.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

Battery::Battery(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious,
    double targetP, double targetQ, double minP, double maxP) :
    Injection(id, name, fictitious),
    m_targetP(network.getVariantManager().getVariantArraySize(), targetP),
    m_targetQ(network.getVariantManager().getVariantArraySize(), targetQ),
    m_minP(checkMinP(*this, minP)),
    m_maxP(checkMaxP(*this, maxP)) {
    ValidationLevel vl = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (stdcxx::isInstanceOf<Network>(network)) {
        auto& n = dynamic_cast<Network&>(network);
        vl = n.getMinimumValidationLevel();
    }
    checkP0(*this, targetP, vl);
    checkQ0(*this, targetQ, vl);
    checkActivePowerLimits(*this, minP, maxP);
}

void Battery::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_targetP[index] = m_targetP[sourceIndex];
        m_targetQ[index] = m_targetQ[sourceIndex];
    }
}

void Battery::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_targetP.resize(m_targetP.size() + number, m_targetP[sourceIndex]);
    m_targetQ.resize(m_targetQ.size() + number, m_targetQ[sourceIndex]);
}

double Battery::getMaxP() const {
    return m_maxP;
}

double Battery::getMinP() const {
    return m_minP;
}

double Battery::getTargetP() const {
    return m_targetP.at(getNetwork().getVariantIndex());
}

double Battery::getTargetQ() const {
    return m_targetQ.at(getNetwork().getVariantIndex());
}

const IdentifiableType& Battery::getType() const {
    static IdentifiableType s_type = IdentifiableType::BATTERY;
    return s_type;
}

const std::string& Battery::getTypeDescription() const {
    static std::string s_typeDescription = "Battery";

    return s_typeDescription;
}

void Battery::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_targetP.resize(m_targetP.size() - number);
    m_targetQ.resize(m_targetQ.size() - number);
}

Battery& Battery::setMaxP(double maxP) {
    checkMaxP(*this, maxP);
    checkActivePowerLimits(*this, m_minP, maxP);
    m_maxP = maxP;

    return *this;
}

Battery& Battery::setMinP(double minP) {
    checkMinP(*this, minP);
    checkActivePowerLimits(*this, minP, m_maxP);
    m_minP = minP;

    return *this;
}

Battery& Battery::setTargetP(double targetP) {
    checkP0(*this, targetP, getNetwork().getMinimumValidationLevel());

    m_targetP[getNetwork().getVariantIndex()] = targetP;

    getNetwork().invalidateValidationLevel();

    return *this;
}

Battery& Battery::setTargetQ(double targetQ) {
    checkQ0(*this, targetQ, getNetwork().getMinimumValidationLevel());

    m_targetQ[getNetwork().getVariantIndex()] = targetQ;

    getNetwork().invalidateValidationLevel();

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
