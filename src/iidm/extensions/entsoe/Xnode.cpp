/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/Xnode.hpp>

#include <powsybl/iidm/DanglingLine.hpp>

namespace powsybl {

namespace iidm {

const std::string& checkNotEmpty(const std::string& value, const std::string& message);

namespace extensions {

namespace entsoe {

Xnode::Xnode(DanglingLine& dl, const std::string& code) :
    Extension(dl),
    m_code(checkNotEmpty(code, "Xnode code is empty")) {
}

void Xnode::assertExtendable(const stdcxx::Reference<powsybl::iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<DanglingLine>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<DanglingLine>()));
    }
}

const std::string& Xnode::getCode() const {
    return m_code;
}

const std::string& Xnode::getName() const {
    static std::string s_name = "xnode";
    return s_name;
}

const std::type_index& Xnode::getType() const {
    static std::type_index s_type = typeid(Xnode);
    return s_type;
}

Xnode& Xnode::setCode(const std::string& code) {
    m_code = checkNotEmpty(code, "Xnode code is empty");
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
