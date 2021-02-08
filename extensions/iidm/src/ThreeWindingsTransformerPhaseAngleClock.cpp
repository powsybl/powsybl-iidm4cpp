/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerPhaseAngleClock.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/PhaseTapChanger.hpp>
#include <powsybl/iidm/RatioTapChanger.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/reference.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerPhaseAngleClock::ThreeWindingsTransformerPhaseAngleClock(ThreeWindingsTransformer& transformer, unsigned long phaseAngleClockLeg2, unsigned long phaseAngleClockLeg3) :
    Extension(transformer),
    m_phaseAngleClockLeg2(checkPhaseAngleClock(phaseAngleClockLeg2)),
    m_phaseAngleClockLeg3(checkPhaseAngleClock(phaseAngleClockLeg3)){
}

void ThreeWindingsTransformerPhaseAngleClock::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<ThreeWindingsTransformer>()));
    }
}

unsigned long ThreeWindingsTransformerPhaseAngleClock::checkPhaseAngleClock(unsigned long phaseAngleClock) const {
    if (phaseAngleClock > 11) {
        throw PowsyblException(stdcxx::format("Unexpected value for phaseAngleClock: %1%", phaseAngleClock));
    }
    return phaseAngleClock;
}

const std::string& ThreeWindingsTransformerPhaseAngleClock::getName() const {
    static std::string s_name = "threeWindingsTransformerPhaseAngleClock";
    return s_name;
}

unsigned long ThreeWindingsTransformerPhaseAngleClock::getPhaseAngleClockLeg2() const {
    return m_phaseAngleClockLeg2;
}

unsigned long ThreeWindingsTransformerPhaseAngleClock::getPhaseAngleClockLeg3() const {
    return m_phaseAngleClockLeg3;
}

const std::type_index& ThreeWindingsTransformerPhaseAngleClock::getType() const {
    static std::type_index s_type = typeid(ThreeWindingsTransformerPhaseAngleClock);
    return s_type;
}

ThreeWindingsTransformerPhaseAngleClock& ThreeWindingsTransformerPhaseAngleClock::setPhaseAngleClockLeg2(unsigned long phaseAngleClockLeg2) {
    m_phaseAngleClockLeg2 = checkPhaseAngleClock(phaseAngleClockLeg2);
    return *this;
}

ThreeWindingsTransformerPhaseAngleClock& ThreeWindingsTransformerPhaseAngleClock::setPhaseAngleClockLeg3(unsigned long phaseAngleClockLeg3) {
    m_phaseAngleClockLeg3 = checkPhaseAngleClock(phaseAngleClockLeg3);
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
