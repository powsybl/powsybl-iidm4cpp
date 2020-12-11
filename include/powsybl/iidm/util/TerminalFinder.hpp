/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP
#define POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP

#include <functional>

#include <powsybl/stdcxx/range.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Terminal;

namespace TerminalFinder {

using Comparator = std::function<bool(const Terminal& t1, const Terminal& t2)>;

stdcxx::CReference<Terminal> find(const stdcxx::const_range<Terminal>& terminals);

stdcxx::Reference<Terminal> find(const stdcxx::range<Terminal>& terminals);

stdcxx::CReference<Terminal> find(const Comparator& comparator, const stdcxx::const_range<Terminal>& terminals);

stdcxx::Reference<Terminal> find(const Comparator& comparator, const stdcxx::range<Terminal>& terminals);

}  // namespace TerminalFinder

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_UTIL_TERMINALFINDER_HPP
