/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKVIEWS_HPP
#define POWSYBL_IIDM_NETWORKVIEWS_HPP

#include <string>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class ConnectedComponent;
class Network;
class Switch;

namespace network {

class BusBreakerView {
public:
    explicit BusBreakerView(Network& network);

    ~BusBreakerView() noexcept = default;

    stdcxx::CReference<Bus> getBus(const std::string& id) const;

    stdcxx::Reference<Bus> getBus(const std::string& id);

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    unsigned long getSwitchCount() const;

    stdcxx::const_range<Switch> getSwitches() const;

    stdcxx::range<Switch> getSwitches();

private:
    Network& m_network;
};

class BusView {
public:
    explicit BusView(Network& network);

    ~BusView() noexcept = default;

    stdcxx::CReference<Bus> getBus(const std::string& id) const;

    stdcxx::Reference<Bus> getBus(const std::string& id);

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    stdcxx::const_range<ConnectedComponent> getConnectedComponents() const;

    stdcxx::range<ConnectedComponent> getConnectedComponents();

private:
    Network& m_network;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKVIEWS_HPP
