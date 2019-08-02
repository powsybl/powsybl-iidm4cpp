/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TERMINAL_HXX
#define POWSYBL_IIDM_TERMINAL_HXX

#include <powsybl/iidm/Terminal.hpp>

#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
bool Terminal::isInstanceOf(const std::reference_wrapper<Terminal>& terminal) {
    const auto& connectable = terminal.get().getConnectable();
    return static_cast<bool>(connectable) && stdcxx::isInstanceOf<T>(connectable.get());
}

template <typename T, typename>
T& Terminal::map(const std::reference_wrapper<Terminal>& terminal) {
    return dynamic_cast<T&>(terminal.get().getConnectable().get());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TERMINAL_HXX
