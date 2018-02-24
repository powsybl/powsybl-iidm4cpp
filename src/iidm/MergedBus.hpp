/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MERGEDBUS_HPP
#define POWSYBL_IIDM_MERGEDBUS_HPP

#include <functional>
#include <set>
#include <string>

#include "CalculatedBus.hpp"
#include "ConfiguredBus.hpp"

namespace powsybl {

namespace iidm {

class MergedBus : public CalculatedBus {
public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    double getP() const override;

    double getQ() const override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public: // CalculatedBus
    void invalidate() override;

public:
    MergedBus(const std::string& id, std::set<std::reference_wrapper<ConfiguredBus> >&& buses);

    virtual ~MergedBus() = default;

private:
    void checkValidity() const;

private:
    std::set<std::reference_wrapper<ConfiguredBus> > m_buses;

    bool m_valid;

};

}

}

#endif  // POWSYBL_IIDM_MERGEDBUS_HPP
