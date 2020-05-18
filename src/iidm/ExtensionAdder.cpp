/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/Extendable.hpp>

namespace powsybl {

namespace iidm {

ExtensionAdder::ExtensionAdder(Extendable& extendable) :
    m_extendable(extendable) {
}

Extendable& ExtensionAdder::add() {
    m_extendable.addExtension(createExtension(m_extendable));
    return m_extendable;
}

}  // namespace iidm

}  // namespace powsybl
