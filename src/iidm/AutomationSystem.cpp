/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/AutomationSystem.hpp>

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

AutomationSystem::AutomationSystem(Network& network, const std::string& id, const std::string& name, bool fictitious, bool enabled) :
    Identifiable(id, name, fictitious),
    m_enabled(network.getVariantManager().getVariantArraySize(), enabled) {
}

void AutomationSystem::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    Identifiable::allocateVariantArrayElement(indexes, sourceIndex);

    for (auto index : indexes) {
        m_enabled[index] = m_enabled[sourceIndex];
    }
}
void AutomationSystem::extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) {
    Identifiable::extendVariantArraySize(initVariantArraySize, number, sourceIndex);

    m_enabled.resize(m_enabled.size() + number, m_enabled[sourceIndex]);
}
void AutomationSystem::reduceVariantArraySize(unsigned long number) {
    Identifiable::reduceVariantArraySize(number);

    m_enabled.resize(m_enabled.size() - number);
}

bool AutomationSystem::isEnabled() const {
    return m_enabled[getNetwork().getVariantIndex()];
}

void AutomationSystem::setEnabled(bool enabled) {
    m_enabled[getNetwork().getVariantIndex()] = enabled;
}

}  // namespace iidm

}  // namespace powsybl
