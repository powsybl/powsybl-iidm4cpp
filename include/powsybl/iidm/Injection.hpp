/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_INJECTION_HPP
#define POWSYBL_IIDM_INJECTION_HPP

#include <powsybl/iidm/Connectable.hpp>

namespace powsybl {

namespace iidm {

class Injection : public Connectable {
public:
    virtual ~Injection() = default;

protected:
    Injection(const std::string& id, const std::string& name, const ConnectableType& connectableType);
};

}

}

#endif  // POWSYBL_IIDM_INJECTION_HPP
