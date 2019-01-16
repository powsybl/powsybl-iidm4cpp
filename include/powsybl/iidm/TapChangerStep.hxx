/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEP_HXX
#define POWSYBL_IIDM_TAPCHANGERSTEP_HXX

#include <powsybl/iidm/TapChangerStep.hpp>

namespace powsybl {

namespace iidm {

template<typename S>
TapChangerStep<S>::TapChangerStep(double rho, double r, double x, double g, double b) :
    m_rho(rho),
    m_r(r),
    m_x(x),
    m_g(g),
    m_b(b) {
}

template<typename S>
double TapChangerStep<S>::getB() const {
    return m_b;
}

template<typename S>
double TapChangerStep<S>::getG() const {
    return m_g;
}

template<typename S>
double TapChangerStep<S>::getR() const {
    return m_r;
}

template<typename S>
double TapChangerStep<S>::getRho() const {
    return m_rho;
}

template<typename S>
double TapChangerStep<S>::getX() const {
    return m_x;
}

template<typename S>
S& TapChangerStep<S>::setB(double b) {
    m_b = b;
    return static_cast<S&>(*this);
}

template<typename S>
S& TapChangerStep<S>::setG(double g) {
    m_g = g;
    return static_cast<S&>(*this);
}

template<typename S>
S& TapChangerStep<S>::setR(double r) {
    m_r = r;
    return static_cast<S&>(*this);
}

template<typename S>
S& TapChangerStep<S>::setRho(double rho) {
    m_rho = rho;
    return static_cast<S&>(*this);
}

template<typename S>
S& TapChangerStep<S>::setX(double x) {
    m_x = x;
    return static_cast<S&>(*this);
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGERSTEP_HXX
