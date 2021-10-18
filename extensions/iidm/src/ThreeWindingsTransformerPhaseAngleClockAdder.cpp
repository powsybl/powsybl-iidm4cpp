/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClockAdder.hpp>

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerPhaseAngleClockAdder::ThreeWindingsTransformerPhaseAngleClockAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ThreeWindingsTransformerPhaseAngleClockAdder::createExtension(Extendable& extendable) {
    if (!m_phaseAngleClockLeg2) {
        throw PowsyblException(stdcxx::format("Undefined value for phaseAngleClockLeg2"));
    }
    if (!m_phaseAngleClockLeg3) {
        throw PowsyblException(stdcxx::format("Undefined value for phaseAngleClockLeg3"));
    }
    if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable)) {
        return std::unique_ptr<ThreeWindingsTransformerPhaseAngleClock>(new ThreeWindingsTransformerPhaseAngleClock(dynamic_cast<ThreeWindingsTransformer&>(extendable), *m_phaseAngleClockLeg2, *m_phaseAngleClockLeg3));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<ThreeWindingsTransformer>()));
}

ThreeWindingsTransformerPhaseAngleClockAdder& ThreeWindingsTransformerPhaseAngleClockAdder::withPhaseAngleClockLeg2(unsigned long phaseAngleClockLeg2) {
    m_phaseAngleClockLeg2 = phaseAngleClockLeg2;
    return *this;
}

ThreeWindingsTransformerPhaseAngleClockAdder& ThreeWindingsTransformerPhaseAngleClockAdder::withPhaseAngleClockLeg3(unsigned long phaseAngleClockLeg3) {
    m_phaseAngleClockLeg3 = phaseAngleClockLeg3;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
