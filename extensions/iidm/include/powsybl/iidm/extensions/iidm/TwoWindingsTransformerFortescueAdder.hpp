/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/FortescueConstants.hpp>
#include <powsybl/iidm/extensions/iidm/WindingConnectionType.hpp>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class TwoWindingsTransformerFortescueAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit TwoWindingsTransformerFortescueAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~TwoWindingsTransformerFortescueAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    TwoWindingsTransformerFortescueAdder(const TwoWindingsTransformerFortescueAdder&) = default;

    /**
     * Move constructor
     */
    TwoWindingsTransformerFortescueAdder(TwoWindingsTransformerFortescueAdder&&) = default;

    /**
     * Copy assignment operator
     */
    TwoWindingsTransformerFortescueAdder& operator=(const TwoWindingsTransformerFortescueAdder&) = delete;

    /**
     * Move assignment operator
     */
    TwoWindingsTransformerFortescueAdder& operator=(TwoWindingsTransformerFortescueAdder&&) = delete;

    TwoWindingsTransformerFortescueAdder& withRz(double rz);
    TwoWindingsTransformerFortescueAdder& withXz(double xz);
    TwoWindingsTransformerFortescueAdder& withFreeFluxes(bool freeFluxes);
    TwoWindingsTransformerFortescueAdder& withConnectionType1(WindingConnectionType connectionType1);
    TwoWindingsTransformerFortescueAdder& withConnectionType2(WindingConnectionType connectionType2);
    TwoWindingsTransformerFortescueAdder& withGroundingR1(double groundingR1);
    TwoWindingsTransformerFortescueAdder& withGroundingX1(double groundingX1);
    TwoWindingsTransformerFortescueAdder& withGroundingR2(double groundingR2);
    TwoWindingsTransformerFortescueAdder& withGroundingX2(double groundingX2);

protected:
    /**
     * Creates the LineFortescue extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:

    double m_rz = stdcxx::nan();
    double m_xz = stdcxx::nan();
    bool m_freeFluxes = DEFAULT_FREE_FLUXES;
    WindingConnectionType m_connectionType1 = DEFAULT_LEG1_CONNECTION_TYPE;
    WindingConnectionType m_connectionType2 = DEFAULT_LEG2_CONNECTION_TYPE;
    double m_groundingR1 = DEFAULT_GROUNDING_R;
    double m_groundingX1 = DEFAULT_GROUNDING_X;
    double m_groundingR2 = DEFAULT_GROUNDING_R;
    double m_groundingX2 = DEFAULT_GROUNDING_X;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_TWOWINDINGSTRANSFORMERFORTESCUEADDER_HPP
