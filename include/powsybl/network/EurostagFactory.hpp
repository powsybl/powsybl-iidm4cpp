/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EUROSTAGFACTORY_HPP
#define POWSYBL_IIDM_EUROSTAGFACTORY_HPP

#include <powsybl/iidm/Network.hpp>

namespace powsybl {

namespace network {

class EurostagFactory {
public:
    static iidm::Network createTutorial1Network();

public:
    EurostagFactory() = delete;
};

}  // namespace network

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EUROSTAGFACTORY_HPP
