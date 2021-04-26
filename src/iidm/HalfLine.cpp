/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#include <powsybl/iidm/HalfLine.hpp>

#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace tie_line {

HalfLine::HalfLine(const std::string& id, const std::string& name, bool fictitious,
                   double r, double x, double g1, double b1, double g2, double b2, const Branch::Side& side) :
    m_id(id),
    m_name(name),
    m_lineCharacteristics(*this, r, x, g1, b1, g2, b2),
    m_fictitious(fictitious),
    m_boundary(stdcxx::make_unique<Boundary>(*this, side)) {
}

HalfLine::HalfLine(HalfLine&& halfLine) noexcept :
    m_parent(halfLine.m_parent),
    m_id(std::move(halfLine.m_id)),
    m_name(std::move(halfLine.m_name)),
    m_lineCharacteristics(*this, halfLine.getR(), halfLine.getX(), halfLine.getG1(), halfLine.getB1(), halfLine.getG2(), halfLine.getB2()),
    m_fictitious(halfLine.m_fictitious),
    m_boundary(stdcxx::make_unique<Boundary>(*this, *halfLine.getBoundary().getSide())) {
}

double HalfLine::getB1() const {
    return m_lineCharacteristics.getB1();
}

double HalfLine::getB2() const {
    return m_lineCharacteristics.getB2();
}

const half_line::Boundary& HalfLine::getBoundary() const {
    return *m_boundary;
}

half_line::Boundary& HalfLine::getBoundary() {
    return *m_boundary;
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

const TieLine& HalfLine::getParent() const {
    return m_parent;
}

TieLine& HalfLine::getParent() {
    return m_parent;
}

double HalfLine::getR() const {
    return m_lineCharacteristics.getR();
}

double HalfLine::getX() const {
    return m_lineCharacteristics.getX();
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

void HalfLine::setParent(TieLine& parent) {
    if (m_parent) {
        throw AssertionError(stdcxx::format("TieLine.HalfLine already attached to %1%", m_parent.get().getId()));
    }
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

}  // namespace tie_line

}  // namespace iidm

}  // namespace powsybl
