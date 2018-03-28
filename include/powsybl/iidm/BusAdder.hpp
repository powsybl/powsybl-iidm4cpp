/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_BUSADDER_HPP
#define POWSYBL_IIDM_BUSADDER_HPP

#include <powsybl/iidm/IdentifiableAdder.hpp>

namespace powsybl {

namespace iidm {

class Bus;
class BusBreakerView;
class VoltageLevel;

class BusAdder : public IdentifiableAdder<BusAdder> {
public:
    Bus& add();

protected: // IdentifiableAdder
    Network& getNetwork() override;

private: // IdentifiableAdder
    const std::string& getTypeDescription() const override;

public:
    explicit BusAdder(VoltageLevel& voltageLevel);

    virtual ~BusAdder() = default;

private:
    VoltageLevel& m_voltageLevel;
};

}

}

#endif  // POWSYBL_IIDM_BUSADDER_HPP
