/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/Boundary.hpp>

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

double Boundary::getAngle(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getAngle() : stdcxx::nan();
}

double Boundary::getV(const stdcxx::CReference<Bus>& bus) {
    return bus ? bus.get().getV() : stdcxx::nan();
}

}  // namespace iidm

}  // namespace powsybl
