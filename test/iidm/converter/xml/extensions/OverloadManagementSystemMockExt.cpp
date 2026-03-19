/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "OverloadManagementSystemMockExt.hpp"

#include <powsybl/iidm/OverloadManagementSystem.hpp>
#include <powsybl/stdcxx/demangle.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

OverloadManagementSystemMockExt::OverloadManagementSystemMockExt(OverloadManagementSystem& oms, const std::string& foo) :
    Extension(oms),
    m_foo(foo) {
}

void OverloadManagementSystemMockExt::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<OverloadManagementSystem>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<OverloadManagementSystem>()));
    }
}

const std::string& OverloadManagementSystemMockExt::getName() const {
    static std::string s_name = "omsMock";
    return s_name;
}

const std::type_index& OverloadManagementSystemMockExt::getType() const {
    static std::type_index s_type = typeid(OverloadManagementSystemMockExt);
    return s_type;
}

const std::string& OverloadManagementSystemMockExt::getFoo() const {
    return m_foo;
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
