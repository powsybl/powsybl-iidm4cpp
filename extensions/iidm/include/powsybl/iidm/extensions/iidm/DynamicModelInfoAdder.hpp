/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_IIDM_DYNAMICMODELINFOADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_IIDM_DYNAMICMODELINFOADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <string>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace iidm {

class DynamicModelInfoAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit DynamicModelInfoAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    DynamicModelInfoAdder(const DynamicModelInfoAdder&) = default;

    /**
     * Move constructor
     */
    DynamicModelInfoAdder(DynamicModelInfoAdder&&) = default;

    /**
     * Destructor
     */
    ~DynamicModelInfoAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    DynamicModelInfoAdder& operator=(const DynamicModelInfoAdder&) = delete;

    /**
     * Move assignment operator
     */
    DynamicModelInfoAdder& operator=(DynamicModelInfoAdder&&) = delete;

    DynamicModelInfoAdder& setModelName(const std::string& modelName);

protected:
    /**
     * Creates the DynamicModelInfo extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:

/**
 * The dynamic model name
 */
    std::string m_modelName;

};

}  // namespace iidm

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_IIDM_HVDCANGLEDROOPACTIVEPOWERCONTROLADDER_HPP
