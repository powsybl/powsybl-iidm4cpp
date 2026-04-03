/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ControlUnit.hpp>

#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ControlUnit::ControlUnit(const std::string& id, bool participate) {
    m_participate.resize(1, participate);
    if (id.empty()) {
        throw PowsyblException("Control unit id is not set");
    }
    m_id = id;
}

const std::string& ControlUnit::getId() const {
    return m_id;
}

bool ControlUnit::isParticipate() const {
    return m_participate[getVariantIndex()];
}

void ControlUnit::setParticipate(bool participate) {
    m_participate[getVariantIndex()] = participate;
}

void ControlUnit::setVariantManagerHolder(const VariantManagerHolder& variantManagerHolder) {
    m_variantManagerHolder = stdcxx::ref(variantManagerHolder);
    m_participate.resize(m_variantManagerHolder.get().getVariantManager().getVariantArraySize(), m_participate[0]);
}

unsigned long ControlUnit::getVariantIndex() const {
    return static_cast<bool>(m_variantManagerHolder) ? m_variantManagerHolder.get().getVariantIndex() : 0;
}

void ControlUnit::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    for (unsigned long index : indexes) {
        m_participate[index] = m_participate[sourceIndex];
    }
}

void ControlUnit::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void ControlUnit::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex)  {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    m_participate.resize(m_participate.size() + number, m_participate[sourceIndex]);
}

void ControlUnit::reduceVariantArraySize(unsigned long number)  {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    m_participate.resize(m_participate.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl