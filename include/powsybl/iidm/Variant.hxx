/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_VARIANT_HXX
#define POWSYBL_IIDM_VARIANT_HXX

#include <powsybl/iidm/Variant.hpp>

namespace powsybl {

namespace iidm {

template <typename Owner, typename V>
Variant<Owner, V>::Variant(Owner& owner) :
    m_owner(owner) {
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_VARIANT_HXX
