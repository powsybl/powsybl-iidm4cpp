/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEPADDER_HXX
#define POWSYBL_IIDM_TAPCHANGERSTEPADDER_HXX

#include <powsybl/iidm/TapChangerStepAdder.hpp>

namespace powsybl {

namespace iidm {

template<typename TCStepAdder, typename TCAdder>
TapChangerStepAdder<TCStepAdder,TCAdder>::TapChangerStepAdder(TCAdder& parent) :
    m_parent(parent) {
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGERSTEPADDER_HXX
