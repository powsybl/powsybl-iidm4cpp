/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Load.hpp>

#include <powsybl/iidm/StateManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

Load::Load(MultiStateObject& network, const std::string& id, const std::string& name, const LoadType& loadType,
           double p0, double q0) :
    Injection(id, name, ConnectableType::LOAD),
    m_network(network),
    m_loadType(loadType),
    m_p0(network.getStateManager().getStateArraySize(), checkP0(*this, p0)),
    m_q0(network.getStateManager().getStateArraySize(), checkQ0(*this, q0)) {

}

const std::string& Load::getTypeDescription() const {
    static std::string s_typeDescription = "Load";

    return s_typeDescription;
}

void Load::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateStateArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_p0[index] = m_p0[sourceIndex];
        m_q0[index] = m_q0[sourceIndex];
    }
}

void Load::deleteStateArrayElement(unsigned long index) {
    Injection::deleteStateArrayElement(index);
}

void Load::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendStateArraySize(initStateArraySize, number, sourceIndex);

    m_p0.resize(m_p0.size() + number, m_p0[sourceIndex]);
    m_q0.resize(m_q0.size() + number, m_q0[sourceIndex]);
}

const LoadType& Load::getLoadType() const {
    return m_loadType;
}

double Load::getP0() const {
    return m_p0.at(m_network.get().getStateIndex());
}

double Load::getQ0() const {
    return m_q0[m_network.get().getStateIndex()];
}

void Load::reduceStateArraySize(unsigned long number) {
    Injection::reduceStateArraySize(number);

    m_p0.resize(m_p0.size() - number);
    m_q0.resize(m_q0.size() - number);
}

Load& Load::setP0(double p0) {
    m_p0[m_network.get().getStateIndex()] = checkP0(*this, p0);

    return *this;
}

Load& Load::setQ0(double q0) {
    m_q0[m_network.get().getStateIndex()] = checkQ0(*this, q0);

    return *this;
}

}

}
