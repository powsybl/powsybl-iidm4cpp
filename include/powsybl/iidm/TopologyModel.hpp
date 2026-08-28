/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TOPOLOGYMODEL_HPP
#define POWSYBL_IIDM_TOPOLOGYMODEL_HPP


#include <powsybl/iidm/Connectable.hpp>
#include <powsybl/iidm/PropertiesHolder.hpp>
#include <powsybl/iidm/VoltageLevelViews.hpp>

#include <powsybl/stdcxx/Predicate.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Switch;
class Terminal;
class VoltageLevel;

class TopologyModel : public MultiVariantObject, public PropertiesHolder {
public:
    using BusBreakerView = voltage_level::BusBreakerView;
    using BusView = voltage_level::BusView;
    using NodeBreakerView = voltage_level::NodeBreakerView;

public:
    const Network& getNetwork() const;
    Network& getNetwork();
    const Network& getParentNetwork() const;
    Network& getParentNetwork();
    std::string getSubnetworkId() const;

    VoltageLevel& getVoltageLevel();
    const VoltageLevel& getVoltageLevel() const;

    virtual void invalidateCache(bool exceptBusBreakerView = false) = 0;

    virtual void attach(Terminal& terminal, bool test) = 0;

    virtual bool connect(Terminal& terminal) = 0;
    virtual bool connect(Terminal& terminal, const stdcxx::Predicate<Switch>& isTypeSwitchToOperate) = 0;

    virtual void detach(Terminal& terminal) = 0;

    virtual bool disconnect(Terminal& terminal) = 0;
    virtual bool disconnect(Terminal& terminal, const stdcxx::Predicate<Switch>& isSwitchOpenable) = 0;


    virtual const NodeBreakerView& getNodeBreakerView() const = 0;
    virtual NodeBreakerView& getNodeBreakerView() = 0;
    virtual const BusBreakerView& getBusBreakerView() const = 0;
    virtual BusBreakerView& getBusBreakerView() = 0;
    virtual const BusView& getBusView() const = 0;
    virtual BusView& getBusView() = 0;

    virtual const TopologyKind& getTopologyKind() const = 0;

    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    unsigned long getConnectableCount() const;
    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::const_range<T> getConnectables() const;
    template <typename T = Connectable, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    stdcxx::range<T> getConnectables();

    virtual unsigned long getSwitchCount() const = 0;
    virtual stdcxx::const_range<Switch> getSwitches() const = 0;
    virtual stdcxx::range<Switch> getSwitches() = 0;


    //Print / Export Topology ?
    // public abstract void printTopology();
    // public abstract void printTopology(PrintStream out, ShortIdDictionary dict);
    // public abstract void exportTopology(Path file) throws IOException;
    // public abstract void exportTopology(Writer writer);
    // public abstract void exportTopology(Writer writer, Random random);

protected:
    TopologyModel(VoltageLevel& voltageLevel);
    friend class VoltageLevel;

    virtual void removeTopology() = 0;

    static void addNextTerminals(Terminal& otherTerminal, TerminalSet& nextTerminals);

    virtual stdcxx::const_range<Terminal> getTerminals() const = 0;
    virtual stdcxx::range<Terminal> getTerminals() = 0;

private:
    VoltageLevel& m_voltageLevel;

};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TopologyModel.hxx>

#endif  // POWSYBL_IIDM_TOPOLOGYMODEL_HPP
