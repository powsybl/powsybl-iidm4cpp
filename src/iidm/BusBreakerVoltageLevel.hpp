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

#include <powsybl/iidm/VariantArray.hpp>
#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/math/UndirectedGraph.hpp>
#include <powsybl/stdcxx/optional.hpp>

#include "BusBreakerVoltageLevelVariant.hpp"
#include "BusBreakerVoltageLevelViews.hpp"
#include "BusTerminalViews.hpp"

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel : public VoltageLevel {
public:
    using Graph = math::UndirectedGraph<ConfiguredBus, Switch>;

public: // VoltageLevel
    void attach(Terminal& terminal, bool test) override;

    bool connect(Terminal& terminal) override;

    void detach(Terminal& terminal) override;

    bool disconnect(Terminal& terminal) override;

    const BusBreakerView& getBusBreakerView() const override;

    BusBreakerView& getBusBreakerView() override;

    const BusView& getBusView() const override;

    BusView& getBusView() override;

    unsigned long getSwitchCount() const override;

    stdcxx::const_range<Switch> getSwitches() const override;

    stdcxx::range<Switch> getSwitches() override;

    const TopologyKind& getTopologyKind() const override;

    void invalidateCache() override;

public:
    BusBreakerVoltageLevel(const std::string& id, const std::string& name, bool fictitious, Substation& substation,
                           double nominalV, double lowVoltageLimit, double highVoltagelimit);

    ~BusBreakerVoltageLevel() noexcept override = default;

    Bus& addBus(std::unique_ptr<ConfiguredBus>&& ptrBus);

    Switch& addSwitch(std::unique_ptr<Switch>&& ptrSwitch, const std::string& busId1, const std::string& busId2);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected: // VoltageLevel
    stdcxx::const_range<Terminal> getTerminals() const override;

    stdcxx::range<Terminal> getTerminals() override;

private: // VoltageLevel
    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

private:
    void checkTerminal(Terminal& terminal) const;

    bus_breaker_voltage_level::CalculatedBusTopology& getCalculatedBusTopology();

    stdcxx::CReference<ConfiguredBus> getConfiguredBus(const std::string& busId, bool throwException) const;

    stdcxx::Reference<ConfiguredBus> getConfiguredBus(const std::string& busId, bool throwException);

    stdcxx::Reference<ConfiguredBus> getConfiguredBus1(const std::string& switchId);

    stdcxx::Reference<ConfiguredBus> getConfiguredBus2(const std::string& switchId);

    const Graph& getGraph() const;

    stdcxx::optional<unsigned long> getEdge(const std::string& switchId, bool throwException) const;

    stdcxx::Reference<MergedBus> getMergedBus(const std::string& busId, bool throwException);

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId, bool throwException);

    stdcxx::optional<unsigned long> getVertex(const std::string& busId, bool throwException) const;

    void removeAllBuses();

    void removeAllSwitches();

    void removeBus(const std::string& busId);

    void removeSwitch(const std::string& switchId);

    void removeTopology() override;

private:
    friend class bus_breaker_voltage_level::BusBreakerViewImpl;

    friend class bus_breaker_voltage_level::BusViewImpl;

    friend class bus_breaker_voltage_level::CalculatedBusTopology;

    friend class bus_terminal::BusBreakerViewImpl;

    friend class bus_terminal::BusViewImpl;

private:
    Graph m_graph;

    std::map<std::string, unsigned long> m_buses;

    std::map<std::string, unsigned long> m_switches;

    bus_breaker_voltage_level::VariantArray m_variants;

    bus_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    bus_breaker_voltage_level::BusViewImpl m_busView;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
