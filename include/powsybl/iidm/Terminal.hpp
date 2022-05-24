/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINAL_HPP
#define POWSYBL_IIDM_TERMINAL_HPP

#include <set>
#include <vector>

#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/TerminalSet.hpp>
#include <powsybl/iidm/TerminalTopologyTraverser.hpp>
#include <powsybl/iidm/TerminalViews.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class Network;
class VoltageLevel;

class Terminal : public MultiVariantObject {
public:
    using BusBreakerView = terminal::BusBreakerView;

    using BusView = terminal::BusView;

    using NodeBreakerView = terminal::NodeBreakerView;

    using TopologyTraverser = terminal::TopologyTraverser;

public:
    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    static bool isInstanceOf(const Terminal& terminal);

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    static const T& map(const Terminal& terminal);

public:
    ~Terminal() noexcept override = default;

    bool connect();

    bool disconnect();

    virtual double getAngle() const = 0;

    virtual const BusBreakerView& getBusBreakerView() const = 0;

    virtual BusBreakerView& getBusBreakerView() = 0;

    virtual const BusView& getBusView() const = 0;

    virtual BusView& getBusView() = 0;

    stdcxx::CReference<Connectable> getConnectable() const;

    stdcxx::Reference<Connectable> getConnectable();

    double getI() const;

    virtual const NodeBreakerView& getNodeBreakerView() const = 0;

    virtual NodeBreakerView& getNodeBreakerView() = 0;

    double getP() const;

    double getQ() const;

    virtual double getV() const = 0;

    const VoltageLevel& getVoltageLevel() const;

    VoltageLevel& getVoltageLevel();

    virtual bool isConnected() const = 0;

    Terminal& setConnectable(const stdcxx::Reference<Connectable>& connectable);

    Terminal& setP(double p);

    Terminal& setQ(double q);

    virtual bool traverse(TopologyTraverser& traverser) = 0;

    virtual bool traverse(TopologyTraverser& traverser, TerminalSet& traversedTerminals) = 0;

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

    friend class Connectable;

protected:
    explicit Terminal(VoltageLevel& voltageLevel);

    const Network& getNetwork() const;

    Network& getNetwork();

private:
    VoltageLevel& m_voltageLevel;

    stdcxx::Reference<Connectable> m_connectable;

    std::vector<double> m_p;

    std::vector<double> m_q;
};

std::unique_ptr<Terminal> createBusTerminal(VoltageLevel& voltageLevel, const std::string& connectableBusId, bool connected);

std::unique_ptr<Terminal> createNodeTerminal(VoltageLevel& voltageLevel, unsigned long node);

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Terminal.hxx>

#endif  // POWSYBL_IIDM_TERMINAL_HPP
