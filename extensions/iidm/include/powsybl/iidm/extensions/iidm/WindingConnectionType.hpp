/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_WINDINGCONNECTIONTYPE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_WINDINGCONNECTIONTYPE_HPP

#include <cstdint>
#include <ostream>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

enum class WindingConnectionType : std::uint8_t {
    Y,
    Y_GROUNDED,
    DELTA
}; 

std::ostream& operator<<(std::ostream& stream, const WindingConnectionType& connectionType);

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_WINDINGCONNECTIONTYPE_HPP
