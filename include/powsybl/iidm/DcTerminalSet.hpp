/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTERMINALSET_HPP
#define POWSYBL_IIDM_DCTERMINALSET_HPP

#include <functional>
#include <set>

#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

class DcTerminal;

using DcTerminalSet = std::set<std::reference_wrapper<DcTerminal>, stdcxx::less<DcTerminal>>;

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTERMINALSET_HPP
