/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONADDER_HPP
#define POWSYBL_IIDM_EXTENSIONADDER_HPP

#include <memory>

namespace powsybl {

namespace iidm {

class Extendable;
class Extension;

class ExtensionAdder {
public:
    /**
     * Copy assignment operator
     */
    ExtensionAdder& operator=(const ExtensionAdder&) = delete;

    /**
     * Move assignment operator
     */
    ExtensionAdder& operator=(ExtensionAdder&&) noexcept = delete;

    /**
     * Add the extension.
     *
     * @return the extendable
     */
    Extendable& add();

protected:
    /**
     * Constructor
     */
    explicit ExtensionAdder(Extendable& extendable);

    /**
     * Destructor
     */
    virtual ~ExtensionAdder() noexcept = default;

    /**
     * Copy constructor
     */
    ExtensionAdder(const ExtensionAdder& extendable) = default;

    /**
     * Move constructor
     */
    ExtensionAdder(ExtensionAdder&&) noexcept = default;

private:
    /**
     * Creates the extension.
     *
     * @param extendable the extendable
     *
     * @return the extension
     */
    virtual std::unique_ptr<Extension> createExtension(Extendable& extendable) = 0;

private:
    Extendable& m_extendable;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONADDER_HPP
