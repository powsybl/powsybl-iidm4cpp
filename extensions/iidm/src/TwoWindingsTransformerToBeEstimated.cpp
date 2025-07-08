/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimated.hpp>

#include <powsybl/iidm/Enum.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerToBeEstimated::TwoWindingsTransformerToBeEstimated(TwoWindingsTransformer& transformer, bool rtcStatus, bool ptcStatus) :
    Extension(transformer),
    m_ptcStatus(ptcStatus),
    m_rtcStatus(rtcStatus) {
}

void TwoWindingsTransformerToBeEstimated::assertExtendable(const stdcxx::Reference<Extendable>& extendable) const {
    if (extendable && !stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable.get())) {
        throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable.get()), stdcxx::demangle<TwoWindingsTransformer>()));
    }
}

const std::string& TwoWindingsTransformerToBeEstimated::getName() const {
    static std::string s_name = "twoWindingsTransformerToBeEstimated";
    return s_name;
}

const std::type_index& TwoWindingsTransformerToBeEstimated::getType() const {
    static std::type_index s_type = typeid(TwoWindingsTransformerToBeEstimated);
    return s_type;
}

bool TwoWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger() const {
    return m_ptcStatus;
}

TwoWindingsTransformerToBeEstimated& TwoWindingsTransformerToBeEstimated::shouldEstimatePhaseTapChanger(bool toBeEstimated) {
    m_ptcStatus = toBeEstimated;

    return *this;
}

bool TwoWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger() const {
    return m_rtcStatus;
}

TwoWindingsTransformerToBeEstimated& TwoWindingsTransformerToBeEstimated::shouldEstimateRatioTapChanger(bool toBeEstimated) {
    m_rtcStatus = toBeEstimated;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
