/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREAADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREAADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>
#include <powsybl/iidm/extensions/entsoe/EntsoeGeographicalCode.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

class EntsoeAreaAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit EntsoeAreaAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    EntsoeAreaAdder(const EntsoeAreaAdder&) = default;

    /**
     * Move constructor
     */
    EntsoeAreaAdder(EntsoeAreaAdder&&) = default;

    /**
     * Destructor
     */
    ~EntsoeAreaAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    EntsoeAreaAdder& operator=(const EntsoeAreaAdder&) = delete;

    /**
     * Move assignment operator
     */
    EntsoeAreaAdder& operator=(EntsoeAreaAdder&&) = delete;

    /**
     * Set the code
     *
     * @param code the code
     *
     * @return this EntsoeAreaAdder object
     */
    EntsoeAreaAdder& withCode(const EntsoeGeographicalCode& code);

protected:
    /**
     * Creates the EntsoeArea extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    stdcxx::optional<EntsoeGeographicalCode> m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_ENTSOEAREAADDER_HPP
