/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HPP
#define POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HPP

#include <vector>

#include <powsybl/stdcxx/math.hpp>

namespace powsybl {

namespace iidm {

template<typename TC, typename S, typename TCStepAdder> // TC = TapChanger; S = TapChangerStep
class TapChangerStepsReplacer {
public:
    virtual ~TapChangerStepsReplacer() noexcept = default;

public:
    virtual TCStepAdder beginStep() = 0;

    virtual void replaceSteps() = 0;

protected:
    TapChangerStepsReplacer(TC& tapChanger);

    std::vector<S> m_newSteps;

    TC& m_tapChangerParent;
};

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/TapChangerStepsReplacer.hxx>

#endif  // POWSYBL_IIDM_TAPCHANGERSTEPSREPLACER_HPP
