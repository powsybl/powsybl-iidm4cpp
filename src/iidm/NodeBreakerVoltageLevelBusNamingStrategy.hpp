/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP
#define POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

class NodeBreakerVoltageLevel;

namespace node_breaker_voltage_level {

class BusNamingStrategy {
public:
    explicit BusNamingStrategy(NodeBreakerVoltageLevel& voltageLevel);

    ~BusNamingStrategy() noexcept = default;

    std::string getId(const std::vector<unsigned long>& nodes);

    std::string getName(const std::vector<unsigned long>& nodes);

private:
    NodeBreakerVoltageLevel& m_voltageLevel;
};

}  // namespace node_breaker_voltage_level

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERVOLTAGELEVELBUSNAMINGSTRATEGY_HPP
