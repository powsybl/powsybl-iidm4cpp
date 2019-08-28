/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_COMPONENT_HPP
#define POWSYBL_IIDM_COMPONENT_HPP

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class Network;

class Component {
public:
    Component(const Component&) = delete;

    Component(Component&&) = delete;

    virtual ~Component() noexcept = default;

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    long getNum() const;

    unsigned long getSize() const;

protected:
    Component(long num, unsigned long size, Network& network);

    virtual stdcxx::CReference<Component> getFilterComponent(const Bus& bus) const = 0;

private:
    long m_num;

    unsigned long m_size;

    Network& m_network;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_COMPONENT_HPP
