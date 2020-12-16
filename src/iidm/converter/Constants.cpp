/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/Constants.hpp>

#include <powsybl/stdcxx/optional.hpp>
 
namespace powsybl {

namespace iidm {

namespace converter {

std::string toString(const char* attribute, stdcxx::optional<int> index) {
    std::string str = attribute;
    if (index) {
        str += std::to_string(*index);
    }
    return str;
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
