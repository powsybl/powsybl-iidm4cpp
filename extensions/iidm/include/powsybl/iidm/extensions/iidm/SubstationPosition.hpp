/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITION_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITION_HPP

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/iidm/Substation.hpp>
#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>

namespace powsybl {

namespace iidm {

class Substation;

namespace extensions {

namespace iidm {

class SubstationPosition : public Extension {
public: // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    SubstationPosition(Substation& substation, const Coordinate& coords);

    const Coordinate& getCoordinates() const;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    Coordinate m_coordinates;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_SUBSTATIONPOSITION_HPP
