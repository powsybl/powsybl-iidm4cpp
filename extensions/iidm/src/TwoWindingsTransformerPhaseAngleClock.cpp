/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerPhaseAngleClock.hpp>

#include <powsybl/AssertionError.hpp>
#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/stdcxx/MessageFormat.hpp>
#include <powsybl/stdcxx/demangle.hpp>
#include <powsybl/stdcxx/instanceof.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerPhaseAngleClock::TwoWindingsTransformerPhaseAngleClock(TwoWindingsTransformer& transformer, unsigned long phaseAngleClock) :
    Extension(transformer),
    m_phaseAngleClock(checkPhaseAngleClock(phaseAngleClock)) {
}

void TwoWindingsTransformerPhaseAngleClock::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<TwoWindingsTransformer>()));
    }
}

unsigned long TwoWindingsTransformerPhaseAngleClock::checkPhaseAngleClock(unsigned long phaseAngleClock) const {
    if (phaseAngleClock > 11) {
        throw PowsyblException(stdcxx::format("Unexpected value for phaseAngleClock: %1%1", phaseAngleClock));
    }
    return phaseAngleClock;
}

const std::string& TwoWindingsTransformerPhaseAngleClock::getName() const {
    static std::string s_name = "twoWindingsTransformerPhaseAngleClock";
    return s_name;
}

unsigned long TwoWindingsTransformerPhaseAngleClock::getPhaseAngleClock() const {
    return m_phaseAngleClock;
}

const std::type_index& TwoWindingsTransformerPhaseAngleClock::getType() const {
    static std::type_index s_type = typeid(TwoWindingsTransformerPhaseAngleClock);
    return s_type;
}

TwoWindingsTransformerPhaseAngleClock& TwoWindingsTransformerPhaseAngleClock::setPhaseAngleClock(unsigned long phaseAngleClock) {
    m_phaseAngleClock = checkPhaseAngleClock(phaseAngleClock);
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
