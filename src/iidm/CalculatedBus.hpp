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

namespace powsybl {

namespace iidm {

class NodeBreakerVoltageLevel;
class NodeTerminal;

/**
 * A Bus implementation used in BusBreakerView and BusView of a NodeBreakerVoltageLevel
 */
class CalculatedBus : public Bus {
public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    stdcxx::const_range<Terminal> getConnectedTerminals() const override;

    stdcxx::range<Terminal> getConnectedTerminals() override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    /**
     * Create a new CalculatedBus from a list of NodeTerminal references
     * @param id the ID of this bus
     * @param name the name of this bus
     * @param voltageLevel the voltage level of this bus
     * @param terminals the list of NodeTerminal references which are connected to this bus
     */
    CalculatedBus(const std::string& id, const std::string& name, NodeBreakerVoltageLevel& voltageLevel, const std::vector<unsigned long>& nodes, std::vector<std::reference_wrapper<NodeTerminal> >&& terminals);

    ~CalculatedBus() noexcept override = default;

    /**
     * Invalidate this bus after the voltage level topology changed
     */
    void invalidate();

private:
    static stdcxx::CReference<NodeTerminal> findTerminal(const NodeBreakerVoltageLevel& voltageLevel, const std::vector<unsigned long>& nodes, const std::vector<std::reference_wrapper<NodeTerminal> >& terminals);

private:
    void checkValidity() const;

private:
    stdcxx::Reference<NodeBreakerVoltageLevel> m_voltageLevel;

    bool m_valid;

    std::vector<std::reference_wrapper<NodeTerminal> > m_terminals;

    stdcxx::CReference<NodeTerminal> m_terminalRef;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CALCULATEDBUS_HPP
