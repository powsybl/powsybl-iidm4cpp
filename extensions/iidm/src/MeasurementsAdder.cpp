/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/extensions/iidm/MeasurementsAdder.hpp>

#include <memory>

#include <powsybl/iidm/Identifiable.hpp>
#include <powsybl/iidm/extensions/iidm/Measurements.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

MeasurementsAdder::MeasurementsAdder(Extendable& extendable) :
    ExtensionAdder(extendable) {
}

std::unique_ptr<Extension> MeasurementsAdder::createExtension(Extendable& extendable) {
    if (stdcxx::isInstanceOf<Identifiable>(extendable)) {
        return std::unique_ptr<Measurements>(new Measurements(dynamic_cast<Identifiable&>(extendable)));
    }
    throw AssertionError(stdcxx::format("Unexpected extendable type: %1% (%2% expected)", stdcxx::demangle(extendable), stdcxx::demangle<Identifiable>()));
}

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl
