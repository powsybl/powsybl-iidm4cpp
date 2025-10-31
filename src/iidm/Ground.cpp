/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Ground.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

Ground::Ground(const std::string& id, const std::string& name) :
    Identifiable(id, name, false) {
}

void Ground::setFictitious(bool fictitious) {
    if (fictitious) {
        throw PowsyblException("The ground cannot be fictitious.");
    }
    else {
        Identifiable::setFictitious(false);
    }
}

const IdentifiableType& Ground::getType() const {
    static IdentifiableType s_type = IdentifiableType::GROUND;
    return s_type;
}

const std::string& Ground::getTypeDescription() const {
    static std::string s_typeDescription = "Ground";

    return s_typeDescription;
}

}  // namespace iidm

}  // namespace powsybl
