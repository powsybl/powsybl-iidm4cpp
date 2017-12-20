/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_MULTISTATEOBJECT_HPP
#define POWSYBL_IIDM_MULTISTATEOBJECT_HPP

namespace powsybl {

namespace iidm {

class StateManager;

class MultiStateObject {
public:
    virtual ~MultiStateObject() = default;

    virtual unsigned long getStateIndex() const = 0;

    virtual StateManager& getStateManager() const = 0;
};

}

}

#endif  // POWSYBL_IIDM_MULTISTATEOBJECT_HPP
