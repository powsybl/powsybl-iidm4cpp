/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LineCharacteristics.hpp>
#include <powsybl/stdcxx/math.hpp>

#include "ValidationUtils.hpp"

namespace powsybl {

namespace iidm {

LineCharacteristics::LineCharacteristics(const Validable& validable) :
    m_validable(validable) {
}

LineCharacteristics::LineCharacteristics(const Validable& validable, double r, double x, double g1, double b1, double g2, double b2) :
    m_validable(validable) {
    setB1(b1);
    setB2(b2);
    setG1(g1);
    setG2(g2);
    setR(r);
    setX(x);
}

double LineCharacteristics::getB1() const {
    return m_b1;
}

double LineCharacteristics::getB2() const {
    return m_b2;
}

double LineCharacteristics::getG1() const {
    return m_g1;
}

double LineCharacteristics::getG2() const {
    return m_g2;
}

double LineCharacteristics::getR() const {
    return m_r;
}

double LineCharacteristics::getX() const {
    return m_x;
}

void LineCharacteristics::setB1(double b1) {
    m_b1 = checkB1(m_validable, b1);
}

void LineCharacteristics::setB2(double b2) {
    m_b2 = checkB2(m_validable, b2);
}

void LineCharacteristics::setG1(double g1) {
    m_g1 = checkG1(m_validable, g1);
}

void LineCharacteristics::setG2(double g2) {
    m_g2 = checkG2(m_validable, g2);
}

void LineCharacteristics::setR(double r) {
    m_r = checkR(m_validable, r);
}

void LineCharacteristics::setX(double x) {
    m_x = checkX(m_validable, x);
}

}  // namespace iidm

}  // namespace powsybl
