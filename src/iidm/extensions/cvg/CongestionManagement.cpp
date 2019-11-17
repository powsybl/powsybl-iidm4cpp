/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/CongestionManagement.hpp>

#include <powsybl/iidm/VariantManager.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

CongestionManagement::CongestionManagement(Battery& battery, bool enabled) :
    AbstractMultiVariantConnectableExtension(battery),
    m_enabled(getVariantManagerHolder(battery).getVariantManager().getVariantArraySize(), enabled) {
}

void CongestionManagement::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_enabled[index] = m_enabled[sourceIndex];
    }
}

void CongestionManagement::assertExtendable(const stdcxx::Reference<powsybl::iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Battery>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Battery>()));
    }
}

void CongestionManagement::deleteVariantArrayElement(unsigned long /*index*/) {
    // Nothing to do
}

void CongestionManagement::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_enabled.resize(m_enabled.size() + number, m_enabled[sourceIndex]);
}

const std::string& CongestionManagement::getName() const {
    static std::string s_name = "congestionManagement";
    return s_name;
}

const std::type_index& CongestionManagement::getType() const {
    static std::type_index s_type = typeid(CongestionManagement);
    return s_type;
}

bool CongestionManagement::isEnabled() const {
    return m_enabled[getVariantIndex()];
}

CongestionManagement& CongestionManagement::setEnabled(bool enabled) {
    m_enabled[getVariantIndex()] = enabled;
    return *this;
}

void CongestionManagement::reduceVariantArraySize(unsigned long number) {
    m_enabled.resize(m_enabled.size() - number);
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

