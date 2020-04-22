/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

#include <set>
#include <string>

#include <powsybl/iidm/TopologyLevel.hpp>

namespace powsybl {

namespace iidm {

namespace converter {

class ExportOptions {
public:
    ExportOptions() = default;

    ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                  bool throwExceptionIfExtensionNotFound, const std::string& version);

    ExportOptions& addExtension(const std::string& extension);

    const std::string& getVersion() const;

    const TopologyLevel& getTopologyLevel() const;

    bool isAnonymized() const;

    bool isIndent() const;

    bool isOnlyMainCc() const;

    bool isThrowExceptionIfExtensionNotFound() const;

    bool isWithBranchSV() const;

    ExportOptions& setAnonymised(bool anonymized);

    ExportOptions& setIndent(bool indent);

    ExportOptions& setOnlyMainCc(bool onlyMainCc);

    ExportOptions& setThrowExceptionIfExtensionNotFound(bool throwExceptionIfExtensionNotFound);

    ExportOptions& setTopologyLevel(const TopologyLevel& topologyLevel);

    ExportOptions& setVersion(const std::string& version);

    ExportOptions& setWithBranchSV(bool withBranchSV);

    bool withExtension(const std::string& extension) const;

private:
    bool m_anonymized{false};

    bool m_indent{true};

    bool m_onlyMainCc{false};

    bool m_throwExceptionIfExtensionNotFound{false};

    TopologyLevel m_topologyLevel{TopologyLevel::NODE_BREAKER};

    bool m_withBranchSV{true};

    std::set<std::string> m_extensions;

    std::string m_version;
};

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

