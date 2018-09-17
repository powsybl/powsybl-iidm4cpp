/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELSTATE_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELSTATE_HPP

#include "NodeBreakerVoltageLevelTopology.hpp"
#include "State.hpp"

namespace powsybl {

namespace iidm {

namespace node_breaker_voltage_level {

class StateImpl : public State<StateImpl> {
public: // State
    std::unique_ptr<StateImpl> copy() const override;

public:
    explicit StateImpl(NodeBreakerVoltageLevel& voltageLevel);

    ~StateImpl() = default;

    CalculatedBusBreakerTopology& getCalculatedBusBreakerTopology();

    CalculatedBusTopology& getCalculatedBusTopology();

private:
    NodeBreakerVoltageLevel& m_voltageLevel;

    CalculatedBusBreakerTopology m_calculatedBusBreakerTopology;

    CalculatedBusTopology m_calculatedBusTopology;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELSTATE_HPP
