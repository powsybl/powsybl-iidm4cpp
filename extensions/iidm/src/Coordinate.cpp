/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/Coordinate.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

Coordinate::Coordinate(double latitude, double longitude) {
    m_latitude = latitude;
    m_longitude = longitude;
}

double Coordinate::getLatitude() const {
    return m_latitude;
}
double Coordinate::getLongitude() const {
    return m_longitude;
}

bool Coordinate::operator==(const Coordinate& coord) const {
    return m_latitude == coord.m_latitude && m_longitude == coord.m_longitude;
}
bool Coordinate::operator!=(const Coordinate& coord) const {
    return m_latitude != coord.m_latitude || m_longitude != coord.m_longitude;
}

std::string Coordinate::toString() const {
    return stdcxx::format("Coordinate(lat=%1%, lon=%2%)", m_latitude, m_longitude);
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
