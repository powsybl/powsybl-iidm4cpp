/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUE_HPP

#include <powsybl/iidm/Extension.hpp>

namespace powsybl {

namespace iidm {

class Line;

namespace extensions {

namespace iidm {

class LineFortescue : public Extension {
public:  // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;


    LineFortescue(Line& line, double rz, double xz);

    double getRz() const;
    double getXz() const;

    LineFortescue& setRz(double rz);
    LineFortescue& setXz(double xz);

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

    double m_rz;
    double m_xz;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LINEFORTESCUE_HPP

