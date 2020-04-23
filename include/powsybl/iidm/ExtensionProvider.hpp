/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_EXTENSIONPROVIDER_HPP
#define POWSYBL_IIDM_EXTENSIONPROVIDER_HPP

#include <string>

namespace powsybl {

namespace iidm {

class ExtensionProvider {
public:
    /**
     * Copy constructor
     */
    ExtensionProvider(const ExtensionProvider&) = default;

    /**
     * Move constructor
     */
    ExtensionProvider(ExtensionProvider&&) noexcept = default;

    /**
     * Destructor
     */
    virtual ~ExtensionProvider() noexcept = default;

    /**
     * Copy assignment operator
     */
    ExtensionProvider& operator=(const ExtensionProvider&) = default;

    /**
     * Move assignment operator
     */
    ExtensionProvider& operator=(ExtensionProvider&&) noexcept = default;

    /**
     * Return the name of the category of this {@link ExtensionProvider} instance
     *
     * @return the name of the category
     */
    const std::string& getCategoryName() const;

    /**
     * Return the name of the extension associated to this {@link ExtensionProvider} instance
     *
     * @return the name of the extension
     */
    const std::string& getExtensionName() const;

protected:
    /**
     * Create an {@link ExtensionProvider} instance for an extension of a category
     *
     * @param extensionName The name of the extension associated to this {@link ExtensionProvider} instance
     * @param categoryName The category of the extension associated to this {@link ExtensionProvider} instance
     */
    ExtensionProvider(std::string&& extensionName, std::string&& categoryName);

private:
    std::string m_extensionName;

    std::string m_categoryName;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_EXTENSIONPROVIDER_HPP
