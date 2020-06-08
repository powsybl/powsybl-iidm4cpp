/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/ConversionParameters.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include <powsybl/iidm/converter/Parameter.hpp>
#include <powsybl/stdcxx/Properties.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

bool ConversionParameters::readBooleanParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return boost::iequals(parameters.get(name), "true");
        }
    }
    return configuredParameter.getBooleanDefaultValue();
}

std::vector<std::string> ConversionParameters::readStringListParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            std::vector<std::string> values;
            boost::algorithm::split(values, parameters.get(name), boost::is_any_of(",:"));
            return values;
        }
    }
    return configuredParameter.getStringListDefaultValue();
}

std::string ConversionParameters::readStringParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return parameters.get(name);
        }
    }
    return configuredParameter.getStringDefaultValue();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
