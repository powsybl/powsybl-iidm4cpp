/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class TwoWindingsTransformerPhaseAngleClockAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit TwoWindingsTransformerPhaseAngleClockAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~TwoWindingsTransformerPhaseAngleClockAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    TwoWindingsTransformerPhaseAngleClockAdder(const TwoWindingsTransformerPhaseAngleClockAdder&) = default;

    /**
     * Move constructor
     */
    TwoWindingsTransformerPhaseAngleClockAdder(TwoWindingsTransformerPhaseAngleClockAdder&&) = default;

    /**
     * Copy assignment operator
     */
    TwoWindingsTransformerPhaseAngleClockAdder& operator=(const TwoWindingsTransformerPhaseAngleClockAdder&) = delete;

    /**
     * Move assignment operator
     */
    TwoWindingsTransformerPhaseAngleClockAdder& operator=(TwoWindingsTransformerPhaseAngleClockAdder&&) = delete;

    /**
     * Set phase angle clock
     *
     * @param phaseAngleClock The phase angle clock
     *
     * @return this TwoWindingsTransformerPhaseAngleClockAdder object
     */
    TwoWindingsTransformerPhaseAngleClockAdder& withPhaseAngleClock(unsigned long phaseAngleClock);

protected:
    /**
     * Creates the TwoWindingsTransformerPhaseAngleClock extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    unsigned long m_phaseAngleClock = 12UL;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
