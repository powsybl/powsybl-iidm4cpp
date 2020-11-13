/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/BusbarSectionPosition.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BusbarSectionPosition::BusbarSectionPosition(BusbarSection& busbarSection, unsigned long busbarIndex, unsigned long sectionIndex) :
    Extension(busbarSection),
    m_busbarIndex(busbarIndex),
    m_sectionIndex(sectionIndex) {
}

void BusbarSectionPosition::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<BusbarSection>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<BusbarSection>()));
    }
}

unsigned long BusbarSectionPosition::getBusbarIndex() const {
    return m_busbarIndex;
}

const std::string& BusbarSectionPosition::getName() const {
    static std::string s_name = "busbarSectionPosition";
    return s_name;
}

unsigned long BusbarSectionPosition::getSectionIndex() const {
    return m_sectionIndex;
}

const std::type_index& BusbarSectionPosition::getType() const {
    static std::type_index s_type = typeid(BusbarSectionPosition);
    return s_type;
}

BusbarSectionPosition& BusbarSectionPosition::setBusbarIndex(unsigned long busbarIndex) {
    m_busbarIndex = busbarIndex;
    return *this;
}

BusbarSectionPosition& BusbarSectionPosition::setSectionIndex(unsigned long sectionIndex) {
    m_sectionIndex = sectionIndex;
    return *this;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
