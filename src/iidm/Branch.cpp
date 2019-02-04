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

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits(const Side& side) const {
    return stdcxx::cref(getCurrentLimitsPtr(side));
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits(const Side& side) {
    return stdcxx::ref(getCurrentLimitsPtr(side));
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits1() const {
    return getCurrentLimits(Side::ONE);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits1() {
    return getCurrentLimits(Side::ONE);
}

stdcxx::CReference<CurrentLimits> Branch::getCurrentLimits2() const {
    return getCurrentLimits(Side::TWO);
}

stdcxx::Reference<CurrentLimits> Branch::getCurrentLimits2() {
    return getCurrentLimits(Side::TWO);
}

const std::unique_ptr<CurrentLimits>& Branch::getCurrentLimitsPtr(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_limits1;

        case Side::TWO:
            return m_limits2;

        default:
            throw AssertionError(logging::format("Unexpected side value: %1%", side));
    }
}

Terminal& Branch::getTerminal(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return getTerminals().at(0).get();

        case Side::TWO:
            return getTerminals().at(1).get();

        default:
            throw AssertionError(logging::format("Unexpected side value: %1%", side));
    }
}

Terminal& Branch::getTerminal1() const {
    return getTerminal(Side::ONE);
}

Terminal& Branch::getTerminal2() const {
    return getTerminal(Side::TWO);
}

CurrentLimitsAdder<Branch::Side, Branch> Branch::newCurrentLimits1() {
    return CurrentLimitsAdder<Branch::Side, Branch>(Side::ONE, *this);
}

CurrentLimitsAdder<Branch::Side, Branch> Branch::newCurrentLimits2() {
    return CurrentLimitsAdder<Branch::Side, Branch>(Side::TWO, *this);
}

void Branch::setCurrentLimits(const Branch::Side& side, std::unique_ptr<CurrentLimits> limits) {
    switch (side) {
        case Side::ONE:
            m_limits1 = std::move(limits);
            break;

        case Side::TWO:
            m_limits2 = std::move(limits);
            break;

        default:
            throw AssertionError(logging::format("Unexpected side value: %1%", side));
    }
}

std::string getSideName(const Branch::Side& side) {
    static std::array<std::string, 2> s_sideNames {{ "ONE", "TWO" }};

    return logging::toString(s_sideNames, side);
}

std::ostream& operator<<(std::ostream& stream, const Branch::Side& side) {
    stream << getSideName(side);
    return stream;
}

}  // namespace iidm

}  // namespace powsybl
