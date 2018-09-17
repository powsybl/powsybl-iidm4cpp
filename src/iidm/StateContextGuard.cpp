/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/StateContextGuard.hpp>

#include <powsybl/iidm/StateContext.hpp>

namespace powsybl {

namespace iidm {

StateContextGuard::StateContextGuard(StateContext& context) :
    m_context(context),
    m_stateIndex(context.getStateIndex()) {
}

StateContextGuard::~StateContextGuard() noexcept {
    m_context.setStateIndex(m_stateIndex);
}

}  // namespace iidm

}  // namespace powsybl
