/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class DiscreteMeasurementsAdder : public ExtensionAdder {
public:
    explicit DiscreteMeasurementsAdder(Extendable& extendable);

private:  // ExtensionAdder
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;
};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_DISCRETEMEASUREMENTSADDER_HPP
