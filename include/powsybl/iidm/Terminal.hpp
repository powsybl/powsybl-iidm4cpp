/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINAL_HPP
#define POWSYBL_IIDM_TERMINAL_HPP

#include <vector>

#include <powsybl/iidm/MultiStateObject.hpp>
#include <powsybl/iidm/Stateful.hpp>
#include <powsybl/iidm/TerminalViews.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class Network;
class VoltageLevel;

class Terminal : public Stateful {
public:
    ~Terminal() noexcept override = default;

    bool connect();

    bool disconnect();

    virtual double getAngle() const = 0;

    virtual const terminal::BusBreakerView& getBusBreakerView() const = 0;

    virtual terminal::BusBreakerView& getBusBreakerView() = 0;

    virtual const terminal::BusView& getBusView() const = 0;

    virtual terminal::BusView& getBusView() = 0;

    const stdcxx::Reference<Connectable>& getConnectable() const;

    double getI() const;

    virtual const terminal::NodeBreakerView& getNodeBreakerView() const = 0;

    virtual terminal::NodeBreakerView& getNodeBreakerView() = 0;

    double getP() const;

    double getQ() const;

    virtual double getV() const = 0;

    const VoltageLevel& getVoltageLevel() const;

    VoltageLevel& getVoltageLevel();

    virtual bool isConnected() const = 0;

    Terminal& setConnectable(const stdcxx::Reference<Connectable>& connectable);

    Terminal& setP(double p);

    Terminal& setQ(double q);

    Terminal& setVoltageLevel(const stdcxx::Reference<VoltageLevel>& voltageLevel);

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

    friend class Connectable;

protected:
    explicit Terminal(MultiStateObject& network);

    const MultiStateObject& getNetwork() const;

private:
    stdcxx::Reference<MultiStateObject> m_network;

    stdcxx::Reference<Connectable> m_connectable;

    stdcxx::Reference<VoltageLevel> m_voltageLevel;

    std::vector<double> m_p;

    std::vector<double> m_q;
};

std::unique_ptr<Terminal> createBusTerminal(Network& network, const std::string& connectableBusId, bool connected);

std::unique_ptr<Terminal> createNodeTerminal(Network& network, unsigned long node);

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINAL_HPP
