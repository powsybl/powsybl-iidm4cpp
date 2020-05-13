/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_PARAMETER_HXX
#define POWSYBL_IIDM_CONVERTER_PARAMETER_HXX

#include <powsybl/iidm/converter/Parameter.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

template <typename T>
T Parameter::getDefaultValue() const {
    return boost::any_cast<T>(m_defaultValue);
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_PARAMETER_HXX
