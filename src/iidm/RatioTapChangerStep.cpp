/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChangerStep.hpp>

namespace powsybl {

namespace iidm {

RatioTapChangerStep::RatioTapChangerStep(unsigned long position, double rho, double r, double x, double g, double b) :
    TapChangerStep(position, rho, r, x, g, b) {
}

}  // namespace iidm

}  // namespace powsybl
