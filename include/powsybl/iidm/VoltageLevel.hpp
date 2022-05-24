/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VOLTAGELEVEL_HPP
#define POWSYBL_IIDM_VOLTAGELEVEL_HPP

#include <set>

#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/Container.hpp>
#include <powsybl/iidm/NetworkRef.hpp>
#include <powsybl/iidm/TerminalSet.hpp>
#include <powsybl/iidm/TopologyKind.hpp>
#include <powsybl/iidm/VoltageLevelTopologyTraverser.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>
#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Battery;
class BatteryAdder;
class DanglingLine;
class DanglingLineAdder;
class Generator;
class GeneratorAdder;
class LccConverterStation;
class LccConverterStationAdder;
class Load;
class LoadAdder;
class Network;
class NetworkIndex;
class ShuntCompensator;
class ShuntCompensatorAdder;
class StaticVarCompensator;
class StaticVarCompensatorAdder;
class Switch;
class Substation;
class Terminal;
class TopologyVisitor;
class VscConverterStation;
class VscConverterStationAdder;

class VoltageLevel : public Container {
public:
    using BusBreakerView = voltage_level::BusBreakerView;

    using BusView = voltage_level::BusView;

    using NodeBreakerView = voltage_level::NodeBreakerView;

    using TopologyTraverser = voltage_level::TopologyTraverser;

public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const IdentifiableType& getType() const override;

public:
    ~VoltageLevel() noexcept override = default;

    virtual void attach(Terminal& terminal, bool test) = 0;

    virtual bool connect(Terminal& terminal) = 0;

    virtual void detach(Terminal& terminal) = 0;

    virtual bool disconnect(Terminal& terminal) = 0;

    unsigned long getBatteryCount() const;

    stdcxx::const_range<Battery> getBatteries() const;

    stdcxx::range<Battery> getBatteries();

    virtual const BusBreakerView& getBusBreakerView() const = 0;

    virtual BusBreakerView& getBusBreakerView() = 0;

    virtual const BusView& getBusView() const = 0;

    virtual BusView& getBusView() = 0;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::CReference<T> getConnectable(const std::string& id) const;

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::Reference<T> getConnectable(const std::string& id);

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::const_range<T> getConnectables() const;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::range<T> getConnectables();

    unsigned long getDanglingLineCount() const;

    stdcxx::const_range<DanglingLine> getDanglingLines() const;

    stdcxx::range<DanglingLine> getDanglingLines();

    unsigned long getGeneratorCount() const;

    stdcxx::const_range<Generator> getGenerators() const;

    stdcxx::range<Generator> getGenerators();

    double getHighVoltageLimit() const;

    unsigned long getLccConverterStationCount() const;

    stdcxx::const_range<LccConverterStation> getLccConverterStations() const;

    stdcxx::range<LccConverterStation> getLccConverterStations();

    unsigned long getLineCount() const;

    stdcxx::const_range<Line> getLines() const;

    stdcxx::range<Line> getLines();

    unsigned long getLoadCount() const;

    stdcxx::const_range<Load> getLoads() const;

    stdcxx::range<Load> getLoads();

    double getLowVoltageLimit() const;

    virtual const NodeBreakerView& getNodeBreakerView() const = 0;

    virtual NodeBreakerView& getNodeBreakerView() = 0;

    double getNominalV() const;

    unsigned long getShuntCompensatorCount() const;

    stdcxx::const_range<ShuntCompensator> getShuntCompensators() const;

    stdcxx::range<ShuntCompensator> getShuntCompensators();

    unsigned long getStaticVarCompensatorCount() const;

    stdcxx::const_range<StaticVarCompensator> getStaticVarCompensators() const;

    stdcxx::range<StaticVarCompensator> getStaticVarCompensators();

    stdcxx::CReference<Substation> getSubstation() const;

    stdcxx::Reference<Substation> getSubstation();

    virtual unsigned long getSwitchCount() const = 0;

    virtual stdcxx::const_range<Switch> getSwitches() const = 0;

    virtual stdcxx::range<Switch> getSwitches() = 0;

    unsigned long getThreeWindingsTransformerCount() const;

    stdcxx::const_range<ThreeWindingsTransformer> getThreeWindingsTransformers() const;

    stdcxx::range<ThreeWindingsTransformer> getThreeWindingsTransformers();

    virtual const TopologyKind& getTopologyKind() const = 0;

    unsigned long getTwoWindingsTransformerCount() const;

    stdcxx::const_range<TwoWindingsTransformer> getTwoWindingsTransformers() const;

    stdcxx::range<TwoWindingsTransformer> getTwoWindingsTransformers();

    unsigned long getVscConverterStationCount() const;

    stdcxx::const_range<VscConverterStation> getVscConverterStations() const;

    stdcxx::range<VscConverterStation> getVscConverterStations();

    virtual void invalidateCache() = 0;

    BatteryAdder newBattery();

    DanglingLineAdder newDanglingLine();

    GeneratorAdder newGenerator();

    LccConverterStationAdder newLccConverterStation();

    LoadAdder newLoad();

    ShuntCompensatorAdder newShuntCompensator();

    StaticVarCompensatorAdder newStaticVarCompensator();

    VscConverterStationAdder newVscConverterStation();

    void remove();

    VoltageLevel& setHighVoltageLimit(double highVoltageLimit);

    VoltageLevel& setLowVoltageLimit(double lowVoltageLimit);

    VoltageLevel& setNominalV(double nominalV);

    void visitEquipments(TopologyVisitor& visitor) const;

protected:
    static void addNextTerminals(Terminal& otherTerminal, TerminalSet& nextTerminals);

protected:
    VoltageLevel(const std::string& id, const std::string& name, bool fictitious, const stdcxx::Reference<Substation>& substation,
                 Network& network, double nominalV, double lowVoltageLimit, double highVoltageLimit);

    virtual stdcxx::const_range<Terminal> getTerminals() const = 0;

    virtual stdcxx::range<Terminal> getTerminals() = 0;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    virtual void removeTopology() = 0;

    void setNetworkRef(Network& network);

    friend class NetworkIndex;

private:
    NetworkRef m_network;

    stdcxx::Reference<Substation> m_substation;

    double m_highVoltageLimit;

    double m_lowVoltageLimit;

    double m_nominalV;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/VoltageLevel.hxx>

#endif  // POWSYBL_IIDM_VOLTAGELEVEL_HPP
