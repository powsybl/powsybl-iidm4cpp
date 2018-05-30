/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATECONTEXT_HPP
#define POWSYBL_IIDM_STATECONTEXT_HPP

namespace powsybl {

namespace iidm {

class StateManager;

class StateContext {
public:
    virtual ~StateContext() = default;

protected:
    virtual unsigned long getStateIndex() const = 0;

    virtual void resetIfStateIndexIs(unsigned long index) = 0;

    virtual void setStateIndex(unsigned long index) = 0;

private:
    friend class StateContextGuard;

    friend class StateManager;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATECONTEXT_HPP
