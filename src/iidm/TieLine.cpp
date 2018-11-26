/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/ValidationException.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

TieLine::HalfLine::HalfLine() :
    Validable(),
    m_id(),
    m_name(),
    m_lineCharacteristics(*this),
    m_xnodeP(stdcxx::nan()),
    m_xnodeQ(stdcxx::nan()) {
}

double TieLine::HalfLine::getB1() const {
    return m_lineCharacteristics.getB1();
}

double TieLine::HalfLine::getB2() const {
    return m_lineCharacteristics.getB2();
}

double TieLine::HalfLine::getG1() const {
    return m_lineCharacteristics.getG1();
}

double TieLine::HalfLine::getG2() const {
    return m_lineCharacteristics.getG2();
}

const std::string& TieLine::HalfLine::getId() const {
    return m_id;
}

std::string TieLine::HalfLine::getMessageHeader() const {
    return logging::format("Half line '%1%': ", m_id);
}

const std::string& TieLine::HalfLine::getName() const {
    return m_name.empty() ? m_id : m_name;
}

double TieLine::HalfLine::getR() const {
    return m_lineCharacteristics.getR();
}

double TieLine::HalfLine::getX() const {
    return m_lineCharacteristics.getX();
}

double TieLine::HalfLine::getXnodeP() const {
    return m_xnodeP;
}

double TieLine::HalfLine::getXnodeQ() const {
    return m_xnodeQ;
}

TieLine::HalfLine& TieLine::HalfLine::setB1(double b1) {
    m_lineCharacteristics.setB1(b1);
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setB2(double b2) {
    m_lineCharacteristics.setB2(b2);
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setG1(double g1) {
    m_lineCharacteristics.setG1(g1);
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setG2(double g2) {
    m_lineCharacteristics.setG2(g2);
    return *this;
}

void TieLine::HalfLine::setId(const std::string& id) {
    m_id = id;
}

void TieLine::HalfLine::setName(const std::string& name) {
    m_name = name;
}

TieLine::HalfLine& TieLine::HalfLine::setR(double r) {
    m_lineCharacteristics.setR(r);
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setX(double x) {
    m_lineCharacteristics.setX(x);
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setXnodeP(double xnodeP) {
    m_xnodeP = xnodeP;
    return *this;
}

TieLine::HalfLine& TieLine::HalfLine::setXnodeQ(double xnodeQ) {
    m_xnodeQ = xnodeQ;
    return *this;
}

TieLine::TieLine(const std::string& id, const std::string& name, const std::string& ucteXnodeCode, HalfLine& half1, HalfLine& half2) :
    Line(id, name),
    m_half1(half1),
    m_half2(half2),
    m_ucteXnodeCode(ucteXnodeCode) {
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

TieLine::HalfLine& TieLine::getHalf1() {
    return m_half1;
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
