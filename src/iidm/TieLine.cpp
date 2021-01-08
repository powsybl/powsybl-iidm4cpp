/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TieLine.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

TieLine::TieLine(const std::string& id, const std::string& name, bool fictitious, const std::string& ucteXnodeCode, HalfLine&& half1, HalfLine&& half2) :
    Line(id, name, fictitious),
    m_half1(std::move(half1)),
    m_half2(std::move(half2)),
    m_ucteXnodeCode(ucteXnodeCode) {
    attach(m_half1);
    attach(m_half2);
}

TieLine::HalfLine& TieLine::attach(TieLine::HalfLine& halfLine) {
    halfLine.setParent(*this);
    return halfLine;
}

double TieLine::getB1() const {
    return m_half1.getB1() + m_half2.getB1();
}

double TieLine::getB2() const {
    return m_half1.getB2() + m_half2.getB2();
}

double TieLine::getG1() const {
    return m_half1.getG1() + m_half2.getG1();
}

double TieLine::getG2() const {
    return m_half1.getG2() + m_half2.getG2();
}

const TieLine::HalfLine& TieLine::getHalf(const Side& side) const {
    switch (side) {
        case Side::ONE:
            return m_half1;
        case Side::TWO:
            return m_half2;
        default:
            throw AssertionError(stdcxx::format("Unknown branch side %1%", side));
    }
}

TieLine::HalfLine& TieLine::getHalf(const Side& side) {
    return const_cast<HalfLine&>(static_cast<const TieLine*>(this)->getHalf(side)); //NOSONAR
}

const TieLine::HalfLine& TieLine::getHalf1() const {
    return m_half1;
}

TieLine::HalfLine& TieLine::getHalf1() {
    return m_half1;
}

const TieLine::HalfLine& TieLine::getHalf2() const {
    return m_half2;
}

TieLine::HalfLine& TieLine::getHalf2() {
    return m_half2;
}

double TieLine::getR() const {
    return m_half1.getR() + m_half2.getR();
}

const std::string& TieLine::getTypeDescription() const {
    static std::string s_typeDescription = "AC tie line";

    return s_typeDescription;
}

const std::string& TieLine::getUcteXnodeCode() const {
    return m_ucteXnodeCode;
}

double TieLine::getX() const {
    return m_half1.getX() + m_half2.getX();
}

bool TieLine::isTieLine() const {
    return true;
}

TieLine& TieLine::setB1(double /*b1*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

TieLine& TieLine::setB2(double /*b2*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

TieLine& TieLine::setG1(double /*g1*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

TieLine& TieLine::setG2(double /*g2*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

TieLine& TieLine::setR(double /*r*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

TieLine& TieLine::setX(double /*x*/) {
    throw ValidationException(*this, "direct modification of characteristics not supported for tie lines");
}

}  // namespace iidm

}  // namespace powsybl
