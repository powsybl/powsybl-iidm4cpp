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

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/IdentifiableType.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/stdcxx/Predicate.hpp>

namespace powsybl {

namespace iidm {

class Network;
class Switch;
class TopologyModel;

class Connectable : public virtual Identifiable {
public:  // Identifiable
    const Network& getNetwork() const override;

    Network& getNetwork() override;

    const Network& getParentNetwork() const override;

    Network& getParentNetwork() override;

public:
    ~Connectable() noexcept override = default;

    Terminal& addTerminal(std::unique_ptr<Terminal>&& terminal);

    std::vector<std::reference_wrapper<Terminal> > getTerminals() const;
    std::vector<std::reference_wrapper<Terminal> > getTerminals(const stdcxx::optional<ThreeSides>& side) const;

    virtual void remove();

    virtual bool connect();
    virtual bool connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);
    virtual bool connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<ThreeSides>& side);

    virtual bool disconnect();
    virtual bool disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable);
    virtual bool disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable, const stdcxx::optional<ThreeSides>& side);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    Connectable() = default;

    const Terminal& getTerminal(unsigned long index) const;

    Terminal& getTerminal(unsigned long index);

    /**
     * Connects the new terminal to this connectable and attach it to its topologyModel,
     * and replace old Terminal, by this new one
     */
    void replaceTerminal(Terminal& oldTerminal, std::unique_ptr<Terminal>&& newTerminal, TopologyModel& topologyModelToAttach);
    friend class VoltageLevel;

private:
    std::vector<std::unique_ptr<Terminal> > m_terminals;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTABLE_HPP
