/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Branch.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/logging/MessageFormat.hpp>

namespace powsybl {

namespace iidm {

Branch::Branch(const std::string& id, const std::string& name, const ConnectableType& connectableType) :
    Connectable(id, name, connectableType) {
}

Terminal& Branch::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getTerminals().at(0).get();

        case Side::TWO:
            return getTerminals().at(1).get();

        default:
            throw AssertionError(logging::format("Unexpected side value: %1%", static_cast<unsigned int>(side)));
    }
}

Terminal& Branch::getTerminal1() const {
    return getTerminal(Side::ONE);
}

Terminal& Branch::getTerminal2() const {
    return getTerminal(Side::TWO);
}

const std::string& getSideName(const Branch::Side& side) {
    static std::array<std::string, 2> s_sideNames {{ "ONE", "TWO" }};

    return s_sideNames.at(static_cast<unsigned int>(side));
}

std::ostream& operator<<(std::ostream& stream, const Branch::Side& side) {
    stream << getSideName(side);
    return stream;
}

}  // namespace iidm

}  // namespace powsybl
