/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUIT_HPP

#include <string>

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

template<typename S>
class AbstractShortCircuit : public Extension {
protected:
    AbstractShortCircuit(Extendable& extendable, double directSubtransX, double directTransX, double stepUpTransformerX);

public:
    ~AbstractShortCircuit() noexcept override = default;

    /**
     * Get the direct-axis subtransient reactance (also known as X''d)
     */
    double getDirectSubtransX() const;

    /**
     * Get the direct-axis transient reactance (also known as X'd)
     */
    double getDirectTransX() const;

    /**
     * Get the step-up transformer reactance if the generator has a non-modeled step-up transformer.
     */
    double getStepUpTransformerX() const;

    /**
     * Set the direct-axis subtransient reactance (also known as X''d)
     */
    S& setDirectSubtransX(double directSubtransX);

    /**
     * Set the direct-axis transient reactance (also known as X'd)
     */
    S& setDirectTransX(double directTransX);

    /**
     * Set the step-up transformer reactance
     */
    S& setStepUpTransformerX(double setUpTransformerX);

protected:
    static double checkTransX(double x);

private:
    double m_directSubtransX;

    double m_directTransX;

    double m_stepUpTransformerX;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuit.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUIT_HPP
