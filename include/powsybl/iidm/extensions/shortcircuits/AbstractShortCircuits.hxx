/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/shortcircuits/AbstractShortCircuits.hpp>

#include <cmath>

#include <powsybl/PowsyblException.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

template <typename T>
AbstractShortCircuits<T>::AbstractShortCircuits(Extendable& extendable, double transientReactance, double stepUpTransformerReactance) :
    Extension(extendable),
    m_transientReactance(checkReactance(transientReactance)),
    m_stepUpTransformerReactance(checkReactance(stepUpTransformerReactance)) {

}

template <typename T>
double AbstractShortCircuits<T>::checkReactance(double x) {
    if (std::isnan(x)) {
        throw PowsyblException("Reactance is not defined.");
    }
    if (x < 0.0) {
        throw PowsyblException("Reactance may not be negative.");
    }
    return x;
}

template <typename T>
double AbstractShortCircuits<T>::getStepUpTransformerReactance() const {
    return m_stepUpTransformerReactance;
}

template <typename T>
double AbstractShortCircuits<T>::getTransientReactance() const {
    return m_transientReactance;
}

template <typename T>
void AbstractShortCircuits<T>::setStepUpTransformerReactance(double stepUpTransformerReactance) {
    m_stepUpTransformerReactance = checkReactance(stepUpTransformerReactance);
}

template <typename T>
void AbstractShortCircuits<T>::setTransientReactance(double transientReactance) {
    m_transientReactance = checkReactance(transientReactance);
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

