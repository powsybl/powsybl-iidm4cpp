/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_ABSTRACTSHORTCIRCUITS_HPP
#define POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_ABSTRACTSHORTCIRCUITS_HPP

#include <string>

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace shortcircuits {

class AbstractShortCircuits : public Extension {
public:
    AbstractShortCircuits(Extendable& extendable, double transientReactance, double stepUpTransformerReactance);

    ~AbstractShortCircuits() noexcept override = default;

    double getStepUpTransformerReactance() const;

    double getTransientReactance() const;

    void setStepUpTransformerReactance(double stepUpTransformerReactance);

    void setTransientReactance(double transientReactance);

private:
    static double checkReactance(double x);

private:
    double m_transientReactance;

    double m_stepUpTransformerReactance;
};

}  // namespace shortcircuits

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SHORTCIRCUITS_ABSTRACTSHORTCIRCUITS_HPP
