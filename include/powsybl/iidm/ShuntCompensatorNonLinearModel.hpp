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

namespace powsybl {

namespace iidm {

class ShuntCompensatorNonLinearModel : public ShuntCompensatorModel {
public:
    class Section {
    public:
        Section(unsigned long index, double b, double g);

        double getB() const;

        double getG() const;

        Section& setB(double b);

        Section& setG(double g);

    private:
        Section& setShuntCompensator(ShuntCompensator& shuntCompensator);

        friend class ShuntCompensatorNonLinearModel;

    private:
        stdcxx::Reference<ShuntCompensator> m_shuntCompensator;

        unsigned long m_index;

        double m_b;

        double m_g;
    };

public:
    explicit ShuntCompensatorNonLinearModel(const std::vector<Section>& sections);

    ~ShuntCompensatorNonLinearModel() noexcept override = default;

    const std::vector<Section>& getAllSections() const;

    std::vector<Section>& getAllSections();

private:
    double getB(unsigned long sectionCount) const override;

    double getG(unsigned long sectionCount) const override;

    unsigned long getMaximumSectionCount() const override;

    const ShuntCompensatorModelType& getType() const override;

    void setShuntCompensator(ShuntCompensator& shuntCompensator) override;

private:
    std::vector<Section> m_sections;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_SHUNTCOMPENSATORNONLINEARMODEL_HPP
