/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/network/LoadFooExt.hpp>

#include <powsybl/iidm/Load.hpp>

namespace powsybl {

namespace network {

LoadFooExt::LoadFooExt(iidm::Load& load) :
    Extension(load) {
}

void LoadFooExt::assertExtendable(const stdcxx::Reference<iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<iidm::Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<iidm::Load>()));
    }
}

const std::string& LoadFooExt::getName() const {
    static std::string s_name = "loadFoo";
    return s_name;
}

const std::type_index& LoadFooExt::getType() const {
    static std::type_index s_type = typeid(LoadFooExt);
    return s_type;
}

}  // namespace network

}  // namespace powsybl
