/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimatedAdder.hpp>

#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/ThreeWindingsTransformerToBeEstimated.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

ThreeWindingsTransformerToBeEstimatedAdder::ThreeWindingsTransformerToBeEstimatedAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> ThreeWindingsTransformerToBeEstimatedAdder::createExtension(Extendable& extendable) const {
    if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(extendable)) {
        return std::unique_ptr<ThreeWindingsTransformerToBeEstimated>(new ThreeWindingsTransformerToBeEstimated(dynamic_cast<ThreeWindingsTransformer&>(extendable), m_rtc1Status, m_rtc2Status, m_rtc3Status, m_ptc1Status, m_ptc2Status, m_ptc3Status));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<ThreeWindingsTransformer>()));
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withPhaseTapChanger1Status(bool toBeEstimated) {
    m_ptc1Status = toBeEstimated;

    return *this;
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withPhaseTapChanger2Status(bool toBeEstimated) {
    m_ptc2Status = toBeEstimated;

    return *this;
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withPhaseTapChanger3Status(bool toBeEstimated) {
    m_ptc3Status = toBeEstimated;

    return *this;
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withRatioTapChanger1Status(bool toBeEstimated) {
    m_rtc1Status = toBeEstimated;

    return *this;
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withRatioTapChanger2Status(bool toBeEstimated) {
    m_rtc2Status = toBeEstimated;

    return *this;
}

ThreeWindingsTransformerToBeEstimatedAdder& ThreeWindingsTransformerToBeEstimatedAdder::withRatioTapChanger3Status(bool toBeEstimated) {
    m_rtc3Status = toBeEstimated;

    return *this;
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
