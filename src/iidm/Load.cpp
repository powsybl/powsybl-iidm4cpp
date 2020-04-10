/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Load.hpp>

#include <powsybl/iidm/VariantManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Load::Load(VariantManagerHolder& network, const std::string& id, const std::string& name, const LoadType& loadType,
           double p0, double q0) :
    Injection(id, name, ConnectableType::LOAD),
    m_loadType(checkLoadType(*this, loadType)),
    m_p0(network.getVariantManager().getVariantArraySize(), checkP0(*this, p0)),
    m_q0(network.getVariantManager().getVariantArraySize(), checkQ0(*this, q0)) {

}

void Load::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_q0[index] = m_q0[sourceIndex];
    }
}

void Load::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_q0.resize(m_q0.size() + number, m_q0[sourceIndex]);
}

const LoadType& Load::getLoadType() const {
    return m_loadType;
}

double Load::getP0() const {
    return m_p0.at(getNetwork().getVariantIndex());
}

double Load::getQ0() const {
    return m_q0[getNetwork().getVariantIndex()];
}

const std::string& Load::getTypeDescription() const {
    static std::string s_typeDescription = "Load";

    return s_typeDescription;
}

void Load::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_p0.resize(m_p0.size() - number);
    m_q0.resize(m_q0.size() - number);
}

Load& Load::setLoadType(const LoadType& loadType) {
    m_loadType = checkLoadType(*this, loadType);

    return *this;
}

Load& Load::setP0(double p0) {
    m_p0[getNetwork().getVariantIndex()] = checkP0(*this, p0);

    return *this;
}

Load& Load::setQ0(double q0) {
    m_q0[getNetwork().getVariantIndex()] = checkQ0(*this, q0);

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
