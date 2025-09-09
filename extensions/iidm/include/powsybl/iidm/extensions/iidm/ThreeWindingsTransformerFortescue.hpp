/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUE_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/extensions/iidm/LegFortescue.hpp>
#include <vector>

namespace powsybl {

namespace iidm {

class ThreeWindingsTransformer;

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerFortescue : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;


    ThreeWindingsTransformerFortescue(ThreeWindingsTransformer& twt, LegFortescue&& leg1,  LegFortescue&& leg2, LegFortescue&& leg3);

    const LegFortescue& getLeg1() const;
    LegFortescue& getLeg1();
    const LegFortescue& getLeg2() const;
    LegFortescue& getLeg2();
    const LegFortescue& getLeg3() const;
    LegFortescue& getLeg3();

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    std::vector<LegFortescue> m_legs;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUE_HPP

