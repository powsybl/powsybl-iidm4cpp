/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HPP
#define POWSYBL_IIDM_VOLTAGELEVEL_HPP

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/bits/Terminal.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class BatteryAdder;
class BusBreakerView;
class BusView;
class DanglingLine;
class DanglingLineAdder;
class Generator;
class GeneratorAdder;
class LccConverterStation;
class LccConverterStationAdder;
class Load;
class LoadAdder;
class Network;
class NodeBreakerView;
class ShuntCompensator;
class ShuntCompensatorAdder;
class StaticVarCompensator;
class StaticVarCompensatorAdder;
class Switch;
class Substation;
class Terminal;
class VscConverterStation;
class VscConverterStationAdder;

class VoltageLevel : public Container, public MultiVariantObject {
public:
    template <typename T>
    using const_range = typename terminal::range_traits<T>::const_range;

    template <typename T>
    using range = typename terminal::range_traits<T>::range;

public:
    ~VoltageLevel() noexcept override = default;

    virtual void attach(Terminal& terminal, bool test) = 0;

    virtual void clean() = 0;

    virtual bool connect(Terminal& terminal) = 0;

    virtual void detach(Terminal& terminal) = 0;

    virtual bool disconnect(Terminal& terminal) = 0;

    unsigned long getBatteryCount() const;

    const_range<Battery> getBatteries() const;

    range<Battery> getBatteries();

    virtual const BusBreakerView& getBusBreakerView() const = 0;

    virtual BusBreakerView& getBusBreakerView() = 0;

    virtual const BusView& getBusView() const = 0;

    virtual BusView& getBusView() = 0;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::CReference<T> getConnectable(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::Reference<T> getConnectable(const std::string& id);

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;

    unsigned long getConnectableCount() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    const_range<T> getConnectables() const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    range<T> getConnectables();

    unsigned long getDanglingLineCount() const;

    const_range<DanglingLine> getDanglingLines() const;

    range<DanglingLine> getDanglingLines();

    unsigned long getGeneratorCount() const;

    const_range<Generator> getGenerators() const;

    range<Generator> getGenerators();

    double getHighVoltageLimit() const;

    unsigned long getLccConverterStationCount() const;

    const_range<LccConverterStation> getLccConverterStations() const;

    range<LccConverterStation> getLccConverterStations();

    unsigned long getLoadCount() const;

    const_range<Load> getLoads() const;

    range<Load> getLoads();

    double getLowVoltageLimit() const;

    const Network& getNetwork() const;

    Network& getNetwork();

    virtual const NodeBreakerView& getNodeBreakerView() const = 0;

    virtual NodeBreakerView& getNodeBreakerView() = 0;

    double getNominalVoltage() const;

    unsigned long getShuntCompensatorCount() const;

    const_range<ShuntCompensator> getShuntCompensators() const;

    range<ShuntCompensator> getShuntCompensators();

    unsigned long getStaticVarCompensatorCount() const;

    const_range<StaticVarCompensator> getStaticVarCompensators() const;

    range<StaticVarCompensator> getStaticVarCompensators();

    const Substation& getSubstation() const;

    Substation& getSubstation();

    // TODO(mathbagu): virtual unsigned long getSwitchCount() const;

    // TODO(mathbagu): virtual const_range<Switch> getSwitches() const;

    // TODO(mathbagu): virtual range<Switch> getSwitches();

    virtual const TopologyKind& getTopologyKind() const = 0;

    unsigned long getVscConverterStationCount() const;

    const_range<VscConverterStation> getVscConverterStations() const;

    range<VscConverterStation> getVscConverterStations();

    virtual void invalidateCache() = 0;

    BatteryAdder newBattery();

    DanglingLineAdder newDanglingLine();

    GeneratorAdder newGenerator();

    LccConverterStationAdder newLccConverterStation();

    LoadAdder newLoad();

    ShuntCompensatorAdder newShuntCompensator();

    StaticVarCompensatorAdder newStaticVarCompensator();

    VscConverterStationAdder newVscConverterStation();

    VoltageLevel& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevel& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevel& setNominalVoltage(double nominalVoltage);

protected:
    VoltageLevel(const std::string& id, const std::string& name, Substation& substation,
                 double nominalVoltage, double lowVoltageLimit, double highVoltageLimit);

    virtual std::vector<std::reference_wrapper<Terminal>> getTerminals() const = 0;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<Substation> m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalVoltage;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VoltageLevel.hxx>

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HPP
