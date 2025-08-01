/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/MeasurementValidationUtil.hpp>

#include <powsybl/PowsyblException.hpp>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>
#include <powsybl/iidm/util/Identifiables.hpp>
#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

namespace MeasurementValidationUtil {

std::string checkId(const std::string& id, const Measurements& measurements) {
    return checkId(id, false, measurements);
}

std::string checkId(const std::string& id, bool idUnicity, const Measurements& measurements) {
    std::string finalId = id;
    if (!id.empty() && measurements.getMeasurement(id)) {
        if(idUnicity) {
            finalId = Identifiables::getUniqueId(id, [&measurements](const std::string& id) {
                return static_cast<bool>(measurements.getMeasurement(id)) ;
            });
            logging::Logger& logger = logging::LoggerFactory::getLogger<Measurements>();
            logger.warn(stdcxx::format("Ensure ID %1% unicity: %2%", id, finalId));
        } else {
            throw PowsyblException(stdcxx::format("There is already a measurement with ID %1%", id));
        }
    }
    return finalId;
}

void checkSide(const Measurement::Type& type, const stdcxx::optional<Measurement::Side>& side, const Connectable& connectable) {
    if (side && stdcxx::isInstanceOf<Injection>(connectable)) {
        throw PowsyblException("Inconsistent side for measurement of injection");
    }
    if (!side && type != Measurement::Type::OTHER && !stdcxx::isInstanceOf<Injection>(connectable)) {
        throw PowsyblException("Inconsistent null side for measurement of branch or three windings transformer");
    }
}

void checkValue(double value, bool valid) {
    if (std::isnan(value) && valid) {
        throw PowsyblException("Valid measurement can not have an undefined value");
    }
}

}  // namespace MeasurementValidationUtil

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
