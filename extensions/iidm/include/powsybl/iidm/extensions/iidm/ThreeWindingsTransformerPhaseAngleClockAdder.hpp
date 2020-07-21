/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCKADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCKADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerPhaseAngleClockAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ThreeWindingsTransformerPhaseAngleClockAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~ThreeWindingsTransformerPhaseAngleClockAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    ThreeWindingsTransformerPhaseAngleClockAdder(const ThreeWindingsTransformerPhaseAngleClockAdder&) = default;

    /**
     * Move constructor
     */
    ThreeWindingsTransformerPhaseAngleClockAdder(ThreeWindingsTransformerPhaseAngleClockAdder&&) = default;

    /**
     * Copy assignment operator
     */
    ThreeWindingsTransformerPhaseAngleClockAdder& operator=(const ThreeWindingsTransformerPhaseAngleClockAdder&) = delete;

    /**
     * Move assignment operator
     */
    ThreeWindingsTransformerPhaseAngleClockAdder& operator=(ThreeWindingsTransformerPhaseAngleClockAdder&&) = delete;

    /**
     * Set phase angle clock value for leg2
     *
     * @param phaseAngleClockLeg2 The phase angle clock
     *
     * @return this ThreeWindingsTransformerPhaseAngleClockAdder object
     */
    ThreeWindingsTransformerPhaseAngleClockAdder& withPhaseAngleClockLeg2(unsigned long phaseAngleClockLeg2);

    /**
     * Set phase angle clock value for leg3
     *
     * @param phaseAngleClockLeg3 The phase angle clock
     *
     * @return this ThreeWindingsTransformerPhaseAngleClockAdder object
     */
    ThreeWindingsTransformerPhaseAngleClockAdder& withPhaseAngleClockLeg3(unsigned long phaseAngleClockLeg3);

protected:
    /**
     * Creates the ThreeWindingsTransformerPhaseAngleClock extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    unsigned long m_phaseAngleClockLeg2 = 12UL;

    unsigned long m_phaseAngleClockLeg3 = 12UL;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERPHASEANGLECLOCKADDER_HPP
