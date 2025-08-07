/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATE_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATE_HPP

#include <powsybl/stdcxx/math.hpp>

#include <string>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class Coordinate {
public:
    Coordinate() = default;
    Coordinate(double latitude, double longitude);
    Coordinate(const Coordinate& coord) = default;
    Coordinate(Coordinate&&) noexcept = default;
    ~Coordinate() noexcept = default;
    Coordinate& operator=(const Coordinate&) = default;
    Coordinate& operator=(Coordinate&&) noexcept = default;

    double getLatitude() const;
    double getLongitude() const;

    bool operator==(const Coordinate& coord) const;
    bool operator!=(const Coordinate& coord) const;
    


    std::string toString() const;

private:
    double m_latitude = stdcxx::nan();
    double m_longitude = stdcxx::nan();
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_COORDINATE_HPP
