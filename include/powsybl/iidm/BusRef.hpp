/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSREF_HPP
#define POWSYBL_IIDM_BUSREF_HPP

#include <powsybl/iidm/TopologyLevel.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;
class Terminal;

class BusRef {
public:
    BusRef() = default;

    BusRef(const BusRef&) = default;

    BusRef(BusRef&&) noexcept = default;

    virtual ~BusRef() noexcept = default;

    BusRef& operator=(const BusRef&) = default;

    BusRef& operator=(BusRef&&) noexcept = default;

    stdcxx::CReference<Bus> resolve(const Network& network, const TopologyLevel& level);

protected:
    static stdcxx::CReference<Bus> chooseBusByLevel(const Terminal& terminal, const TopologyLevel& level);

protected:
    virtual stdcxx::CReference<Bus> resolveByLevel(const Network& network, const TopologyLevel& level) = 0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSREF_HPP
