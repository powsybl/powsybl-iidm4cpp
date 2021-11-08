/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODEADDER_HPP
#define POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODEADDER_HPP

#include <string>

#include <powsybl/iidm/ExtensionAdder.hpp>

namespace powsybl {

namespace iidm {

namespace extensions {

namespace entsoe {

class XnodeAdder : public ExtensionAdder {
public:
    /**
     * Constructor
     */
    explicit XnodeAdder(Extendable& extendable);

    /**
     * Copy constructor
     */
    XnodeAdder(const XnodeAdder&) = default;

    /**
     * Move constructor
     */
    XnodeAdder(XnodeAdder&&) noexcept = default;

    /**
     * Destructor
     */
    ~XnodeAdder() noexcept override = default;

    /**
     * Copy assignment operator
     */
    XnodeAdder& operator=(const XnodeAdder&) = delete;

    /**
     * Move assignment operator
     */
    XnodeAdder& operator=(XnodeAdder&&) noexcept = delete;

    /**
     * Set code
     *
     * @param code The code
     *
     * @return this XnodeAdder object
     */
    XnodeAdder& withCode(const std::string& code);

protected:
    /**
     * Creates the Xnode extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    std::unique_ptr<Extension> createExtension(Extendable& extendable) override;

private:
    std::string m_code;
};

}  // namespace entsoe

}  // namespace extensions

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONS_ENTSOE_XNODEADDER_HPP
