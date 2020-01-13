/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/cvg/ActiveSeason.hpp>

#include <powsybl/iidm/Connectable.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace cvg {

ActiveSeason::ActiveSeason(Connectable& connectable, const std::string& value) :
    Extension(connectable),
    m_value(value) {
}

void ActiveSeason::assertExtendable(const stdcxx::Reference<powsybl::iidm::Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Connectable>(extendable.get())) {
        throw AssertionError(logging::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Connectable>()));
    }
}

const std::string& ActiveSeason::getName() const {
    static std::string s_name = "activeSeason";
    return s_name;
}

const std::type_index& ActiveSeason::getType() const {
    static std::type_index s_type = typeid(ActiveSeason);
    return s_type;
}

const std::string& ActiveSeason::getValue() const {
    return m_value;
}

ActiveSeason& ActiveSeason::setValue(const std::string& value) {
    m_value = value;
    return *this;
}

}  // namespace cvg

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

