/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClockAdder.hpp>

#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClock.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerPhaseAngleClockAdder::TwoWindingsTransformerPhaseAngleClockAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> TwoWindingsTransformerPhaseAngleClockAdder::createExtension(Extendable& extendable) const {
    if (!m_phaseAngleClock) {
        throw PowsyblException(stdcxx::format("Undefined value for phaseAngleClock"));
    }
    if (stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable)) {
        return std::unique_ptr<TwoWindingsTransformerPhaseAngleClock>(new TwoWindingsTransformerPhaseAngleClock(dynamic_cast<TwoWindingsTransformer&>(extendable), *m_phaseAngleClock));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<TwoWindingsTransformer>()));
}

TwoWindingsTransformerPhaseAngleClockAdder& TwoWindingsTransformerPhaseAngleClockAdder::withPhaseAngleClock(unsigned long phaseAngleClock) {
    m_phaseAngleClock = phaseAngleClock;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
