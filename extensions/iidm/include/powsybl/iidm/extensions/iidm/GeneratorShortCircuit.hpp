/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUIT_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUIT_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace iidm {

class GeneratorShortCircuit : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    /**
     * Get the direct-axis subtransient reactance (also known as X''d)
     */
    double getDirectSubtransX() const;

    /**
     * Get the direct-axis transient reactance (also known as X'd)
     */
    double getDirectTransX() const;

    /**
     * Get the step-up transformer reactance if the generator has a non-modeled step-up transformer.
     */
    double getStepUpTransformerX() const;

    /**
     * Set the direct-axis subtransient reactance (also known as X''d)
     */
    GeneratorShortCircuit& setDirectSubtransX(double directSubtransX);

    /**
     * Set the direct-axis transient reactance (also known as X'd)
     */
    GeneratorShortCircuit& setDirectTransX(double directTransX);

    /**
     * Set the step-up transformer reactance
     */
    GeneratorShortCircuit& setStepUpTransformerX(double setUpTransformerX);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    GeneratorShortCircuit(Generator& generator, double directSubtransX, double directTransX, double stepUpTransformerX);

    friend class GeneratorShortCircuitAdder;

private:
    double m_directSubtransX;

    double m_directTransX;

    double m_stepUpTransformerX;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORSHORTCIRCUIT_HPP
