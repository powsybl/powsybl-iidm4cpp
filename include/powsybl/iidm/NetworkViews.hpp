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
class BusBreakerVoltageLevel;
class Network;
class NodeBreakerVoltageLevel;
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
    void invalidateCache();

    friend class iidm::BusBreakerVoltageLevel;

    friend class iidm::NodeBreakerVoltageLevel;

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

    stdcxx::const_range<Component> getConnectedComponents() const;

    stdcxx::range<Component> getConnectedComponents();

    stdcxx::const_range<Component> getSynchronousComponents() const;

    stdcxx::range<Component> getSynchronousComponents();

private:
    void invalidateCache();

    friend class iidm::BusBreakerVoltageLevel;

    friend class iidm::NodeBreakerVoltageLevel;

private:
    Network& m_network;
};

}  // namespace network

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NETWORKVIEWS_HPP
