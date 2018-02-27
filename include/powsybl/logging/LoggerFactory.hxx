/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOGGERFACTORY_HXX
#define POWSYBL_IIDM_LOGGERFACTORY_HXX

#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace logging {

template <typename T>
Logger& LoggerFactory::getLogger() {
    return getLogger(typeid(T).name());
}

}


}

#endif  // POWSYBL_IIDM_LOGGERFACTORY_HXX
