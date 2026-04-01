/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuit.hpp>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

template<typename S>
AbstractShortCircuit<S>::AbstractShortCircuit(Extendable& extendable, double directSubtransX, double directTransX, double stepUpTransformerX) :
    Extension(extendable),
    m_directSubtransX(directSubtransX),
    m_directTransX(checkTransX(directTransX)),
    m_stepUpTransformerX(stepUpTransformerX) {

}

template<typename S>
double AbstractShortCircuit<S>::checkTransX(double x) {
    if (std::isnan(x)) {
        throw PowsyblException("Undefined directTransX");
    }
    return x;
}

template<typename S>
double AbstractShortCircuit<S>::getDirectSubtransX() const {
    return m_directSubtransX;
}

template<typename S>
double AbstractShortCircuit<S>::getDirectTransX() const {
    return m_directTransX;
}

template<typename S>
double AbstractShortCircuit<S>::getStepUpTransformerX() const {
    return m_stepUpTransformerX;
}

template<typename S>
S& AbstractShortCircuit<S>::setDirectSubtransX(double directSubtransX) {
    m_directSubtransX = directSubtransX;
    return static_cast<S&>(*this);
}

template<typename S>
S& AbstractShortCircuit<S>::setDirectTransX(double directTransX) {
    m_directTransX = checkTransX(directTransX);
    return static_cast<S&>(*this);
}

template<typename S>
S& AbstractShortCircuit<S>::setStepUpTransformerX(double setUpTransformerX) {
    m_stepUpTransformerX = setUpTransformerX;
    return static_cast<S&>(*this);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
