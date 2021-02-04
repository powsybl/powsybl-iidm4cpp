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
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class BusBreakerVoltageLevel;
class BusTerminal;
class Network;
class Terminal;

class ConfiguredBus : public Bus {
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

    const VoltageLevel& getVoltageLevel() const override;

    VoltageLevel& getVoltageLevel() override;

    Bus& setAngle(double angle) override;

    void setConnectedComponentNumber(const stdcxx::optional<unsigned long>& connectedComponentNumber) override;

    void setSynchronousComponentNumber(const stdcxx::optional<unsigned long>& synchronousComponentNumber) override;

    Bus& setV(double v) override;

    void visitConnectedOrConnectableEquipments(TopologyVisitor& visitor) override;

public:
    ConfiguredBus(const std::string& id, const std::string& name, bool fictitious, BusBreakerVoltageLevel& voltageLevel);

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

private:
    stdcxx::Reference<BusBreakerVoltageLevel> m_voltageLevel;

    std::vector<std::list<std::reference_wrapper<BusTerminal> > > m_terminals;

    std::vector<double> m_v;

    std::vector<double> m_angle;

    std::vector<stdcxx::optional<unsigned long>> m_connectedComponentNumber;

    std::vector<stdcxx::optional<unsigned long>> m_synchronousComponentNumber;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONFIGUREDBUS_HPP
