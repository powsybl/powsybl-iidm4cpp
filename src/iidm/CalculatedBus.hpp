/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CALCULATEDBUS_HPP
#define POWSYBL_IIDM_CALCULATEDBUS_HPP

#include <functional>
#include <vector>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

#include "NodeBreakerVoltageLevel.hpp"

namespace powsybl {

namespace iidm {

/**
 * A Bus implementation used in BusBreakerView and BusView of a NodeBreakerVoltageLevel
 */
class CalculatedBus : public Bus {
public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    /**
     * Create a new CalculatedBus from a list of NodeTerminal references
     * @param id the ID of this bus
     * @param voltageLevel the voltage level of this bus
     * @param terminals the list of NodeTerminal references which are connected to this bus
     */
    CalculatedBus(const std::string& id, NodeBreakerVoltageLevel& voltageLevel, std::vector<std::reference_wrapper<NodeTerminal> >&& terminals);

    virtual ~CalculatedBus() = default;

    /**
     * Invalidate this bus after the voltage level topology changed
     */
    void invalidate();

private:
    void checkValidity() const;

private:
    stdcxx::Reference<NodeBreakerVoltageLevel> m_voltageLevel;

    std::vector<std::reference_wrapper<NodeTerminal> > m_terminals;

    bool m_valid;
};

}

}

#endif  // POWSYBL_IIDM_CALCULATEDBUS_HPP
