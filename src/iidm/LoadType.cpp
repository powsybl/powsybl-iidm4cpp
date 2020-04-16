/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadType.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<LoadType>() {
    static std::initializer_list<std::string> s_typeNames {
        "UNDEFINED",
        "AUXILIARY",
        "FICTITIOUS"
    };
    return s_typeNames;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
