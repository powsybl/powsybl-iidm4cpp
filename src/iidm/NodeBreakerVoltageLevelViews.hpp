/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP

#include <powsybl/iidm/BusBreakerView.hpp>
#include <powsybl/iidm/BusView.hpp>
#include <powsybl/iidm/NodeBreakerView.hpp>

namespace powsybl {

namespace iidm {

class NodeBreakerVoltageLevel;

namespace node_breaker_voltage_level {

class NodeBreakerViewImpl : public NodeBreakerView {
public: // NodeBreakerView
    stdcxx::Reference<BusbarSection> getBusbarSection(const std::string& bbsId) const override;

    unsigned long getBusbarSectionCount() const override;

    unsigned long getNode1(const std::string& switchId) const override;

    unsigned long getNode2(const std::string& switchId) const override;

    unsigned long getNodeCount() const override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const override;

    unsigned long getSwitchCount() const override;

    stdcxx::Reference<Terminal> getTerminal(unsigned long node) const override;

    stdcxx::Reference<Terminal> getTerminal1(const std::string& switchId) const override;

    stdcxx::Reference<Terminal> getTerminal2(const std::string& switchId) const override;

    SwitchAdder newBreaker() override;

    BusbarSectionAdder newBusbarSection() override;

    SwitchAdder newDisconnector() override;

    SwitchAdder newSwitch() override;

    void removeSwitch(const std::string& switchId) override;

    void setNodeCount(unsigned long nodeCount) override;

    void traverse(unsigned long node, const Traverser& traverser) override;

public:
    explicit NodeBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    virtual ~NodeBreakerViewImpl() = default;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

class BusBreakerViewImpl : public BusBreakerView {
public: // BusBreakerView
    stdcxx::Reference<Bus> getBus(const std::string& busId) const override;

    stdcxx::Reference<Bus> getBus1(const std::string& switchId) const override;

    stdcxx::Reference<Bus> getBus2(const std::string& switchId) const override;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId) const override;

public:
    explicit BusBreakerViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    virtual ~BusBreakerViewImpl() = default;

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

class BusViewImpl : public BusView {
public:
    stdcxx::Reference<Bus> getBus(const std::string& busId) const override;

public:
    explicit BusViewImpl(NodeBreakerVoltageLevel& voltageLevel);

    virtual ~BusViewImpl() = default;

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

}

}

}

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELVIEWS_HPP
