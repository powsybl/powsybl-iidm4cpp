/**
 * Copyright (c) 2019, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP
#define POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

#include <powsybl/iidm/TopologyLevel.hpp>

namespace powsybl {

namespace iidm {

class ExportOptions {
public:
    ExportOptions() = default;

    ExportOptions(bool withBranchSV, bool indent, bool onlyMainCc, const TopologyLevel& topologyLevel,
                  bool throwExceptionIfExtensionNotFound);

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

    ExportOptions& setWithBranchSV(bool withBranchSV);

private:
    bool m_anonymized{false};

    bool m_indent{true};

    bool m_onlyMainCc{false};

    bool m_throwExceptionIfExtensionNotFound{false};

    TopologyLevel m_topologyLevel{TopologyLevel::NODE_BREAKER};

    bool m_withBranchSV{true};
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_EXPORTOPTIONS_HPP

