/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/DiscreteMeasurementValidationUtil.hpp>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/Switch.hpp>
#include <powsybl/iidm/ThreeWindingsTransformer.hpp>
#include <powsybl/iidm/TwoWindingsTransformer.hpp>
#include <powsybl/iidm/extensions/iidm/DiscreteMeasurements.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

#include <powsybl/stdcxx/format.hpp>
#include <powsybl/stdcxx/instanceof.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

namespace DiscreteMeasurementValidationUtil {

std::string checkId(const std::string& id, const DiscreteMeasurements& discreteMeasurements) {
    return checkId(id, false, discreteMeasurements);
}

std::string checkId(const std::string& id, bool idUnicity, const DiscreteMeasurements& discreteMeasurements) {
    std::string finalId = id;
    if (!id.empty() && discreteMeasurements.getDiscreteMeasurement(id)) {
        if(idUnicity) {
            finalId = Identifiables::getUniqueId(id, [&discreteMeasurements](const std::string& id) {
                return static_cast<bool>(discreteMeasurements.getDiscreteMeasurement(id)) ;
            });
            logging::Logger& logger = logging::LoggerFactory::getLogger<DiscreteMeasurements>();
            logger.warn(stdcxx::format("Ensure ID %1% unicity: %2%", id, finalId));
        } else {
            throw PowsyblException(stdcxx::format("There is already a discrete measurement with ID %1%", id));
        }
    }
    return finalId;
}

void checkTapChanger(const stdcxx::optional<DiscreteMeasurement::TapChanger>& tapChanger, const DiscreteMeasurement::Type& type, const Identifiable& identifiable) {
    if (!tapChanger && type == DiscreteMeasurement::Type::TAP_POSITION) {
        throw PowsyblException("The measured tap changer must be specified");
    }
    if (tapChanger) {
        if (!(stdcxx::isInstanceOf<TwoWindingsTransformer>(identifiable) || stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable))) {
            throw PowsyblException("A tap changer is specified when the measured equipment is not a tap changer");
        }
        if (stdcxx::isInstanceOf<TwoWindingsTransformer>(identifiable)
            && tapChanger != DiscreteMeasurement::TapChanger::RATIO_TAP_CHANGER
            && tapChanger != DiscreteMeasurement::TapChanger::PHASE_TAP_CHANGER) {
            throw PowsyblException("A non existent tap changer place has been specified for a two windings transformer's tap changer");
        }
        if (stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable)
            && (tapChanger == DiscreteMeasurement::TapChanger::RATIO_TAP_CHANGER
            || tapChanger == DiscreteMeasurement::TapChanger::PHASE_TAP_CHANGER)) {
            throw PowsyblException("Side is not specified for the measured tap changer of a three windings transformer");
        }
    }
}

void checkType(const DiscreteMeasurement::Type& type, const Identifiable& identifiable) {
    if (type == DiscreteMeasurement::Type::SWITCH_POSITION && !stdcxx::isInstanceOf<Switch>(identifiable)) {
        throw PowsyblException("SWITCH_POSITION discrete not linked to a switch");
    }
    if (type == DiscreteMeasurement::Type::TAP_POSITION && !(stdcxx::isInstanceOf<TwoWindingsTransformer>(identifiable) || stdcxx::isInstanceOf<ThreeWindingsTransformer>(identifiable))) {
        throw PowsyblException("TAP_POSITION discrete not linked to a transformer");
    }
}

void checkValue(const boost::any& value, bool valid) {
    if (value.empty() && valid) {
        throw PowsyblException("A valid discrete measurement can not have an undefined value");
    }
}

DiscreteMeasurement::ValueType getValueType(const boost::any& value) {
    const std::type_info& info = value.type();
    if (info == typeid(std::string)) {
        return DiscreteMeasurement::ValueType::STRING;
    }
    if (info == typeid(int)) {
        return DiscreteMeasurement::ValueType::INT;
    }
    if (info == typeid(bool)) {
        return DiscreteMeasurement::ValueType::BOOLEAN;
    }
    throw PowsyblException(stdcxx::format("Unsupported value type for discrete measurement: %1%", info.name()));
}

}  // namespace DiscreteMeasurementValidationUtil

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
