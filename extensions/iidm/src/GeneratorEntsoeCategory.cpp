/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/GeneratorEntsoeCategory.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/Generator.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

GeneratorEntsoeCategory::GeneratorEntsoeCategory(Generator& generator, unsigned long code) :
    Extension(generator),
    m_code(checkCode(code)) {
}

void GeneratorEntsoeCategory::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<Generator>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<Generator>()));
    }
}

unsigned long GeneratorEntsoeCategory::checkCode(unsigned long code) {
    if (code < 1) {
        throw PowsyblException(stdcxx::format("Bad generator ENTSO-E code %1%", code));
    }
    return code;
}

unsigned long GeneratorEntsoeCategory::getCode() const {
    return m_code;
}

const std::string& GeneratorEntsoeCategory::getName() const {
    static std::string s_name = "entsoeCategory";
    return s_name;
}

const std::type_index& GeneratorEntsoeCategory::getType() const {
    static std::type_index s_type = typeid(GeneratorEntsoeCategory);
    return s_type;
}

GeneratorEntsoeCategory& GeneratorEntsoeCategory::setCode(unsigned long code) {
    m_code = checkCode(code);
    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

