/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/stdcxx/MessageFormat.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

Extension::Extension(powsybl::iidm::Extendable& extendable) :
    m_extendable(extendable) {
}

void Extension::setExtendable(powsybl::iidm::Extendable& extendable) {
    setExtendable(stdcxx::ref(extendable));
}

void Extension::setExtendable(const stdcxx::Reference<Extendable>& extendable) {
    assertExtendable(extendable);

    if (m_extendable && extendable && !stdcxx::areSame(m_extendable.get(), extendable.get())) {
        throw PowsyblException(stdcxx::format("Extension is already associated to the extendable %1%", m_extendable));
    }
    m_extendable = extendable;
}

}  // namespace iidm

}  // namespace powsybl
