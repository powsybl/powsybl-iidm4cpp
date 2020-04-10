/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensator.hpp>
#include <powsybl/iidm/VariantManager.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

ShuntCompensator::ShuntCompensator(VariantManagerHolder& network, const std::string& id, const std::string& name,
    double bPerSection, unsigned long maximumSectionCount, unsigned long currentSectionCount) :
    Injection(id, name, ConnectableType::SHUNT_COMPENSATOR),
    m_bPerSection(checkbPerSection(*this, bPerSection)),
    m_maximumSectionCount(maximumSectionCount),
    m_currentSectionCount(network.getVariantManager().getVariantArraySize(), currentSectionCount) {
    checkSections(*this, currentSectionCount, maximumSectionCount);
}

void ShuntCompensator::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Injection::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_currentSectionCount[index] = m_currentSectionCount[sourceIndex];
    }
}

void ShuntCompensator::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Injection::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_currentSectionCount.resize(m_currentSectionCount.size() + number, m_currentSectionCount[sourceIndex]);
}

double ShuntCompensator::getbPerSection() const {
    return m_bPerSection;
}

double ShuntCompensator::getCurrentB() const {
    return m_bPerSection * getCurrentSectionCount();
}

unsigned long ShuntCompensator::getCurrentSectionCount() const {
    return m_currentSectionCount.at(getNetwork().getVariantIndex());
}

double ShuntCompensator::getMaximumB() const {
    return m_bPerSection * m_maximumSectionCount;
}

unsigned long ShuntCompensator::getMaximumSectionCount() const {
    return m_maximumSectionCount;
}

const std::string& ShuntCompensator::getTypeDescription() const {
    static std::string s_typeDescription = "Shunt compensator";

    return s_typeDescription;
}

void ShuntCompensator::reduceVariantArraySize(unsigned long number) {
    Injection::reduceVariantArraySize(number);

    m_currentSectionCount.resize(m_currentSectionCount.size() - number);
}

ShuntCompensator& ShuntCompensator::setbPerSection(double bPerSection) {
    m_bPerSection = checkbPerSection(*this, bPerSection);

    return *this;
}

ShuntCompensator& ShuntCompensator::setCurrentSectionCount(unsigned long currentSectionCount) {
    checkSections(*this, currentSectionCount, m_maximumSectionCount);
    m_currentSectionCount[getNetwork().getVariantIndex()] = currentSectionCount;

    return *this;
}

ShuntCompensator& ShuntCompensator::setMaximumSectionCount(unsigned long maximumSectionCount) {
    checkSections(*this, getCurrentSectionCount(), maximumSectionCount);
    m_maximumSectionCount = maximumSectionCount;

    return *this;
}

}  // namespace iidm

}  // namespace powsybl
