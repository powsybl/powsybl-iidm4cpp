/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONFIGUREDBUS_HPP
#define POWSYBL_IIDM_CONFIGUREDBUS_HPP

#include <functional>
#include <list>
#include <vector>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusTerminal.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/Stateful.hpp>

#include "BusAdder.hpp"

namespace powsybl {

namespace iidm {

class ConfiguredBus : public Bus, public Stateful {
public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    ConfiguredBus(const std::string& id, BusBreakerVoltageLevel& voltageLevel);

    virtual ~ConfiguredBus() = default;

    void addTerminal(BusTerminal& terminal);

    unsigned long getTerminalCount() const;

    void removeTerminal(BusTerminal& terminal);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

private:
    std::reference_wrapper<BusBreakerVoltageLevel> m_voltageLevel;

    std::reference_wrapper<MultiStateObject> m_network;

    std::vector<std::list<std::reference_wrapper<BusTerminal> > > m_terminals;

    std::vector<double> m_v;

    std::vector<double> m_angle;
};

}

}

#endif  // POWSYBL_IIDM_CONFIGUREDBUS_HPP
