/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/LegFortescueAdder.hpp>

#include <powsybl/stdcxx/optional.hpp>


namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class ThreeWindingsTransformerFortescueAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit ThreeWindingsTransformerFortescueAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~ThreeWindingsTransformerFortescueAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    ThreeWindingsTransformerFortescueAdder(const ThreeWindingsTransformerFortescueAdder&) = default;

    /**
     * Move constructor
     */
    ThreeWindingsTransformerFortescueAdder(ThreeWindingsTransformerFortescueAdder&&) = default;

    /**
     * Copy assignment operator
     */
    ThreeWindingsTransformerFortescueAdder& operator=(const ThreeWindingsTransformerFortescueAdder&) = delete;

    /**
     * Move assignment operator
     */
    ThreeWindingsTransformerFortescueAdder& operator=(ThreeWindingsTransformerFortescueAdder&&) = delete;

    LegFortescueAdder newLeg1();
    LegFortescueAdder newLeg2();
    LegFortescueAdder newLeg3();

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

    void setLegAdder(const LegFortescueAdder& legAdder, unsigned long legNumber);
    friend class LegFortescueAdder;

    stdcxx::optional<LegFortescueAdder> m_LegAdder1;
    stdcxx::optional<LegFortescueAdder> m_LegAdder2;
    stdcxx::optional<LegFortescueAdder> m_LegAdder3;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_THREEWINDINGSTRANSFORMERFORTESCUEADDER_HPP
