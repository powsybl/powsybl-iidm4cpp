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
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;
class BusTerminal;
class Terminal;

class ConfiguredBus : public Bus, public MultiVariantObject {
public: // Bus
    double getAngle() const override;

    unsigned long getConnectedTerminalCount() const override;

    bus::Terminals getConnectedTerminals() const override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    ConfiguredBus(const std::string& id, const std::string& name, BusBreakerVoltageLevel& voltageLevel);

    ~ConfiguredBus() noexcept override = default;

    void addTerminal(BusTerminal& terminal);

    unsigned long getTerminalCount() const;

    std::vector<std::reference_wrapper<BusTerminal> > getTerminals() const;

    void removeTerminal(BusTerminal& terminal);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private:
    stdcxx::Reference<BusBreakerVoltageLevel> m_voltageLevel;

    stdcxx::Reference<VariantManagerHolder> m_network;

    std::vector<std::list<std::reference_wrapper<BusTerminal> > > m_terminals;

    std::vector<double> m_v;

    std::vector<double> m_angle;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONFIGUREDBUS_HPP
