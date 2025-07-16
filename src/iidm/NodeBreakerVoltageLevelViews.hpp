/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP

#include <powsybl/iidm/VoltageLevelViews.hpp>

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

class NodeBreakerViewImpl : public voltage_level::NodeBreakerView {
public: // NodeBreakerView
    stdcxx::CReference<BusbarSection> getBusbarSection(const std::string& bbsId) const override;

    stdcxx::Reference<BusbarSection> getBusbarSection(const std::string& bbsId) override;

    unsigned long getBusbarSectionCount() const override;

    stdcxx::const_range<BusbarSection> getBusbarSections() const override;

    stdcxx::range<BusbarSection> getBusbarSections() override;

    unsigned long getInternalConnectionCount() const override;

    stdcxx::const_range<InternalConnection> getInternalConnections() const override;

    long getMaximumNodeIndex() const override;

    unsigned long getNode1(const std::string& switchId) const override;

    unsigned long getNode2(const std::string& switchId) const override;

    stdcxx::const_range<unsigned long> getNodes() const override;

    stdcxx::CReference<Terminal> getOptionalTerminal(unsigned long node) const override;

    stdcxx::Reference<Terminal> getOptionalTerminal(unsigned long node) override;

    stdcxx::CReference<Switch> getSwitch(const std::string& switchId) const override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) override;

    unsigned long getSwitchCount() const override;

    stdcxx::const_range<Switch> getSwitches() const override;

    stdcxx::range<Switch> getSwitches() override;

    stdcxx::CReference<Terminal> getTerminal(unsigned long node) const override;

    stdcxx::Reference<Terminal> getTerminal(unsigned long node) override;

    stdcxx::CReference<Terminal> getTerminal1(const std::string& switchId) const override;

    stdcxx::Reference<Terminal> getTerminal1(const std::string& switchId) override;

    stdcxx::CReference<Terminal> getTerminal2(const std::string& switchId) const override;

    stdcxx::Reference<Terminal> getTerminal2(const std::string& switchId) override;

    bool hasAttachedEquipment(unsigned long node) const override;

    SwitchAdder newBreaker() override;

    BusbarSectionAdder newBusbarSection() override;

    SwitchAdder newDisconnector() override;

    InternalConnectionAdder newInternalConnection() override;

    SwitchAdder newSwitch() override;

    void removeInternalConnections(unsigned long node1, unsigned long node2) override;

    void removeSwitch(const std::string& switchId) override;

    void traverse(unsigned long node, const TopologyTraverser& traverser) const override;

    void traverse(stdcxx::const_range<unsigned long>& nodes, const TopologyTraverser& traverser) const override;

public:
    explicit NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    ~NodeBreakerViewImpl() noexcept override = default;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

class BusBreakerViewImpl : public voltage_level::BusBreakerView {
public: // BusBreakerView
    stdcxx::CReference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus(const std::string& busId) override;

    stdcxx::CReference<Bus> getBus1(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus1(const std::string& switchId) override;

    stdcxx::CReference<Bus> getBus2(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus2(const std::string& switchId) override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::CReference<Switch> getSwitch(const std::string& switchId) const override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) override;

    stdcxx::const_range<Switch> getSwitches() const override;

    stdcxx::range<Switch> getSwitches() override;

    unsigned long getSwitchCount() const override;

    void traverse(const Bus& bus, const TopologyTraverser& traverser) override;

public:
    explicit BusBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    ~BusBreakerViewImpl() noexcept override = default;

private: // BusBreakerView
    BusAdder newBus() override;

    SwitchAdder newSwitch() override;

    void removeAllBuses() override;

    void removeAllSwitches() override;

    void removeBus(const std::string& busId) override;

    void removeSwitch(const std::string& switchId) override;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

class BusViewImpl : public voltage_level::BusView {
public: // BusView
    stdcxx::CReference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus(const std::string& busId) override;

    stdcxx::const_range<Bus> getBuses() const override;

    stdcxx::range<Bus> getBuses() override;

    stdcxx::CReference<Bus> getMergedBus(const std::string& busbarSectionId) const override;

    stdcxx::Reference<Bus> getMergedBus(const std::string& busbarSectionId) override;

public:
    explicit BusViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    ~BusViewImpl() noexcept override = default;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP
