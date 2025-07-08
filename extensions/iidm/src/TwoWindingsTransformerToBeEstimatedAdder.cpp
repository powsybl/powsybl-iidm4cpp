/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimatedAdder.hpp>

#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/TwoWindingsTransformerToBeEstimated.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

TwoWindingsTransformerToBeEstimatedAdder::TwoWindingsTransformerToBeEstimatedAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> TwoWindingsTransformerToBeEstimatedAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<TwoWindingsTransformer>(extendable)) {
        return std::unique_ptr<TwoWindingsTransformerToBeEstimated>(new TwoWindingsTransformerToBeEstimated(dynamic_cast<TwoWindingsTransformer&>(extendable), m_rtcStatus, m_ptcStatus));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<TwoWindingsTransformer>()));
}

TwoWindingsTransformerToBeEstimatedAdder& TwoWindingsTransformerToBeEstimatedAdder::withPhaseTapChangerStatus(bool toBeEstimated) {
    m_ptcStatus = toBeEstimated;

    return *this;
}

TwoWindingsTransformerToBeEstimatedAdder& TwoWindingsTransformerToBeEstimatedAdder::withRatioTapChangerStatus(bool toBeEstimated) {
    m_rtcStatus = toBeEstimated;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
