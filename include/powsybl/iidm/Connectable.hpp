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
    std::vector<std::reference_wrapper<Terminal> > getTerminals(const stdcxx::optional<TerminalNumber>& terminalNumber) const;

    virtual void remove();

    /**
     * Try to connect all sides of the connectable by operating only non-fictional breakers,
     * If you wish to operate other kind of switches, use connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate)
     */
    virtual bool connect();

    /**
     * Try to connect all sides of the connectable by operating switches that respect the given predicate.
     */
    virtual bool connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate);

    /**
     * Try to connect the connectable on the given side by operating switches that respect the given predicate.
     * 
     * @param isTypeSwitchToOperate predicate to filter allowed switches to operate on
     * @param side optional side of the connectable to connect. Use an empty optional to connect all sides
     * @return true if the connection succeeded, false otherwise (if one of the terminal of this connectable could not be connected, or it was already connected)
     */
    virtual bool connect(const stdcxx::Predicate<Switch>& isTypeSwitchToOperate, const stdcxx::optional<ThreeSides>& side);

    /**
     * Try to disconnect all sides of the connectable by operating only closed non-fictional breakers,
     * If you wish to operate other kind of switches, use disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable)
     */
    virtual bool disconnect();
    /**
     * Try to disconnect the connectable on the given side by operating switches that respect the given predicate.
     */
    virtual bool disconnect(const stdcxx::Predicate<Switch>& isSwitchOpenable);
    /**
     * Try to disconnect the connectable on the given side by operating switches that respect the given predicate.
     * 
     * @param isSwitchOpenable predicate to filter allowed switches to operate on
     * @param side optional side of the connectable to disconnect. Use an empty optional to disconnect all sides
     * @return true if the diconnection succeeded, false otherwise (if one of the terminal of this connectable could not be disconnected, or it was already disconnected)
     */
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

    unsigned long getTerminalsCount() const;

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
