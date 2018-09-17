/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP

#include <map>
#include <string>

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/optional.hpp>

#include "BusBreakerVoltageLevelState.hpp"
#include "BusBreakerVoltageLevelViews.hpp"
#include "StateArray.hpp"

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel : public VoltageLevel {
public:
    typedef math::UndirectedGraph<ConfiguredBus, Switch> Graph;

public: // VoltageLevel
    void attach(Terminal& terminal, bool test) override;

    void clean() override;

    bool connect(Terminal& terminal) override;

    void detach(Terminal& terminal) override;

    bool disconnect(Terminal& terminal) override;

    const BusBreakerView& getBusBreakerView() const override;

    BusBreakerView& getBusBreakerView() override;

    const BusView& getBusView() const override;

    BusView& getBusView() override;

    const TopologyKind& getTopologyKind() const override;

    void invalidateCache() override;

public:
    BusBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                           double nominalVoltage, double lowVoltageLimit, double highVoltagelimit);

    ~BusBreakerVoltageLevel() noexcept override = default;

    Bus& addBus(std::unique_ptr<ConfiguredBus>&& ptrBus);

    Switch& addSwitch(std::unique_ptr<Switch>&& ptrSwitch, const std::string& busId1, const std::string& busId2);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

private: // VoltageLevel
    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

private:
    void checkTerminal(Terminal& terminal) const;

    bus_breaker_voltage_level::CalculatedBusTopology& getCalculatedBusTopology();

    stdcxx::Reference<ConfiguredBus> getConfiguredBus(const std::string& busId, bool throwException) const;

    stdcxx::Reference<ConfiguredBus> getConfiguredBus1(const std::string& switchId) const;

    stdcxx::Reference<ConfiguredBus> getConfiguredBus2(const std::string& switchId) const;

    const Graph& getGraph() const;

    stdcxx::optional<unsigned long> getEdge(const std::string& switchId, bool throwException) const;

    stdcxx::Reference<MergedBus> getMergedBus(const std::string& busId, bool throwException) const;

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId, bool throwException) const;

    stdcxx::optional<unsigned long> getVertex(const std::string& busId, bool throwException) const;

    void removeAllBuses();

    void removeAllSwitches();

    void removeBus(const std::string& busId);

    void removeSwitch(const std::string& switchId);

private:
    friend class bus_breaker_voltage_level::BusBreakerViewImpl;

    friend class bus_breaker_voltage_level::BusViewImpl;

    friend class bus_breaker_voltage_level::CalculatedBusTopology;

private:
    Graph m_graph;

    std::map<std::string, unsigned long> m_buses;

    std::map<std::string, unsigned long> m_switches;

    StateArray<bus_breaker_voltage_level::StateImpl> m_states;

    bus_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    bus_breaker_voltage_level::BusViewImpl m_busView;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
