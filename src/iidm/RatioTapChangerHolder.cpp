/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/RatioTapChangerHolder.hpp>

namespace powsybl {

namespace iidm {

stdcxx::CReference<RatioTapChanger> RatioTapChangerHolder::getOptionalRatioTapChanger() const {
    return getRatioTapChanger();
}

stdcxx::Reference<RatioTapChanger> RatioTapChangerHolder::getOptionalRatioTapChanger() {
    return getRatioTapChanger();
}

bool RatioTapChangerHolder::hasRatioTapChanger() const {
    return getRatioTapChanger();
}

}  // namespace iidm

}  // namespace powsybl
