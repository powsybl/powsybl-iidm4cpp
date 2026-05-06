/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/DcLine.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

DcLine::DcLine(const std::string& id, const std::string& name, bool fictitious, double r) :
    Identifiable(id, name, fictitious),
    m_r(checkDoubleParamPositive(*this, r, "r")) {
}

const IdentifiableType& DcLine::getType() const {
    static IdentifiableType s_type = IdentifiableType::DC_LINE;
    return s_type;
}

const std::string& DcLine::getTypeDescription() const {
    static std::string s_typeDescription = "DC Line";
    return s_typeDescription;
}

const DcTerminal& DcLine::getDcTerminal1() const {
    return DcConnectable::getDcTerminal(0);
}
DcTerminal& DcLine::getDcTerminal1() {
    return DcConnectable::getDcTerminal(0);
}

const DcTerminal& DcLine::getDcTerminal2() const {
    return DcConnectable::getDcTerminal(1);
}
DcTerminal& DcLine::getDcTerminal2() {
    return DcConnectable::getDcTerminal(1);
}

const DcTerminal& DcLine::getDcTerminal(const TwoSides& side) const {
    switch (side) {
        case TwoSides::ONE:
            return getDcTerminal1();

        case TwoSides::TWO:
            return getDcTerminal2();

        case TwoSides::UNDEFINED:
        default:
            throw AssertionError(stdcxx::format("Unexpected TwoSides value: %1%", side));
    }
}
DcTerminal& DcLine::getDcTerminal(const TwoSides& side) {
    return const_cast<DcTerminal&>(static_cast<const DcLine*>(this)->getDcTerminal(side));
}

TwoSides DcLine::getSide(const DcTerminal& terminal) const {
    if (stdcxx::areSame(terminal, getDcTerminal1())) {
        return TwoSides::ONE;
    }
    if (stdcxx::areSame(terminal, getDcTerminal2())) {
        return TwoSides::TWO;
    }

    throw AssertionError("The DC terminal is not connected to this DC Line");
}

double DcLine::getR() const {
    return m_r;
}
DcLine& DcLine::setR(double r) {
    checkDoubleParamPositive(*this, r, "r");
    m_r = r;
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
