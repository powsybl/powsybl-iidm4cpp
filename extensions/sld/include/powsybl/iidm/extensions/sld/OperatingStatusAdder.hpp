/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUSADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUSADDER_HPP

#include <powsybl/iidm/ExtensionAdder.hpp>

#include <powsybl/iidm/extensions/sld/OperatingStatus.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace sld {

class OperatingStatusAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit OperatingStatusAdder(Extendable& extendable);

    /**
     * Destructor
     */
    ~OperatingStatusAdder() noexcept override = default;

    /**
     * Copy constructor
     */
    OperatingStatusAdder(const OperatingStatusAdder&) = default;

    /**
     * Move constructor
     */
    OperatingStatusAdder(OperatingStatusAdder&&) = default;

    /**
     * Copy assignment operator
     */
    OperatingStatusAdder& operator=(const OperatingStatusAdder&) = delete;

    /**
     * Move assignment operator
     */
    OperatingStatusAdder& operator=(OperatingStatusAdder&&) = delete;

    /**
     * Set operating status
     *
     * @param status the operating status
     *
     * @return this OperatingStatusAdder object
     */
    OperatingStatusAdder& withStatus(const OperatingStatus::Status& status);

protected:
    /**
     * Creates the OperatingStatus extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) const override;

private:
    stdcxx::optional<OperatingStatus::Status> m_status;
};

}  // namespace sld

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_SLD_OPERATINGSTATUSADDER_HPP
