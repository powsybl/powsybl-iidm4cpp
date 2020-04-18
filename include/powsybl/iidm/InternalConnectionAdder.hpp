/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INTERNALCONNECTIONADDER_HPP
#define POWSYBL_IIDM_INTERNALCONNECTIONADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace node_breaker_view {

class InternalConnectionAdder {
public:
    explicit InternalConnectionAdder(VoltageLevel& voltageLevel);

    ~InternalConnectionAdder() noexcept = default;

    void add();

    InternalConnectionAdder& setNode1(unsigned long node1);

    InternalConnectionAdder& setNode2(unsigned long node2);

private:
    VoltageLevel& m_voltageLevel;

    stdcxx::optional<unsigned long> m_node1;

    stdcxx::optional<unsigned long> m_node2;
};

}  // namespace node_breaker_view

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_INTERNALCONNECTIONADDER_HPP
