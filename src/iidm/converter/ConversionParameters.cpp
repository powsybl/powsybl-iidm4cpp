/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ConversionParameters.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>

#include <powsybl/stdcxx/Properties.hpp>

#include "Parameter.hpp"

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

double ConversionParameters::readDoubleParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return std::stod(parameters.get(name));
        }
    }
    return configuredParameter.getDoubleDefaultValue();
}

std::vector<std::string> ConversionParameters::readStringListParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            std::vector<std::string> values;
            return boost::algorithm::split(values, parameters.get(name), boost::is_any_of(",:"));
        }
    }
    return configuredParameter.getStringListDefaultValue();
}

const std::string& ConversionParameters::readStringParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return parameters.get(name);
        }
    }
    return configuredParameter.getStringDefaultValue();
}

int ConversionParameters::readIntParameter(const stdcxx::Properties& parameters, const Parameter& configuredParameter) {
    for (const std::string& name : configuredParameter.getNames()) {
        if (parameters.contains(name)) {
            return std::stoi(parameters.get(name));
        }
    }
    return configuredParameter.getIntDefaultValue();
}

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl
