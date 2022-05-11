/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTABLE_HPP
#define POWSYBL_IIDM_CONNECTABLE_HPP

#include <memory>
#include <vector>

#include <powsybl/iidm/ConnectableType.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

class Network;

namespace bus_terminal {

class BusBreakerViewImpl;

}  // namespace bus_terminal

namespace node_terminal {

class BusBreakerViewImpl;
class NodeBreakerViewImpl;

}  // namespace node_terminal

class Connectable : public Identifiable {
public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

public:
    ~Connectable() noexcept override = default;

    Terminal& addTerminal(std::unique_ptr<Terminal>&& terminal);

    const ConnectableType& getType() const;

    std::vector<std::reference_wrapper<Terminal> > getTerminals() const;

    virtual void remove();

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    Connectable(const std::string& id, const std::string& name, bool fictitious, const ConnectableType& connectableType);

    const Terminal& getTerminal(unsigned long index) const;

    Terminal& getTerminal(unsigned long index);

    void move(Terminal& oldTerminal, const std::string& oldConnectionInfo, const std::string& busId, bool connected);

    void move(Terminal& oldTerminal, const std::string& oldConnectionInfo, unsigned long node, const std::string& voltageLevelId);

private:
    void attachTerminal(Terminal& oldTerminal, const std::string& oldConnectionInfo, VoltageLevel& voltageLevel, std::unique_ptr<Terminal>&& terminal);

    friend class bus_terminal::BusBreakerViewImpl;

    friend class node_terminal::BusBreakerViewImpl;
    friend class node_terminal::NodeBreakerViewImpl;

private:
    ConnectableType m_connectableType;

    std::vector<std::unique_ptr<Terminal> > m_terminals;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTABLE_HPP
