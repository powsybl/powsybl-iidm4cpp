/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescue.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerFortescue::ThreeWindingsTransformerFortescue(ThreeWindingsTransformer& twt, LegFortescue&& leg1,  LegFortescue&& leg2, LegFortescue&& leg3) :
    Extension(twt) {
    m_legs.emplace_back(std::move(leg1));
    m_legs.emplace_back(std::move(leg2));
    m_legs.emplace_back(std::move(leg3));
}


void ThreeWindingsTransformerFortescue::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<ThreeWindingsTransformer>()));
    }
}

const std::string& ThreeWindingsTransformerFortescue::getName() const {
    static std::string s_name = "threeWindingsTransformerFortescue";
    return s_name;
}

const std::type_index& ThreeWindingsTransformerFortescue::getType() const {
    static std::type_index s_type = typeid(ThreeWindingsTransformerFortescue);
    return s_type;
}

const LegFortescue& ThreeWindingsTransformerFortescue::getLeg1() const {
    return m_legs[0];
}
LegFortescue& ThreeWindingsTransformerFortescue::getLeg1() {
    return m_legs[0];
}
const LegFortescue& ThreeWindingsTransformerFortescue::getLeg2() const {
    return m_legs[1];
}
LegFortescue& ThreeWindingsTransformerFortescue::getLeg2() {
    return m_legs[1];
}
const LegFortescue& ThreeWindingsTransformerFortescue::getLeg3() const {
    return m_legs[2];
}
LegFortescue& ThreeWindingsTransformerFortescue::getLeg3() {
    return m_legs[2];
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

