/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/InternalConnection.hpp>

namespace powsybl {

namespace iidm {

namespace node_breaker_view {

InternalConnection::InternalConnection(unsigned long node1, unsigned long node2) :
    m_node1(node1),
    m_node2(node2) {
}

unsigned long InternalConnection::getNode1() {
    return m_node1;
}

unsigned long InternalConnection::getNode2() {
    return m_node2;
}

}  // namespace node_breaker_view

}  // namespace iidm

}  // namespace powsybl
