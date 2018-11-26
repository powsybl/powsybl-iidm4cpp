/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LINECHARACTERISTICS_HPP
#define POWSYBL_IIDM_LINECHARACTERISTICS_HPP

namespace powsybl {

namespace iidm {

class Validable;

class LineCharacteristics {
public:
    LineCharacteristics(const Validable& validable);

    LineCharacteristics(const Validable& validable, double r, double x, double g1, double b1, double g2, double b2);

    ~LineCharacteristics() noexcept = default;

    double getB1() const;

    double getB2() const;

    double getG1() const;

    double getG2() const;

    double getR() const;

    double getX() const;

    void setB1(double b1);

    void setB2(double b2);

    void setG1(double g1);

    void setG2(double g2);

    void setR(double r);

    void setX(double x);

private:
    const Validable& m_validable;

    double m_b1;

    double m_b2;

    double m_g1;

    double m_g2;

    double m_r;

    double m_x;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LINECHARACTERISTICS_HPP
