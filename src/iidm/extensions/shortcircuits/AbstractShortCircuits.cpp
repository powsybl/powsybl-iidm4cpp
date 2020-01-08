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

AbstractShortCircuits::AbstractShortCircuits(Extendable& extendable, double transientReactance, double stepUpTransformerReactance) :
    Extension(extendable),
    m_transientReactance(checkReactance(transientReactance)),
    m_stepUpTransformerReactance(checkReactance(stepUpTransformerReactance)) {

}

double AbstractShortCircuits::checkReactance(double x) {
    if (std::isnan(x)) {
        throw PowsyblException("Reactance is not defined.");
    }
    if (x < 0.0) {
        throw PowsyblException("Reactance may not be negative.");
    }
    return x;
}

double AbstractShortCircuits::getStepUpTransformerReactance() const {
    return m_stepUpTransformerReactance;
}

double AbstractShortCircuits::getTransientReactance() const {
    return m_transientReactance;
}

void AbstractShortCircuits::setStepUpTransformerReactance(double stepUpTransformerReactance) {
    m_stepUpTransformerReactance = checkReactance(stepUpTransformerReactance);
}

void AbstractShortCircuits::setTransientReactance(double transientReactance) {
    m_transientReactance = checkReactance(transientReactance);
}

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
