/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_NODEBREAKERTOPOLOGYBUSNAMINGSTRATEGY_HPP
#define POWSYBL_IIDM_NODEBREAKERTOPOLOGYBUSNAMINGSTRATEGY_HPP

#include <string>
#include <vector>

namespace powsybl {

namespace iidm {

class VoltageLevel;

namespace node_breaker_topology_model {

class BusNamingStrategy {
public:
    explicit BusNamingStrategy(VoltageLevel& voltageLevel);

    ~BusNamingStrategy() noexcept = default;

    std::string getId(const std::vector<unsigned long>& nodes);

    std::string getName(const std::vector<unsigned long>& nodes);

private:
    VoltageLevel& m_voltageLevel;
};

}  // namespace node_breaker_topology_model

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_NODEBREAKERTOPOLOGYBUSNAMINGSTRATEGY_HPP
