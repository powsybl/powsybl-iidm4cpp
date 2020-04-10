/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkRef.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

NetworkRef::NetworkRef(Network& network) :
    m_pointer(&network) {
}

const Network& NetworkRef::get() const {
    if (m_pointer == nullptr) {
        throw AssertionError("m_pointer is null");
    }
    return *m_pointer;
}

Network& NetworkRef::get() {
    if (m_pointer == nullptr) {
        throw AssertionError("m_pointer is null");
    }
    return *m_pointer;
}

void NetworkRef::set(Network& network) {
    m_pointer = &network;
}

}  // namespace iidm

}  // namespace powsybl
