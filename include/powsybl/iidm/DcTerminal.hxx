/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTERMINAL_HXX
#define POWSYBL_IIDM_DCTERMINAL_HXX

#include <powsybl/iidm/DcTerminal.hpp>

#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

template <typename T, typename>
bool DcTerminal::isInstanceOf(const DcTerminal& dcTerminal) {
    const auto& dcConnectable = dcTerminal.getDcConnectable();
    return static_cast<bool>(dcConnectable) && stdcxx::isInstanceOf<T>(dcConnectable.get());
}

template <typename T, typename>
const T& DcTerminal::map(const DcTerminal& dcTerminal) {
    return dynamic_cast<const T&>(dcTerminal.getDcConnectable().get());
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTERMINAL_HXX
