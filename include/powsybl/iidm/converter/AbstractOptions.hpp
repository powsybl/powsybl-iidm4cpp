/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HPP

#include <set>
#include <string>

#include <powsybl/stdcxx/optional.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class Extension;

namespace converter {

template<typename Options>
class AbstractOptions {

public:
    /**
     * Return true if an exception should be thrown if an XML serializer for an extension is not found.
     *
     * @return true if an exception should be thrown, false otherwise
     */
    bool isThrowExceptionIfExtensionNotFound() const;

    /**
     * Return true if the automation systems must be managed.
     *
     * @return true if the automation systems must be managed, false otherwise
     */
    bool isWithAutomationSystems() const;

    /**
     * Enable/Disable the lookup of extension's serializers permissive mode.
     *
     * @param throwExceptionIfExtensionNotFound The extension's serializer lookup status
     *
     * @return This Options object
     */
    Options& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

    /**
     * Enable/Disable the management of the automation systems
     *
     * @param withAutomationSystems Set to true to manage automation systems
     *
     * @return this Options object
     */
    Options& setWithAutomationSystems(bool withAutomationSystems);


    /**
     * Add an extension to the list of included extensions.
     *
     * @param extension The name of the extension to be included
     *
     * @return this ExportOptions object
     */
    Options& addIncludedExtension(const std::string& extension);
    /**
     * Add an extension to the list of excluded extensions.
     *
     * @param extension The name of the extension to be excluded
     *
     * @return this ExportOptions object
     */
    Options& addExcludedExtension(const std::string& extension);

    /**
     * reset both included and excluded extensions list to a default configuration.
     */
    Options& resetExtensions();

    /**
     * Set the extensions to be included.
     *
     * @param extensions The set of extensions to be included
     *
     * @return this Options object
     */
    Options& setIncludedExtensions(const std::set<std::string>& extensions);
    /**
     * Set the extensions to be excluded.
     *
     * @param extensions The set of extensions to be excluded
     *
     * @return this Options object
     */
    Options& setExcludedExtensions(const std::set<std::string>& extensions);

    /**
     * Return true if one (or more) extension of the given list is managed
     *
     * @param extensions the list of extensions to be checked.
     *
     * @return true if at least one extension should is managed, false otherwise
     */
    bool hasAtLeastOneExtension(const stdcxx::const_range<Extension>& extensions) const;
    /**
     * Return true if the given extension is included.
     *
     * @param extension The name of the extension
     *
     * @return true if the given extension is managed, false otherwise
     */
    bool withExtension(const std::string& extension) const;

    /**
     * Return true if all extension are excluded.
     */
    bool withNoExtension() const;
    /**
     * Return true if all extensions are included.
     */
    bool withAllExtensions() const;


protected:
    /**
     * Default constructor
     */
    AbstractOptions() = default;

    AbstractOptions(bool throwExceptionIfExtensionNotFound);

    bool checkAndAddExtensions(bool includeExtensions, const std::set<std::string>& includedExtensions, bool excludeExtensions, const std::set<std::string>& excludedExtensions, bool warnOnInclusionEmptiness);


    bool m_throwExceptionIfExtensionNotFound = false;

    bool m_withAutomationSystems = true;

    stdcxx::optional<std::set<std::string>> m_includedExtensions;
    stdcxx::optional<std::set<std::string>> m_excludedExtensions;

};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/AbstractOptions.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HPP

