/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/sld/BusbarSectionPositionAdder.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/BusbarSection.hpp>
#include <powsybl/iidm/extensions/sld/BusbarSectionPosition.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

BusbarSectionPositionAdder::BusbarSectionPositionAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> BusbarSectionPositionAdder::createExtension(Extendable& extendable) {
    if (!m_busbarIndex) {
        throw PowsyblException("Undefined value for busbar index");
    }
    if (!m_sectionIndex) {
        throw PowsyblException("Undefined value for section index");
    }
    if (stdcxx::isInstanceOf<BusbarSection>(extendable)) {
        return std::unique_ptr<BusbarSectionPosition>(new BusbarSectionPosition(dynamic_cast<BusbarSection&>(extendable), *m_busbarIndex, *m_sectionIndex));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<BusbarSection>()));
}

BusbarSectionPositionAdder& BusbarSectionPositionAdder::withBusbarIndex(unsigned long busbarIndex) {
    m_busbarIndex = busbarIndex;
    return *this;
}

BusbarSectionPositionAdder& BusbarSectionPositionAdder::withSectionIndex(unsigned long sectionIndex) {
    m_sectionIndex = sectionIndex;
    return *this;
}

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
