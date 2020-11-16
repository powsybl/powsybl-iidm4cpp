/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/NetworkVariant.hpp>

#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace network {

std::unique_ptr<VariantImpl> VariantImpl::copy() const {
    return stdcxx::make_unique<VariantImpl>();
}

}  // namespace network

}  // namespace iidm

}  // namespace powsybl
