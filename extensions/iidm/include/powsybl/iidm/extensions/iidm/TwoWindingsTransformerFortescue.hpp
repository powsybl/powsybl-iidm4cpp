/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUE_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/extensions/iidm/WindingConnectionType.hpp>

namespace powsybl {

namespace iidm {

class TwoWindingsTransformer;

namespace extensions {

namespace iidm {

class TwoWindingsTransformerFortescue : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;


    TwoWindingsTransformerFortescue(TwoWindingsTransformer& twt, double rz, double xz, bool freeFluxes,
                                    WindingConnectionType connectionType1, WindingConnectionType connectionType2,
                                    double groundingR1, double groundingX1, double groundingR2, double groundingX2 );


    bool isFreeFluxes() const;
    double getRz() const;
    double getXz() const;
    double getGroundingR1() const;
    double getGroundingX1() const;
    double getGroundingR2() const;
    double getGroundingX2() const;
    WindingConnectionType getConnectionType1() const;
    WindingConnectionType getConnectionType2() const;

    TwoWindingsTransformerFortescue& setFreeFluxes(bool freeFluxes);
    TwoWindingsTransformerFortescue& setRz(double rz);
    TwoWindingsTransformerFortescue& setXz(double xz);
    TwoWindingsTransformerFortescue& setGroundingR1(double groundingR);
    TwoWindingsTransformerFortescue& setGroundingX1(double groundingX);
    TwoWindingsTransformerFortescue& setGroundingR2(double groundingR);
    TwoWindingsTransformerFortescue& setGroundingX2(double groundingX);
    TwoWindingsTransformerFortescue& setConnectionType1(WindingConnectionType connectionType);
    TwoWindingsTransformerFortescue& setConnectionType2(WindingConnectionType connectionType);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    double m_rz;
    double m_xz;
    bool m_freeFluxes;
    WindingConnectionType m_connectionType1;
    WindingConnectionType m_connectionType2;
    double m_groundingR1;
    double m_groundingX1;
    double m_groundingR2;
    double m_groundingX2;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUE_HPP

