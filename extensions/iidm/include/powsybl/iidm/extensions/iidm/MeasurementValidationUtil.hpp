/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTVALIDATIONUTIL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTVALIDATIONUTIL_HPP

#include <string>

#include <powsybl/iidm/extensions/iidm/Measurement.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Connectable;

namespace extensions {

namespace iidm {

class Measurements;

namespace MeasurementValidationUtil {

void checkId(const std::string& id, const Measurements& measurements);

void checkSide(const Measurement::Type& type, const stdcxx::optional<Measurement::Side>& side, const Connectable& connectable);

void checkValue(double value, bool valid);

}  // namespace MeasurementValidationUtil

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_MEASUREMENTVALIDATIONUTIL_HPP
