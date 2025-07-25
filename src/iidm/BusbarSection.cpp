/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/BusbarSection.hpp>

#include "NodeTerminal.hpp"

namespace powsybl {

namespace iidm {

BusbarSection::BusbarSection(const std::string& id, const std::string& name, bool fictitious) :
    Injection(id, name, fictitious) {
}

double BusbarSection::getAngle() const {
    const auto& terminal = dynamic_cast<const NodeTerminal&>(getTerminal());

    return terminal.getAngle();
}

const IdentifiableType& BusbarSection::getType() const {
    static IdentifiableType s_type = IdentifiableType::BUSBAR_SECTION;
    return s_type;
}

const std::string& BusbarSection::getTypeDescription() const {
    static std::string s_typeDescription = "Busbar section";

    return s_typeDescription;
}

double BusbarSection::getV() const {
    const auto& terminal = dynamic_cast<const NodeTerminal&>(getTerminal());

    return terminal.getV();
}

}  // namespace iidm

}  // namespace powsybl
