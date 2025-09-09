/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_FORTESCUECONSTANTS_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_FORTESCUECONSTANTS_HPP

#include <powsybl/iidm/extensions/iidm/WindingConnectionType.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

static const bool DEFAULT_GROUNDED = false;
static const double DEFAULT_GROUNDING_R = 0.0;
static const double DEFAULT_GROUNDING_X = 0.0;
static const bool DEFAULT_FREE_FLUXES = true;
static const WindingConnectionType DEFAULT_LEG1_CONNECTION_TYPE = WindingConnectionType::DELTA;
static const WindingConnectionType DEFAULT_LEG2_CONNECTION_TYPE = WindingConnectionType::Y_GROUNDED;
static const WindingConnectionType DEFAULT_LEG3_CONNECTION_TYPE = WindingConnectionType::DELTA;

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_FORTESCUECONSTANTS_HPP