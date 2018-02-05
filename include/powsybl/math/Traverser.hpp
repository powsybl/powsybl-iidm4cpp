/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TRAVERSER_HPP
#define POWSYBL_IIDM_TRAVERSER_HPP

#include <functional>

#include <powsybl/math/TraverseResult.hpp>

namespace powsybl {

namespace math {

typedef std::function<TraverseResult(unsigned long v1, unsigned long e, unsigned long v2)> Traverser;

}

}

#endif  // POWSYBL_IIDM_TRAVERSER_HPP
