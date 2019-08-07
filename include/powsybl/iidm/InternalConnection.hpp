/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INTERNALCONNECTION_HPP
#define POWSYBL_IIDM_INTERNALCONNECTION_HPP

namespace powsybl {

namespace iidm {

namespace node_breaker_view {

class InternalConnection {
public:
    InternalConnection(unsigned long node1, unsigned long node2);

    InternalConnection(const InternalConnection& internalConnection) = default;

    ~InternalConnection() = default;

    unsigned long getNode1();

    unsigned long getNode2();

private:
    unsigned long m_node1;

    unsigned long m_node2;
};

}  // namespace node_breaker_view

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INTERNALCONNECTION_HPP
