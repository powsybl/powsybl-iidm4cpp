/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONFIGUREDBUS_HPP
#define POWSYBL_IIDM_CONFIGUREDBUS_HPP

#include <list>
#include <vector>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/BusAdder.hpp>
#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/Stateful.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;
class BusTerminal;

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
    stdcxx::Reference<BusBreakerVoltageLevel> m_voltageLevel;

    stdcxx::Reference<MultiStateObject> m_network;

    std::vector<std::list<std::reference_wrapper<BusTerminal> > > m_terminals;

    std::vector<double> m_v;

    std::vector<double> m_angle;
};

}

}

#endif  // POWSYBL_IIDM_CONFIGUREDBUS_HPP
