/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Connectable.hpp>

namespace powsybl {

namespace iidm {

Connectable::Connectable(const std::string& id, const std::string& name, const ConnectableType& connectableType) :
    Identifiable(id, name),
    m_connectableType(connectableType) {
}

void Connectable::allocateStateArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
}

void Connectable::deleteStateArrayElement(unsigned long index) {
}

void Connectable::extendStateArraySize(unsigned long initStateArraySize, unsigned long number, unsigned long sourceIndex) {
}

const ConnectableType& Connectable::getConnectableType() const {
    return m_connectableType;
}

void Connectable::reduceStateArraySize(unsigned long number) {
}

}

}
