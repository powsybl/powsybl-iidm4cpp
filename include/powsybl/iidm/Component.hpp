/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_COMPONENT_HPP
#define POWSYBL_IIDM_COMPONENT_HPP

#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class DcBus;
class Network;

class Component {
public:
    Component(Network& network, unsigned long num, unsigned long size);

    Component(const Component&) = delete;

    Component(Component&&) noexcept = delete;

    virtual ~Component() noexcept = default;

    Component& operator=(const Component&) = delete;

    Component& operator=(Component&&) noexcept = delete;

    virtual stdcxx::const_range<Bus> getBuses() const;

    virtual stdcxx::range<Bus> getBuses();

    virtual stdcxx::const_range<DcBus> getDcBuses() const;

    virtual stdcxx::range<DcBus> getDcBuses();

    unsigned long getNum() const;

    unsigned long getSize() const;

protected:
    virtual bool checkBus(const Bus& bus) const = 0;
    virtual bool checkDcBus(const DcBus& dcBus) const = 0;

private:
    Network& m_network;

    unsigned long m_num;

    unsigned long m_size;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_COMPONENT_HPP
