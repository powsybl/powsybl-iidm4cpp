/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HXX
#define POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HXX

#include <powsybl/iidm/converter/AbstractOptions.hpp>

#include <powsybl/iidm/Extension.hpp>

#include <powsybl/logging/Logger.hpp>
#include <powsybl/logging/LoggerFactory.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

template<typename Options>
AbstractOptions<Options>::AbstractOptions(bool throwExceptionIfExtensionNotFound) : 
    m_throwExceptionIfExtensionNotFound(throwExceptionIfExtensionNotFound) {
}

template<typename Options>
bool AbstractOptions<Options>::isThrowExceptionIfExtensionNotFound() const {
    return m_throwExceptionIfExtensionNotFound;
}

template<typename Options>
bool AbstractOptions<Options>::isWithAutomationSystems() const {
    return m_withAutomationSystems;
}

template<typename Options>
Options& AbstractOptions<Options>::setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound) {
    m_throwExceptionIfExtensionNotFound = throwExceptionIfExtensionNotFound;
    return static_cast<Options&>(*this);

}

template<typename Options>
Options& AbstractOptions<Options>::setWithAutomationSystems(bool withAutomationSystems) {
    m_withAutomationSystems = withAutomationSystems;
    return static_cast<Options&>(*this);
}


template<typename Options>
Options& AbstractOptions<Options>::addIncludedExtension(const std::string& extension) {
    if(m_excludedExtensions.has_value()) {
        if(m_excludedExtensions->erase(extension)) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
            logger.warn(stdcxx::format("Previously excluded extensions %1% will now be included", extension));
        }
    } else {
        if(!m_includedExtensions) {
            m_includedExtensions = std::set<std::string>();
        }
        m_includedExtensions->insert(extension);
    }
    return static_cast<Options&>(*this);
}

template<typename Options>
Options& AbstractOptions<Options>::addExcludedExtension(const std::string& extension) {
    if(m_includedExtensions.has_value()) {
        if(m_includedExtensions->erase(extension)) {
            logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
            logger.warn(stdcxx::format("Previously included extensions %1% will now be excluded", extension));
        }
    } else {
        if(!m_excludedExtensions) {
            m_excludedExtensions = std::set<std::string>();
        }
        m_excludedExtensions->insert(extension);
    }
    return static_cast<Options&>(*this);
}

template<typename Options>
Options& AbstractOptions<Options>::resetExtensions() {
    m_includedExtensions.reset();
    m_excludedExtensions.reset();
    return static_cast<Options&>(*this);
}

template<typename Options>
Options& AbstractOptions<Options>::setIncludedExtensions(const std::set<std::string>& extensions) {
    if(m_excludedExtensions.has_value()) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
        logger.warn(stdcxx::format("Previously excluded extensions list will now be ignored %1%", stdcxx::toString(m_excludedExtensions.get())));
    }
    if(extensions.empty()) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
        logger.warn(stdcxx::format("All extensions will be excluded"));
    }
    m_excludedExtensions.reset();
    m_includedExtensions = extensions;
    return static_cast<Options&>(*this);
}

template<typename Options>
Options& AbstractOptions<Options>::setExcludedExtensions(const std::set<std::string>& extensions) {
    if(m_includedExtensions.has_value()) {
        logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
        logger.warn(stdcxx::format("Previously included extensions list will now be ignored %1%", stdcxx::toString(m_includedExtensions.get())));
    }
    m_includedExtensions.reset();
    m_excludedExtensions = extensions;
    return static_cast<Options&>(*this);
}

template<typename Options>
bool AbstractOptions<Options>::checkAndAddExtensions(bool includeExtensions, const std::set<std::string>& includedExtensions, bool excludeExtensions, const std::set<std::string>& excludedExtensions, bool warnOnInclusionEmptiness) {
    if(includeExtensions && excludeExtensions) {
        throw PowsyblException("You can't define both included and excluded extensions in parameters.");
    }
    if(excludeExtensions) {
        AbstractOptions<Options>::setExcludedExtensions(excludedExtensions);
    }
    if(includeExtensions) {
        AbstractOptions<Options>::setIncludedExtensions(includedExtensions);
        if(includedExtensions.empty()) {
            if(warnOnInclusionEmptiness) {
                logging::Logger& logger = logging::LoggerFactory::getLogger<AbstractOptions>();
                logger.warn(stdcxx::format("All extensions are excluded."));
            }
            return false;
        }
    }
    return true;
}

template<typename Options>
bool AbstractOptions<Options>::hasAtLeastOneExtension(const stdcxx::const_range<Extension>& extensions) const {
    if (withAllExtensions()) {
        return true;
    }

    return (std::any_of(extensions.begin(), extensions.end(), [=](const Extension& extension) { 
                return withExtension(extension.getName()); 
        } ));
}

template<typename Options>
bool AbstractOptions<Options>::withExtension(const std::string& extension) const {
    return (!m_excludedExtensions || !m_excludedExtensions->count(extension)) && 
            (!m_includedExtensions || m_includedExtensions->count(extension));
}

template<typename Options>
bool AbstractOptions<Options>::withNoExtension() const {
    return m_includedExtensions.has_value() && m_includedExtensions->empty();
}

template<typename Options>
bool AbstractOptions<Options>::withAllExtensions() const {
    return !m_includedExtensions && (!m_excludedExtensions || m_excludedExtensions->empty());
}


}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_ABSTRACTOPTIONS_HXX
