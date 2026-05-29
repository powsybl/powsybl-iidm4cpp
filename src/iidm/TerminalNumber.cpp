/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TerminalNumber.hpp>

#include <powsybl/iidm/Enum.hpp>

namespace powsybl {

namespace iidm {

namespace Enum {

template <>
const std::initializer_list<std::string>& getNames<TerminalNumber>() {
    static std::initializer_list<std::string> s_names {
        "UNDEFINED",
        "ONE",
        "TWO"
    };
    return s_names;
}

}  // namespace Enum

std::ostream& operator<<(std::ostream& stream, const TerminalNumber& num) {
    stream << Enum::toString(num);
    return stream;
}

}  // namespace iidm

}  // namespace powsybl
