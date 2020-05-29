/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/entsoe/EntsoeArea.hpp>

#include <powsybl/iidm/Substation.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

EntsoeArea::EntsoeArea(Substation& substation, const EntsoeGeographicalCode& code) :
    Extension(substation),
    m_code(code) {

}

void EntsoeArea::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Substation>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Substation>()));
    }
}

const EntsoeGeographicalCode& EntsoeArea::getCode() const {
    return m_code;
}

const std::string& EntsoeArea::getName() const {
    static std::string s_name = "entsoeArea";
    return s_name;
}

const std::type_index& EntsoeArea::getType() const {
    static std::type_index s_type = typeid(EntsoeArea);
    return s_type;
}

EntsoeArea& EntsoeArea::setCode(const EntsoeGeographicalCode& code) {
    m_code = code;
    return *this;
}

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

