/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_PARAMETER_HPP
#define POWSYBL_IIDM_CONVERTER_PARAMETER_HPP

#include <set>
#include <string>
#include <vector>

#include <boost/any.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

enum class ParameterType : char {
    BOOLEAN,
    STRING,
    STRING_LIST
};

class Parameter {
public:
    Parameter() = delete;

    Parameter(const std::string& name, const ParameterType& type, const std::string& description, const std::string& defaultValue);

    Parameter(const std::string& name, const ParameterType& type, const std::string& description, bool defaultValue);

    Parameter(const std::string& name, const ParameterType& type, const std::string& description, const std::set<std::string>& defaultValue);

    ~Parameter() = default;

    Parameter(const Parameter&) = default;

    Parameter(Parameter&&) = default;

    Parameter& operator=(const Parameter&) = default;

    Parameter& operator=(Parameter&&) = default;

    Parameter& addAdditionalNames(const std::string& additionalName);

    template <typename T>
    T getDefaultValue() const;

    const std::string& getDescription() const;

    const std::string& getName() const;

    const std::vector<std::string>& getNames() const;

    const ParameterType& getType() const;

private:
    std::vector<std::string> m_names;

    ParameterType m_type;

    std::string m_description;

    boost::any m_defaultValue;
};

static const char* const ANONYMISED = "iidm.export.xml.anonymised";
static const char* const EXTENSIONS_LIST = "iidm.export.xml.extensions";
static const char* const INDENT = "iidm.export.xml.indent";
static const char* const ONLY_MAIN_CC = "iidm.export.xml.only-main-cc";
static const char* const THROW_EXCEPTION_IF_EXTENSION_NOT_FOUND = "iidm.export.xml.throw-exception-if-extension-not-found";
static const char* const TOPOLOGY_LEVEL = "iidm.export.xml.topology-level";
static const char* const VERSION = "iidm.export.xml.version";
static const char* const WITH_BRANCH_STATE_VARIABLES = "iidm.export.xml.with-branch-state-variables";

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#include <powsybl/iidm/converter/Parameter.hxx>

#endif  // POWSYBL_IIDM_CONVERTER_PARAMETER_HPP
