/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_DCTOPOLOGYVARIANT_HPP
#define POWSYBL_IIDM_DCTOPOLOGYVARIANT_HPP

#include <powsybl/iidm/DcTopologyDcBusTopology.hpp>
#include <powsybl/iidm/Variant.hpp>
#include <powsybl/iidm/VariantArray.hpp>

namespace powsybl {

namespace iidm {

class DcTopologyModel;

namespace dc_topology_model {

class DcBusTopologyVariantImpl : public Variant<DcTopologyModel, DcBusTopologyVariantImpl> {
public: // Variant
    std::unique_ptr<DcBusTopologyVariantImpl> copy() const override;

public:
    explicit DcBusTopologyVariantImpl(DcTopologyModel& dcTopologyModel);

    DcBusTopologyVariantImpl(DcBusTopologyVariantImpl&&) noexcept = delete;

    DcBusTopologyVariantImpl(DcTopologyModel& dcTopologyModel, DcBusTopologyVariantImpl&& variant) noexcept;

    ~DcBusTopologyVariantImpl() noexcept override = default;

    const DcBusTopology& getDcBusTopology() const;
    DcBusTopology& getDcBusTopology();

private:
    DcBusTopology m_DcBusTopology;
};

using VariantArray = iidm::VariantArray<DcTopologyModel, DcBusTopologyVariantImpl>;

}  // namespace dc_topology_model

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_DCTOPOLOGYVARIANT_HPP