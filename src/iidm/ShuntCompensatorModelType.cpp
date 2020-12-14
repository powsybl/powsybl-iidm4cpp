/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ShuntCompensatorModelType.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<ShuntCompensatorModelType>() {
    static std::initializer_list<std::string> s_names = {
        "LINEAR",
        "NON_LINEAR"
    };
    return s_names;
}

}  // namespace Enum

}  // namespace iidm

}  // namespace powsybl
