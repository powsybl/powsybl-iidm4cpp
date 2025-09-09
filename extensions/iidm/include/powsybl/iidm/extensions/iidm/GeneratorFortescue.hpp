/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUE_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Generator;

namespace extensions {

namespace iidm {

class GeneratorFortescue : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;


    GeneratorFortescue(Generator& generator, bool grounded, double rz, double xz, double rn, double xn, double groundingR, double groundingX);

    bool isGrounded() const;
    GeneratorFortescue& setGrounded(bool grounded);

    double getRz() const;
    double getXz() const;
    double getRn() const;
    double getXn() const;
    double getGroundingR() const;
    double getGroundingX() const;

    GeneratorFortescue& setRz(double rz);
    GeneratorFortescue& setXz(double xz);
    GeneratorFortescue& setRn(double rn);
    GeneratorFortescue& setXn(double xn);
    GeneratorFortescue& setGroundingR(double groundingR);
    GeneratorFortescue& setGroundingX(double groundingX);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    bool m_grounded;
    double m_rz;
    double m_xz;
    double m_rn;
    double m_xn;
    double m_groundingR;
    double m_groundingX;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_GENERATORFORTESCUE_HPP

