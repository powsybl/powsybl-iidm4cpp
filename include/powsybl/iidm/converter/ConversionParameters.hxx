/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HXX
#define POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HXX

#include <powsybl/iidm/converter/ConversionParameters.hpp>

#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

template <typename T>
T ConversionParameters::readParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return parameters.get<T>(name);
        }
    }
    return configuredParameter.getDefaultValue<T>();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_CONVERSIONPARAMETERS_HXX
