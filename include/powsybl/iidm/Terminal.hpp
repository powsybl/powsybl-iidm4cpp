/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINAL_HPP
#define POWSYBL_IIDM_TERMINAL_HPP

#include <vector>

#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/TerminalViews.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Connectable;
class Network;
class VoltageLevel;

class Terminal : public MultiVariantObject {
public:
    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    static bool isInstanceOf(const std::reference_wrapper<Terminal>& terminal);

    template <typename T, typename = typename std::enable_if<std::is_base_of<Connectable, T>::value>::type>
    static T& map(const std::reference_wrapper<Terminal>& terminal);

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

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

    friend class Connectable;

protected:
    explicit Terminal(VariantManagerHolder& network);

    const VariantManagerHolder& getNetwork() const;

private:
    stdcxx::Reference<VariantManagerHolder> m_network;

    stdcxx::Reference<Connectable> m_connectable;

    stdcxx::Reference<VoltageLevel> m_voltageLevel;

    std::vector<double> m_p;

    std::vector<double> m_q;
};

std::unique_ptr<Terminal> createBusTerminal(Network& network, const std::string& connectableBusId, bool connected);

std::unique_ptr<Terminal> createNodeTerminal(Network& network, unsigned long node);

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/Terminal.hxx>

#endif  // POWSYBL_IIDM_TERMINAL_HPP
