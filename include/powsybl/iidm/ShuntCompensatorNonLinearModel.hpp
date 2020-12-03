/**
 * Copyright (c) 2020, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODEL_HPP
#define POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODEL_HPP

#include <vector>

#include <powsybl/iidm/ShuntCompensatorModel.hpp>
#include <powsybl/stdcxx/range.hpp>

namespace powsybl {

namespace iidm {

class ShuntCompensatorNonLinearModel : public ShuntCompensatorModel {
public:
    class Section {
    public:
        Section(double b, double g);

        double getB() const;

        double getG() const;

        Section& setB(double b);

        Section& setG(double g);

    private:
        void setModel(ShuntCompensatorNonLinearModel& model);

        friend class ShuntCompensatorNonLinearModel;

    private:
        stdcxx::Reference<ShuntCompensatorNonLinearModel> m_model;

        double m_b;

        double m_g;
    };

public:
    explicit ShuntCompensatorNonLinearModel(ShuntCompensator& shuntCompensator, const std::vector<Section>& sections);

    ~ShuntCompensatorNonLinearModel() noexcept override = default;

    stdcxx::const_range<Section> getAllSections() const;

    stdcxx::range<Section> getAllSections();

private:
    double getB(unsigned long sectionCount) const override;

    double getG(unsigned long sectionCount) const override;

    unsigned long getMaximumSectionCount() const override;

    const ShuntCompensatorModelType& getType() const override;

private:
    std::vector<Section> m_sections;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODEL_HPP
