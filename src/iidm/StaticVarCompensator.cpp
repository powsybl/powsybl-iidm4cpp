/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StaticVarCompensator.hpp>

namespace powsybl {

namespace iidm {

StaticVarCompensator::StaticVarCompensator(const std::string& id, const std::string& name) :
    Injection(id, name, ConnectableType::STATIC_VAR_COMPENSATOR) {

}

}  // namespace iidm

}  // namespace powsybl
