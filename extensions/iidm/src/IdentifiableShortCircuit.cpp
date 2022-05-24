/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/IdentifiableShortCircuit.hpp>

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

IdentifiableShortCircuit::IdentifiableShortCircuit(Identifiable& identifiable, double ipMin, double ipMax) :
    Extension(identifiable),
    m_ipMin(ipMin),
    m_ipMax(ipMax) {
}

void IdentifiableShortCircuit::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Identifiable>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Identifiable>()));
    }
}

double IdentifiableShortCircuit::getIpMax() const {
    return m_ipMax;
}

double IdentifiableShortCircuit::getIpMin() const {
    return m_ipMin;
}

const std::string& IdentifiableShortCircuit::getName() const {
    static std::string s_name = "identifiableShortCircuit";
    return s_name;
}

const std::type_index& IdentifiableShortCircuit::getType() const {
    static std::type_index s_type = typeid(IdentifiableShortCircuit);
    return s_type;
}

IdentifiableShortCircuit& IdentifiableShortCircuit::setIpMax(double ipMax) {
    if (std::isnan(ipMax)) {
        throw PowsyblException("Undefined ipMax");
    }
    m_ipMax = ipMax;
    return *this;
}

IdentifiableShortCircuit& IdentifiableShortCircuit::setIpMin(double ipMin) {
    m_ipMin = ipMin;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
