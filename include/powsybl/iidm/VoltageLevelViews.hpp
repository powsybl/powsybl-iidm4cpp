/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVELVIEWS_HPP
#define POWSYBL_IIDM_VOLTAGELEVELVIEWS_HPP

#include <functional>
#include <string>
#include <vector>

#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/BusbarSectionAdder.hpp>
#include <powsybl/iidm/InternalConnection.hpp>
#include <powsybl/iidm/InternalConnectionAdder.hpp>
#include <powsybl/iidm/SwitchAdder.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class BusbarSection;
class Switch;
class Terminal;

namespace voltage_level {

class BusBreakerView {
public:
    using SwitchAdder = bus_breaker_view::SwitchAdder;

public:
    virtual ~BusBreakerView() noexcept = default;

    virtual stdcxx::Reference<Bus> getBus(const std::string& busId) const = 0;

    virtual stdcxx::Reference<Bus> getBus1(const std::string& switchId) const = 0;

    virtual stdcxx::Reference<Bus> getBus2(const std::string& switchId) const = 0;

    virtual stdcxx::const_range<Bus> getBuses() const = 0;

    virtual stdcxx::range<Bus> getBuses() = 0;

    virtual stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const = 0;

    virtual unsigned long getSwitchCount() const = 0;

    virtual stdcxx::const_range<Switch> getSwitches() const = 0;

    virtual stdcxx::range<Switch> getSwitches() = 0;

    virtual BusAdder newBus() = 0;

    virtual SwitchAdder newSwitch() = 0;

    virtual void removeAllBuses() = 0;

    virtual void removeAllSwitches() = 0;

    virtual void removeBus(const std::string& busId) = 0;

    virtual void removeSwitch(const std::string& switchId) = 0;
};

class BusView {
public:
    virtual ~BusView() noexcept = default;

    virtual stdcxx::Reference<Bus> getBus(const std::string& busId) const = 0;

    virtual stdcxx::const_range<Bus> getBuses() const = 0;

    virtual stdcxx::range<Bus> getBuses() = 0;

    virtual stdcxx::Reference<Bus> getMergedBus(const std::string& configuredBusId) const = 0;
};

class NodeBreakerView {
public:
    using InternalConnection = node_breaker_view::InternalConnection;

    using InternalConnectionAdder = node_breaker_view::InternalConnectionAdder;

    using SwitchAdder = node_breaker_view::SwitchAdder;

    using Traverser = std::function<bool(unsigned long node1, const stdcxx::Reference<Switch>& sw, unsigned long node2)>;

public:
    virtual ~NodeBreakerView() noexcept = default;

    virtual stdcxx::Reference<BusbarSection> getBusbarSection(const std::string& bbsId) const = 0;

    virtual unsigned long getBusbarSectionCount() const = 0;

    virtual stdcxx::const_range<BusbarSection> getBusbarSections() const = 0;

    virtual stdcxx::range<BusbarSection> getBusbarSections() = 0;

    virtual unsigned long getInternalConnectionCount() const = 0;

    virtual stdcxx::const_range<InternalConnection> getInternalConnections() const = 0;

    virtual stdcxx::range<InternalConnection> getInternalConnections() = 0;

    virtual unsigned long getNode1(const std::string& switchId) const = 0;

    virtual unsigned long getNode2(const std::string& switchId) const = 0;

    virtual unsigned long getNodeCount() const = 0;

    //TODO(thiebarr) getNodes

    virtual stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const = 0;

    virtual unsigned long getSwitchCount() const = 0;

    virtual stdcxx::const_range<Switch> getSwitches() const = 0;

    virtual stdcxx::range<Switch> getSwitches() = 0;

    virtual stdcxx::Reference<Terminal> getTerminal(unsigned long node) const = 0;

    virtual stdcxx::Reference<Terminal> getTerminal1(const std::string& switchId) const = 0;

    virtual stdcxx::Reference<Terminal> getTerminal2(const std::string& switchId) const = 0;

    virtual SwitchAdder newBreaker() = 0;

    virtual BusbarSectionAdder newBusbarSection() = 0;

    virtual SwitchAdder newDisconnector() = 0;

    virtual InternalConnectionAdder newInternalConnection() = 0;

    virtual SwitchAdder newSwitch() = 0;

    virtual void removeSwitch(const std::string& switchId) = 0;

    virtual NodeBreakerView& setNodeCount(unsigned long nodeCount) = 0;

    virtual void traverse(unsigned long node, const Traverser& traverser) const = 0;
};

}  // namespace voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VOLTAGELEVELVIEWS_HPP
