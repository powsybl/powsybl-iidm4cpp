/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#include <powsybl/iidm/HalfLine.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace tie_line {

HalfLine::HalfLine(const std::string& id, const std::string& name, bool fictitious, double xnodeP, double xnodeQ,
                   double r, double x, double g1, double b1, double g2, double b2) :
    m_id(id),
    m_name(name),
    m_lineCharacteristics(*this, r, x, g1, b1, g2, b2),
    m_xnodeP(xnodeP),
    m_xnodeQ(xnodeQ),
    m_fictitious(fictitious) {
}

double HalfLine::getB1() const {
    return m_lineCharacteristics.getB1();
}

double HalfLine::getB2() const {
    return m_lineCharacteristics.getB2();
}

double HalfLine::getG1() const {
    return m_lineCharacteristics.getG1();
}

double HalfLine::getG2() const {
    return m_lineCharacteristics.getG2();
}

const std::string& HalfLine::getId() const {
    return m_id;
}

std::string HalfLine::getMessageHeader() const {
    return stdcxx::format("Half line '%1%': ", m_id);
}

const std::string& HalfLine::getName() const {
    return m_name.empty() ? m_id : m_name;
}

double HalfLine::getR() const {
    return m_lineCharacteristics.getR();
}

double HalfLine::getX() const {
    return m_lineCharacteristics.getX();
}

double HalfLine::getXnodeP() const {
    return m_xnodeP;
}

double HalfLine::getXnodeQ() const {
    return m_xnodeQ;
}

bool HalfLine::isFictitious() const {
    return m_fictitious;
}

HalfLine& HalfLine::setB1(double b1) {
    m_lineCharacteristics.setB1(b1);
    return *this;
}

HalfLine& HalfLine::setB2(double b2) {
    m_lineCharacteristics.setB2(b2);
    return *this;
}

HalfLine& HalfLine::setFictitious(bool fictitious) {
    m_fictitious = fictitious;
    return *this;
}

HalfLine& HalfLine::setG1(double g1) {
    m_lineCharacteristics.setG1(g1);
    return *this;
}

HalfLine& HalfLine::setG2(double g2) {
    m_lineCharacteristics.setG2(g2);
    return *this;
}

void HalfLine::setId(const std::string& id) {
    m_id = id;
}

void HalfLine::setName(const std::string& name) {
    m_name = name;
}

void HalfLine::setParent(TieLine& parent) {
    m_parent = parent;
}

HalfLine& HalfLine::setR(double r) {
    m_lineCharacteristics.setR(r);
    return *this;
}

HalfLine& HalfLine::setX(double x) {
    m_lineCharacteristics.setX(x);
    return *this;
}

HalfLine& HalfLine::setXnodeP(double xnodeP) {
    m_xnodeP = xnodeP;
    return *this;
}

HalfLine& HalfLine::setXnodeQ(double xnodeQ) {
    m_xnodeQ = xnodeQ;
    return *this;
}


}  // namespace tie_line

}  // namespace iidm

}  // namespace powsybl
