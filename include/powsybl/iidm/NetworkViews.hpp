/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NETWORKVIEWS_HPP
#define POWSYBL_IIDM_NETWORKVIEWS_HPP

#include <string>

#include <powsybl/iidm/Component.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class BusBreakerTopologyModel;
class Network;
class NodeBreakerTopologyModel;
class Switch;

namespace network {

class BusBreakerView {
public:
    explicit BusBreakerView(Network& network);

    ~BusBreakerView() noexcept = default;

    virtual stdcxx::CReference<Bus> getBus(const std::string& id) const;

    virtual stdcxx::Reference<Bus> getBus(const std::string& id);

    unsigned long getBusCount() const;

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    unsigned long getSwitchCount() const;

    stdcxx::const_range<Switch> getSwitches() const;

    stdcxx::range<Switch> getSwitches();

protected:
    void invalidateCache();

    friend class iidm::BusBreakerTopologyModel;

    friend class iidm::NodeBreakerTopologyModel;

protected:
    Network& m_network;
};

class BusView {
public:
    explicit BusView(Network& network);

    ~BusView() noexcept = default;

    virtual stdcxx::CReference<Bus> getBus(const std::string& id) const;

    virtual stdcxx::Reference<Bus> getBus(const std::string& id);

    unsigned long getBusCount() const;

    stdcxx::const_range<Bus> getBuses() const;

    stdcxx::range<Bus> getBuses();

    virtual stdcxx::const_range<Component> getConnectedComponents() const;

    virtual stdcxx::range<Component> getConnectedComponents();

    virtual stdcxx::const_range<Component> getSynchronousComponents() const;

    virtual stdcxx::range<Component> getSynchronousComponents();

protected:
    void invalidateCache();

    friend class iidm::BusBreakerTopologyModel;
    friend class iidm::NodeBreakerTopologyModel;

protected:
    Network& m_network;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKVIEWS_HPP
