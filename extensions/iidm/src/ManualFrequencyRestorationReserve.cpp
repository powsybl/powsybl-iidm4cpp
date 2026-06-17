/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ManualFrequencyRestorationReserve.hpp>

#include <powsybl/iidm/Injection.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ManualFrequencyRestorationReserve::ManualFrequencyRestorationReserve(Injection& injection, bool participate) :
    Extension(injection),
    m_participate(participate) {
}

void ManualFrequencyRestorationReserve::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Injection>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Injection>()));
    }
}

const std::string& ManualFrequencyRestorationReserve::getName() const {
    static std::string s_name = "manualFrequencyRestorationReserve";
    return s_name;
}

const std::type_index& ManualFrequencyRestorationReserve::getType() const {
    static std::type_index s_type = typeid(ManualFrequencyRestorationReserve);
    return s_type;
}

bool ManualFrequencyRestorationReserve::isParticipate() const {
    return m_participate;
}

ManualFrequencyRestorationReserve& ManualFrequencyRestorationReserve::setParticipate(bool participate) {
    m_participate = participate;
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
