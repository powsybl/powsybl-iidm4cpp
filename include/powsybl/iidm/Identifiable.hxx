/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_IDENTIFIABLE_HXX
#define POWSYBL_IIDM_IDENTIFIABLE_HXX

#include <powsybl/iidm/Identifiable.hpp>

namespace powsybl {

namespace iidm {

template <typename T>
bool Identifiable::isInstanceOf(const Identifiable& identifiable) {
    return stdcxx::isInstanceOf<T>(identifiable);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_IDENTIFIABLE_HXX
