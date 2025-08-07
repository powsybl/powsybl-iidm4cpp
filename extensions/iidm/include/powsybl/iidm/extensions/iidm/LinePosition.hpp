/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITION_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITION_HPP

#include <powsybl/iidm/Extension.hpp>

#include <vector>

namespace powsybl {

namespace iidm {

class Line;

namespace extensions {

namespace iidm {

class Coordinate;

class LinePosition : public Extension {
public: // Extension
    const std::string& getName() const override;

    const std::type_index& getType() const override;

public:
    LinePosition(Line& line, std::vector<Coordinate> coords);

    const std::vector<Coordinate>& getCoordinates() const;

private:  // Extension
    void assertExtendable(const stdcxx::Reference<Extendable>& extendable) const override;

private:
    std::vector<Coordinate> m_coordinates;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_LINEPOSITION_HPP
