/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELTOPOLOGY_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELTOPOLOGY_HPP

#include <functional>
#include <memory>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

#include "NodeBreakerVoltageLevelBusCache.hpp"
#include "NodeBreakerVoltageLevelGraph.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

class graph;

class CalculatedBusTopology {
public:
    explicit CalculatedBusTopology(NodeBreakerVoltageLevel& voltageLevel);

    virtual ~CalculatedBusTopology() noexcept = default;

    stdcxx::Reference<CalculatedBus> getBus(unsigned long node);

    stdcxx::Reference<CalculatedBus> getBus(const std::string& id, bool throwException);

    stdcxx::range<CalculatedBus> getBuses();

    stdcxx::Reference<Bus> getConnectableBus(unsigned long node);

    void invalidateCache();

    void updateCache();

protected:
    using SwitchPredicate = std::function<bool(const stdcxx::Reference<Switch>& a)>;

protected:
    const NodeBreakerVoltageLevel& getVoltageLevel() const;

    NodeBreakerVoltageLevel& getVoltageLevel();

private:
    virtual SwitchPredicate createSwitchPredicate() const;

    virtual bool isBusValid(const typename node_breaker_voltage_level::Graph& graph, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& terminals) const;

    void traverse(unsigned long v, std::vector<bool>& encountered, const CalculatedBusTopology::SwitchPredicate& terminate, BusCache::CalculatedBusById& busById, BusCache::CalculatedBusByNode& busByNode);

    void updateCache(const SwitchPredicate& predicate);

private:
    NodeBreakerVoltageLevel& m_voltageLevel;

    std::unique_ptr<BusCache> m_cache;
};

class CalculatedBusBreakerTopology : public CalculatedBusTopology {
public:
    explicit CalculatedBusBreakerTopology(NodeBreakerVoltageLevel& voltageLevel);

    ~CalculatedBusBreakerTopology() noexcept override = default;

    stdcxx::Reference<CalculatedBus> getBus1(const std::string& switchId, bool throwException);

    stdcxx::Reference<CalculatedBus> getBus2(const std::string& switchId, bool throwException);

    stdcxx::Reference<Switch> getSwitch(const std::string& switchId, bool throwException);

private: // CalculatedBusTopology
    SwitchPredicate createSwitchPredicate() const override;

    stdcxx::Reference<Switch> getRetainedSwitch(const stdcxx::optional<unsigned long>& e) const;

    bool isBusValid(const node_breaker_voltage_level::Graph& graph, const std::vector<unsigned long>& vertices, const std::vector<std::reference_wrapper<NodeTerminal> >& terminals) const override;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELTOPOLOGY_HPP
