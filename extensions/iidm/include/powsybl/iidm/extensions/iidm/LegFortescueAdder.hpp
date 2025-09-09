/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUEADDER_HPP

#include <powsybl/iidm/extensions/iidm/WindingConnectionType.hpp>

#include <powsybl/iidm/extensions/iidm/FortescueConstants.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerFortescue.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerFortescueAdder;
class LegFortescue;

class LegFortescueAdder {
public:

    ThreeWindingsTransformerFortescueAdder& add();

    LegFortescueAdder& withRz(double rz);
    LegFortescueAdder& withXz(double xz);
    LegFortescueAdder& withFreeFluxes(bool freeFluxes);
    LegFortescueAdder& withGroundingR(double groundingR);
    LegFortescueAdder& withGroundingX(double groundingX);
    LegFortescueAdder& withConnectionType(WindingConnectionType connectionType);

private:
    LegFortescueAdder(ThreeWindingsTransformerFortescueAdder& parent, unsigned long legNumber, WindingConnectionType connectionType);
    friend class ThreeWindingsTransformerFortescueAdder;

    LegFortescue build() const;

    ThreeWindingsTransformerFortescueAdder& m_parent;

    unsigned long m_legNumber;
    double m_rz = stdcxx::nan();
    double m_xz = stdcxx::nan();
    bool m_freeFluxes = DEFAULT_FREE_FLUXES;
    WindingConnectionType m_connectionType;
    double m_groundingR = DEFAULT_GROUNDING_R;
    double m_groundingX = DEFAULT_GROUNDING_X;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUEADDER_HPP

