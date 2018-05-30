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
#include <powsybl/iidm/Stateful.hpp>
#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

class Network;

class Connectable : public Identifiable, public Stateful {
public:
    virtual ~Connectable() = default;

    Terminal& addTerminal(std::unique_ptr<Terminal>&& terminal);

    const ConnectableType& getConnectableType() const;

    std::vector<std::reference_wrapper<Terminal> > getTerminals() const;

    void remove();

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

protected:
    Connectable(const std::string& id, const std::string& name, const ConnectableType& connectableType);

    const Network& getNetwork() const;

    Network& getNetwork();

    const Terminal& getTerminal(unsigned long index) const;

    Terminal& getTerminal(unsigned long index);

private:
    ConnectableType m_connectableType;

    std::vector<std::unique_ptr<Terminal> > m_terminals;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTABLE_HPP
