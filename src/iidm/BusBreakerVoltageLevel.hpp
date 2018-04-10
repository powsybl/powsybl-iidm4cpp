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

#include "BusBreakerVoltageLevelViews.hpp"

namespace powsybl {

namespace iidm {

class Bus;
class Switch;
class ConfiguredBus;

class BusBreakerVoltageLevel : public VoltageLevel {
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

    virtual ~BusBreakerVoltageLevel() = default;

    Bus& addBus(std::unique_ptr<ConfiguredBus>&& bus);

private: // VoltageLevel
    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

private:
    void checkTerminal(Terminal& terminal) const;

    stdcxx::Reference<ConfiguredBus> getBus(const std::string& busId, bool throwException) const;

    stdcxx::optional<unsigned long> getVertex(const std::string& busId, bool throwException) const;

private:
    typedef math::UndirectedGraph<ConfiguredBus, Switch> Graph;

    Graph m_graph;

    std::map<std::string, unsigned long> m_buses;

    bus_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    bus_breaker_voltage_level::BusViewImpl m_busView;
};

}

}

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
