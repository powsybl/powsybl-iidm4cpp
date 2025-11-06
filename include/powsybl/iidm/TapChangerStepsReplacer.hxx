/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HXX
#define POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HXX

#include <powsybl/iidm/TapChangerStepsReplacer.hpp>

namespace powsybl {

namespace iidm {

template<typename TC, typename S, typename TCStepAdder>
TapChangerStepsReplacer<TC, S, TCStepAdder>::TapChangerStepsReplacer(TC& tapChanger) :
    m_tapChangerParent(tapChanger) {
}

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HXX
