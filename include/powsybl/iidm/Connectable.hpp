/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONNECTABLE_HPP
#define POWSYBL_IIDM_CONNECTABLE_HPP

#include <powsybl/iidm/ConnectableType.hpp>
#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Stateful.hpp>

namespace powsybl {

namespace iidm {

class Connectable : public Identifiable, public Stateful {
public:
    virtual ~Connectable() = default;

    const ConnectableType& getConnectableType() const;

protected: // Stateful
    void allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteStateArrayElement(unsigned long index) override;

    void extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceStateArraySize(unsigned long number) override;

protected:
    Connectable(const std::string& id, const std::string& name, const ConnectableType& connectableType);

private:
    ConnectableType m_connectableType;
};

}

}

#endif  // POWSYBL_IIDM_CONNECTABLE_HPP
