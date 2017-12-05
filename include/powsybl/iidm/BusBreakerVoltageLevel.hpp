/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
#define POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP

#include <powsybl/iidm/VoltageLevel.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel : public VoltageLevel {
public:
    BusBreakerVoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                           double nominalVoltage, double lowVoltageLimit, double highVoltagelimit);

    virtual ~BusBreakerVoltageLevel() = default;

public:
    TopologyKind getTopologyKind() const override;
};

}

}

#endif  // POWSYBL_IIDM_BUSBREAKERVOLTAGELEVEL_HPP
