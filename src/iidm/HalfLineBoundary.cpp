/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "HalfLineBoundary.hpp"

#include <powsybl/iidm/Bus.hpp>
#include <powsybl/iidm/HalfLine.hpp>
#include <powsybl/iidm/Terminal.hpp>
#include <powsybl/iidm/TieLine.hpp>
#include <powsybl/iidm/util/SV.hpp>

namespace powsybl {

namespace iidm {

namespace half_line {

Boundary::Boundary(tie_line::HalfLine& halfline, const Branch::Side& side) :
    util::half_line::AbstractHalfLineBoundary(halfline, side) {
}

const Connectable& Boundary::getConnectable() const {
    return getParent().getParent();
}

Connectable& Boundary::getConnectable() {
    return getParent().getParent();
}

}  // namespace half_line

}  // namespace iidm

}  // namespace powsybl
