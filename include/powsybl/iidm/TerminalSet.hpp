/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINALSET_HPP
#define POWSYBL_IIDM_TERMINALSET_HPP

#include <functional>
#include <set>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

using TerminalSet = std::set<std::reference_wrapper<Terminal>, stdcxx::less<Terminal>>;

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM4CPP_TERMINALSET_HPP
