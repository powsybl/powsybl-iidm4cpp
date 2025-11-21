/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTVALIDATIONUTIL_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTVALIDATIONUTIL_HPP

#include <string>

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurement.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class Identifiable;

namespace extensions {

namespace iidm {

namespace DiscreteMeasurementValidationUtil {

std::string checkId(const std::string& id, const DiscreteMeasurements& discreteMeasurements);
std::string checkId(const std::string& id, bool idUnicity, const DiscreteMeasurements& discreteMeasurements);

void checkTapChanger(const stdcxx::optional<DiscreteMeasurement::TapChanger>& tapChanger, const DiscreteMeasurement::Type& type, const Identifiable& identifiable);

void checkType(const DiscreteMeasurement::Type& type, const Identifiable& identifiable);

void checkValue(const boost::any& value, bool valid);

DiscreteMeasurement::ValueType getValueType(const boost::any& value);

}  // namespace DiscreteMeasurementValidationUtil

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTVALIDATIONUTIL_HPP
