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

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;
class BusTerminal;
class Terminal;

class ConfiguredBus : public Bus, public MultiVariantObject {
public: // Bus
    double getAngle() const override;

    stdcxx::CReference<Component> getConnectedComponent() const override;

    stdcxx::Reference<Component> getConnectedComponent() override;

    unsigned long getConnectedTerminalCount() const override;

    stdcxx::const_range<Terminal> getConnectedTerminals() const override;

    stdcxx::range<Terminal> getConnectedTerminals() override;

    stdcxx::CReference<Component> getSynchronousComponent() const override;

    stdcxx::Reference<Component> getSynchronousComponent() override;

    double getV() const override;

    VoltageLevel& getVoltageLevel() const override;

    Bus& setAngle(double angle) override;

    Bus& setV(double v) override;

public:
    ConfiguredBus(const std::string& id, const std::string& name, BusBreakerVoltageLevel& voltageLevel);

    ~ConfiguredBus() noexcept override = default;

    void addTerminal(BusTerminal& terminal);

    unsigned long getTerminalCount() const;

    stdcxx::const_range<BusTerminal> getTerminals() const;

    stdcxx::range<BusTerminal> getTerminals();

    void removeTerminal(BusTerminal& terminal);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected: // Bus
    void setConnectedComponentNumber(long connectedComponentNumber) override;

    void setSynchronousComponentNumber(long componentNumber) override;

private:
    long getConnectedComponentNumber() const;

    long getSynchronousComponentNumber() const;

    friend class MergedBus;

private:
    stdcxx::Reference<BusBreakerVoltageLevel> m_voltageLevel;

    stdcxx::Reference<VariantManagerHolder> m_network;

    std::vector<std::list<std::reference_wrapper<BusTerminal> > > m_terminals;

    std::vector<double> m_v;

    std::vector<double> m_angle;

    std::vector<long> m_connectedComponentNumber;

    std::vector<long> m_synchronousComponentNumber;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONFIGUREDBUS_HPP
