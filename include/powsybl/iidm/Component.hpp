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
class Network;

class Component {
public:
    Component(Network& network, unsigned long num, unsigned long size);

    Component(const Component& component) = delete;

    Component(Component&& component) noexcept = delete;

    virtual ~Component() noexcept = default;

    Component& operator=(const Component& component) = delete;

    Component& operator=(Component&& component) noexcept = delete;

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    unsigned long getNum() const;

    unsigned long getSize() const;

private:
    virtual bool checkBus(const Bus& bus) const = 0;

private:
    Network& m_network;

    unsigned long m_num;

    unsigned long m_size;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_COMPONENT_HPP
