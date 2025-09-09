/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUEADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/iidm/FortescueConstants.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class GeneratorFortescueAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit GeneratorFortescueAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~GeneratorFortescueAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    GeneratorFortescueAdder(const GeneratorFortescueAdder&) = default;

    /**
     * Move constructor
     */
    GeneratorFortescueAdder(GeneratorFortescueAdder&&) = default;

    /**
     * Copy assignment operator
     */
    GeneratorFortescueAdder& operator=(const GeneratorFortescueAdder&) = delete;

    /**
     * Move assignment operator
     */
    GeneratorFortescueAdder& operator=(GeneratorFortescueAdder&&) = delete;


    GeneratorFortescueAdder& withGrounded(bool grounded);
    GeneratorFortescueAdder& withRz(double rz);
    GeneratorFortescueAdder& withXz(double xz);
    GeneratorFortescueAdder& withRn(double rn);
    GeneratorFortescueAdder& withXn(double xn);
    GeneratorFortescueAdder& withGroundingR(double groundingR);
    GeneratorFortescueAdder& withGroundingX(double groundingX);

protected:
    /**
     * Creates the GeneratorFortescue extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:

    bool m_grounded = DEFAULT_GROUNDED;
    double m_rz = stdcxx::nan();
    double m_xz = stdcxx::nan();
    double m_rn = stdcxx::nan();
    double m_xn = stdcxx::nan();
    double m_groundingR = DEFAULT_GROUNDING_R;
    double m_groundingX = DEFAULT_GROUNDING_X;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUEADDER_HPP
