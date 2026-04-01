/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUITADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUITADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

template<typename A>
class AbstractShortCircuitAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit AbstractShortCircuitAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~AbstractShortCircuitAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    AbstractShortCircuitAdder(const AbstractShortCircuitAdder&) = default;

    /**
     * Move constructor
     */
    AbstractShortCircuitAdder(AbstractShortCircuitAdder&&) = default;

    /**
     * Copy assignment operator
     */
    AbstractShortCircuitAdder& operator=(const AbstractShortCircuitAdder&) = delete;

    /**
     * Move assignment operator
     */
    AbstractShortCircuitAdder& operator=(AbstractShortCircuitAdder&&) = delete;

    /**
     * Set the direct-axis subtransient reactance (also known as X''d)
     */
    A& withDirectSubtransX(double directSubtransX);

    /**
     * Set the direct-axis transient reactance (also known as X'd)
     */
    A& withDirectTransX(double directTransX);

    /**
     * Set the step-up transformer reactance
     */
    A& withStepUpTransformerX(double stepUpTransformerX);

protected:
    double m_directTransX = 0.0;

    double m_directSubtransX = stdcxx::nan();

    double m_stepUpTransformerX = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/extensions/iidm/AbstractShortCircuitAdder.hxx>

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_ABSTRACTSHORTCIRCUITADDER_HPP
