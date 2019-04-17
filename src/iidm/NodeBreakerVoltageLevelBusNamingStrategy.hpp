/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP

#include <atomic>
#include <string>

namespace powsybl {

namespace iidm {

class NodeBreakerVoltageLevel;

namespace node_breaker_voltage_level {

class BusNamingStrategy {
public:
    explicit BusNamingStrategy(NodeBreakerVoltageLevel& voltageLevel);

    ~BusNamingStrategy() noexcept = default;

    std::string getName();

private:
    NodeBreakerVoltageLevel& m_voltageLevel;

    std::atomic_ulong m_counter;
};

}  // node_breaker_voltage_level

}  // iidm

}  // powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP
