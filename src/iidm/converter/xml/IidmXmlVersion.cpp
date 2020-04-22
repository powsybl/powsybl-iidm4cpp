/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/converter/xml/IidmXmlVersion.hpp>

#include <functional>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <powsybl/PowsyblException.hpp>
#include <powsybl/logging/MessageFormat.hpp>
#include <powsybl/stdcxx/memory.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

bool operator==(const std::reference_wrapper<const IidmXmlVersion>& ref, const IidmXmlVersion& version) {
    return stdcxx::areSame(ref.get(), version);
}

IidmXmlVersion::IidmXmlVersion(std::string&& domain, std::vector<int>&& versionArray) noexcept :
    m_domain(domain),
    m_versionArray(versionArray) {
}

bool IidmXmlVersion::operator<(const IidmXmlVersion& version) const {
    const auto& versions = all();

    const auto& it1 = std::find(versions.cbegin(), versions.cend(), *this);
    const auto& it2 = std::find(versions.cbegin(), versions.cend(), std::cref(version));

    return it1 < it2;
}

bool IidmXmlVersion::operator<=(const IidmXmlVersion& version) const {
    const auto& versions = all();

    const auto& it1 = std::find(versions.cbegin(), versions.cend(), *this);
    const auto& it2 = std::find(versions.cbegin(), versions.cend(), std::cref(version));

    return it1 <= it2;
}

bool IidmXmlVersion::operator>(const IidmXmlVersion& version) const {
    return !(*this <= version);
}

bool IidmXmlVersion::operator>=(const IidmXmlVersion& version) const {
    return !(*this < version);
}

const IidmXmlVersions& IidmXmlVersion::all() {
    static std::vector<std::reference_wrapper<const IidmXmlVersion> > s_versions {{
        std::cref(IidmXmlVersion::V1_0()),
        std::cref(IidmXmlVersion::V1_1())
    }};

    return s_versions;
}

const IidmXmlVersion& IidmXmlVersion::CURRENT_IIDM_XML_VERSION() {
    return V1_1();
}

const IidmXmlVersion& IidmXmlVersion::fromNamespaceURI(const std::string& namespaceURI) {
    const std::string version = namespaceURI.substr(namespaceURI.find_last_of('/') + 1);
    const IidmXmlVersion& v = of(version, "_");
    if (namespaceURI != v.getNamespaceUri()) {
        throw PowsyblException(logging::format(
            "Namespace %1% is not supported. The namespace for IIDM XML version %2% is: %3%.",
            namespaceURI, v.toString("."), v.getNamespaceUri()));
    }

    return v;
}

std::string IidmXmlVersion::getNamespaceUri() const {
    return logging::format("http://www.%1%/schema/iidm/%2%", m_domain, toString("_"));
}

std::string IidmXmlVersion::getXsd() const {
    return logging::format("iidm_V%1%.xsd", toString("_"));
}

const IidmXmlVersion& IidmXmlVersion::of(const std::string& version, const std::string& separator) {
    for (const auto& v : all()) {
        if (v.get().toString(separator) == version) {
            return v;
        }
    }

    throw PowsyblException(logging::format("Version %1% is not supported.", version));
}

std::string IidmXmlVersion::toString(const std::string& separator) const {
    const auto& mapper = [](const int& i) { return std::to_string(i); };

    return boost::algorithm::join(m_versionArray | boost::adaptors::transformed(mapper), separator);
}

const IidmXmlVersion& IidmXmlVersion::V1_0() {
    static IidmXmlVersion V1_0("itesla_project.eu", {{1, 0}});
    return V1_0;
}

const IidmXmlVersion& IidmXmlVersion::V1_1() {
    static IidmXmlVersion V1_1("powsybl.org", {{1, 1}});
    return V1_1;
}

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl