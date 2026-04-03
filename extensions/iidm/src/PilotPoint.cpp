/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/PilotPoint.hpp>

#include <powsybl/iidm/VariantManager.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

PilotPoint::PilotPoint(const std::vector<std::string>& ids, double targetV) {
    if(ids.empty()) {
        throw PowsyblException("Empty busbar section of bus ID list");
    }
    for(const auto& id : ids) {
        if(id.empty()) {
            throw PowsyblException("Empty busbar section or bus ID");
        }
    }
    if(std::isnan(targetV)){
        throw PowsyblException("Invalid pilot point target voltage");
    }
    m_busbarSectionsOrBusesIds = ids;
    m_targetV.resize(1, targetV);
}

const std::vector<std::string>& PilotPoint::getBusbarSectionsOrBusesIds() const {
    return m_busbarSectionsOrBusesIds;
}

double PilotPoint::getTargetV() const {
    return m_targetV[getVariantIndex()];
}

void PilotPoint::setTargetV(double targetV) {
    if(std::isnan(targetV)){
        throw PowsyblException("Invalid pilot point target voltage");
    }
    m_targetV[getVariantIndex()] = targetV;
}

void PilotPoint::setVariantManagerHolder(const VariantManagerHolder& variantManagerHolder) {
    m_variantManagerHolder = stdcxx::ref(variantManagerHolder);
    m_targetV.resize(m_variantManagerHolder.get().getVariantManager().getVariantArraySize(), m_targetV[0]);
}

unsigned long PilotPoint::getVariantIndex() const {
    return static_cast<bool>(m_variantManagerHolder) ? m_variantManagerHolder.get().getVariantIndex() : 0;
}

void PilotPoint::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    for (unsigned long index : indexes) {
        m_targetV[index] = m_targetV[sourceIndex];
    }
}

void PilotPoint::deleteVariantArrayElement(unsigned long /*index*/) {
    //nothing to do
}

void PilotPoint::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex)  {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    m_targetV.resize(m_targetV.size() + number, m_targetV[sourceIndex]);
}

void PilotPoint::reduceVariantArraySize(unsigned long number)  {
    if(!static_cast<bool>(m_variantManagerHolder)) {
        return;
    }
    m_targetV.resize(m_targetV.size() - number);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl