/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "LoadMockExt.hpp"

#include <powsybl/iidm/Extendable.hpp>
#include <powsybl/iidm/Load.hpp>
#include <powsybl/stdcxx/demangle.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

namespace extensions {

LoadMockExt::LoadMockExt(Load& load) :
    Extension(load) {
}

void LoadMockExt::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Load>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Load>()));
    }
}

const std::string& LoadMockExt::getName() const {
    static std::string s_name = "loadMock";
    return s_name;
}

const std::type_index& LoadMockExt::getType() const {
    static std::type_index s_type = typeid(LoadMockExt);
    return s_type;
}

}  // namespace extensions

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
