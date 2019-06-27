/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Battery.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Battery::Battery(VariantManagerHolder& network, const std::string& id, const std::string& name,
    double p0, double q0, double minP, double maxP) :
    Injection(id, name, ConnectableType::BATTERY),
    m_network(network),
    m_p0(network.getVariantManager().getVariantArraySize(), checkP0(*this, p0)),
    m_q0(network.getVariantManager().getVariantArraySize(), checkQ0(*this, q0)),
    m_minP(checkMinP(*this, minP)),
    m_maxP(checkMaxP(*this, maxP)) {
    checkActivePowerLimits(*this, minP, maxP, p0);
}

void Battery::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_q0[index] = m_q0[sourceIndex];
    }
}

void Battery::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_q0.resize(m_q0.size() + number, m_q0[sourceIndex]);
}

double Battery::getMaxP() const {
    return m_maxP;
}

double Battery::getMinP() const {
    return m_minP;
}

double Battery::getP0() const {
    return m_p0.at(m_network.get().getVariantIndex());
}

double Battery::getQ0() const {
    return m_q0.at(m_network.get().getVariantIndex());
}

const std::string& Battery::getTypeDescription() const {
    static std::string s_typeDescription = "Battery";

    return s_typeDescription;
}

void Battery::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_p0.resize(m_p0.size() - number);
    m_q0.resize(m_q0.size() - number);
}

Battery& Battery::setMaxP(double maxP) {
    checkMaxP(*this, maxP);
    checkActivePowerLimits(*this, m_minP, maxP, getP0());
    m_maxP = maxP;

    return *this;
}

Battery& Battery::setMinP(double minP) {
    checkMinP(*this, minP);
    checkActivePowerLimits(*this, minP, m_maxP, getP0());
    m_minP = minP;

    return *this;
}

Battery& Battery::setP0(double p0) {
    checkP0(*this, p0);
    checkActivePowerLimits(*this, m_minP, m_maxP, p0);
    m_p0[m_network.get().getVariantIndex()] = p0;

    return *this;
}

Battery& Battery::setQ0(double q0) {
    m_q0[m_network.get().getVariantIndex()] = checkQ0(*this, q0);

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
