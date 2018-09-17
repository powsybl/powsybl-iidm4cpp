/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELSTATE_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELSTATE_HPP

#include "BusBreakerVoltageLevelTopology.hpp"
#include "State.hpp"

namespace powsybl {

namespace iidm {

namespace bus_breaker_voltage_level {

class StateImpl : public State<StateImpl> {
public: // State
    std::unique_ptr<StateImpl> copy() const override;

public:
    explicit StateImpl(BusBreakerVoltageLevel& voltageLevel);

    ~StateImpl() noexcept = default;

    CalculatedBusTopology& getCalculatedBusTopology();

private:
    BusBreakerVoltageLevel& m_voltageLevel;

    CalculatedBusTopology m_calculatedBusTopology;
};

}  // namespace bus_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVELSTATE_HPP
