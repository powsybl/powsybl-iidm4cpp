/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#include "NetworkExtension.hpp"

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace iidm {

NetworkExtension::NetworkExtension(Network& network, bool value) :
    AbstractMultiVariantIdentifiableExtension(network) {
    unsigned long variantArraySize = getVariantManagerHolder().getVariantManager().getVariantArraySize();
    m_value.resize(variantArraySize, value);
}

void NetworkExtension::allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) {
    for (unsigned long index : indexes) {
        m_value[index] = m_value[sourceIndex];
    }
}

void NetworkExtension::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Network>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Network>()));
    }
}

void NetworkExtension::deleteVariantArrayElement(unsigned long /*index*/) {
    // Does nothing
}

void NetworkExtension::extendVariantArraySize(unsigned long /*initVariantArraySize*/, unsigned long number, unsigned long sourceIndex) {
    m_value.resize(m_value.size() + number, m_value[sourceIndex]);
}

const std::string& NetworkExtension::getName() const {
    static std::string s_name = "networkExtension";
    return s_name;
}

const std::type_index& NetworkExtension::getType() const {
    static std::type_index s_type = typeid(NetworkExtension);
    return s_type;
}

bool NetworkExtension::getValue() const {
    return m_value[getVariantIndex()];
}

void NetworkExtension::reduceVariantArraySize(unsigned long number) {
    m_value.resize(m_value.size() - number);
}

NetworkExtension& NetworkExtension::setValue(bool value) {
    m_value[getVariantIndex()] = value;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
