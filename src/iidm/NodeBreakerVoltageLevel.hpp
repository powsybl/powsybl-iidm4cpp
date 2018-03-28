/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP

#include <map>
#include <string>

#include <powsybl/iidm/VoltageLevel.hpp>
#include <powsybl/math/UndirectedGraph.hpp>

#include "NodeBreakerVoltageLevelViews.hpp"

namespace powsybl {

namespace iidm {

class NodeTerminal;
class Switch;

class NodeBreakerVoltageLevel : public VoltageLevel {
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

    const NodeBreakerView& getNodeBreakerView() const override;

    NodeBreakerView& getNodeBreakerView() override;

    const TopologyKind& getTopologyKind() const override;

    void invalidateCache() override;

public:
    NodeBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                            double nominalVoltage, double lowVoltageLimit, double highVoltagelimit);

    virtual ~NodeBreakerVoltageLevel() = default;

private:
    void checkTerminal(Terminal& terminal) const;

private:
    typedef math::UndirectedGraph<NodeTerminal, Switch> Graph;

    Graph m_graph;

    node_breaker_voltage_level::NodeBreakerViewImpl m_nodeBreakerView;

    node_breaker_voltage_level::BusBreakerViewImpl m_busBreakerView;

    node_breaker_voltage_level::BusViewImpl m_busView;
};

}

}

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVEL_HPP
