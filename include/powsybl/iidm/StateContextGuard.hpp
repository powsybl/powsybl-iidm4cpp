/**
 * Copyright (c) 2018, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_STATECONTEXTGUARD_HPP
#define POWSYBL_IIDM_STATECONTEXTGUARD_HPP

namespace powsybl {

namespace iidm {

class StateContext;

/**
 * A scope guard implementation based on RAII idiom to restore the state index
 */
class StateContextGuard {
public:
    explicit StateContextGuard(StateContext& context);

    ~StateContextGuard();

private: // Non copyable
    StateContextGuard(const StateContextGuard& guard) = delete;

    StateContextGuard& operator=(const StateContextGuard& guard) = delete;

private:
    StateContext& m_context;

    unsigned long m_stateIndex;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_STATECONTEXTGUARD_HPP
