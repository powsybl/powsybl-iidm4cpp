/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Line.hpp>

#include <powsybl/iidm/ValidationUtils.hpp>

namespace powsybl {

namespace iidm {

Line::Line(const std::string& id, const std::string& name, bool fictitious) :
    Branch(id, name, fictitious),
    m_lineCharacteristics(*this) {
}

Line::Line(const std::string& id, const std::string& name, bool fictitious, double r, double x, double g1, double b1, double g2, double b2) :
    Branch(id, name, fictitious),
    m_lineCharacteristics(*this, r, x, g1, b1, g2, b2) {
}

double Line::getB1() const {
    return m_lineCharacteristics.getB1();
}

double Line::getB2() const {
    return m_lineCharacteristics.getB2();
}

double Line::getG1() const {
    return m_lineCharacteristics.getG1();
}

double Line::getG2() const {
    return m_lineCharacteristics.getG2();
}

double Line::getR() const {
    return m_lineCharacteristics.getR();
}

const IdentifiableType& Line::getType() const {
    static IdentifiableType s_type = IdentifiableType::LINE;
    return s_type;
}

const std::string& Line::getTypeDescription() const {
    static std::string s_typeDescription = "AC line";

    return s_typeDescription;
}

double Line::getX() const {
    return m_lineCharacteristics.getX();
}

bool Line::isTieLine() const {
    return false;
}

Line& Line::setB1(double b1) {
    m_lineCharacteristics.setB1(b1);
    return *this;
}

Line& Line::setB2(double b2) {
    m_lineCharacteristics.setB2(b2);
    return *this;
}

Line& Line::setG1(double g1) {
    m_lineCharacteristics.setG1(g1);
    return *this;
}

Line& Line::setG2(double g2) {
    m_lineCharacteristics.setG2(g2);
    return *this;
}

Line& Line::setR(double r) {
    m_lineCharacteristics.setR(r);
    return *this;
}

Line& Line::setX(double x) {
    m_lineCharacteristics.setX(x);
    return *this;
}

}  // namespace iidm

}  // namespace powsybl
