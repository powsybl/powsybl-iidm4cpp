/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUE_HPP

#include <powsybl/iidm/extensions/iidm/WindingConnectionType.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class LegFortescue {
public:
    LegFortescue(double rz, double xz, bool freeFluxes, WindingConnectionType connectionType, double groundingR, double groundingX);

    bool isFreeFluxes() const;
    double getRz() const;
    double getXz() const;
    double getGroundingR() const;
    double getGroundingX() const;
    WindingConnectionType getConnectionType() const;

    LegFortescue& setRz(double rz);
    LegFortescue& setXz(double xz);
    LegFortescue& setGroundingR(double groundingR);
    LegFortescue& setGroundingX(double groundingX);
    LegFortescue& setConnectionType(WindingConnectionType connectionType);
    LegFortescue& setFreeFluxes(bool freeFluxes);

private:

    double m_rz;
    double m_xz;
    bool m_freeFluxes;
    WindingConnectionType m_connectionType;
    double m_groundingR;
    double m_groundingX;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LEGFORTESCUE_HPP

