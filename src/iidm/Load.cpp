/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Load.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/iidm/VariantManager.hpp>

namespace powsybl {

namespace iidm {

Load::Load(VariantManagerHolder& network, const std::string& id, const std::string& name, bool fictitious, const LoadType& loadType,
           double p0, double q0) :
    Injection(id, name, fictitious),
    m_loadType(checkLoadType(*this, loadType)),
    m_p0(network.getVariantManager().getVariantArraySize(), p0),
    m_q0(network.getVariantManager().getVariantArraySize(), q0) {
    ValidationLevel vl = ValidationLevel::STEADY_STATE_HYPOTHESIS;
    if (stdcxx::isInstanceOf<Network>(network)) {
        auto& n = dynamic_cast<Network&>(network);
        vl = n.getMinimumValidationLevel();
    }
    checkP0(*this, p0, vl);
    checkQ0(*this, q0, vl);
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

const IdentifiableType& Load::getType() const {
    static IdentifiableType s_type = IdentifiableType::LOAD;
    return s_type;
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
    checkP0(*this, p0, getNetwork().getMinimumValidationLevel());
    m_p0[getNetwork().getVariantIndex()] = p0;
    getNetwork().invalidateValidationLevel();
    return *this;
}

Load& Load::setQ0(double q0) {
    checkQ0(*this, q0, getNetwork().getMinimumValidationLevel());
    m_q0[getNetwork().getVariantIndex()] = q0;
    getNetwork().invalidateValidationLevel();
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
