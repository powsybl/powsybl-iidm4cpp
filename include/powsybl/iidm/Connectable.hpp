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
#include <powsybl/iidm/MultiVariantObject.hpp>
#include <powsybl/iidm/Terminal.hpp>

namespace powsybl {

namespace iidm {

class Network;

class Connectable : public Identifiable, public MultiVariantObject {
public:
    ~Connectable() noexcept override = default;

    Terminal& addTerminal(std::unique_ptr<Terminal>&& terminal);

    const ConnectableType& getType() const;

    std::vector<std::reference_wrapper<Terminal> > getTerminals() const;

    void remove();

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

protected:
    Connectable(const std::string& id, const std::string& name, const ConnectableType& connectableType);

    stdcxx::CReference<Network> getNetwork() const;

    stdcxx::Reference<Network> getNetwork();

    const Terminal& getTerminal(unsigned long index) const;

    Terminal& getTerminal(unsigned long index);

private:
    ConnectableType m_connectableType;

    std::vector<std::unique_ptr<Terminal> > m_terminals;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONNECTABLE_HPP
