/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADZIPMODEL_HPP
#define POWSYBL_IIDM_LOADZIPMODEL_HPP

#include <memory>

#include <powsybl/iidm/LoadModel.hpp>

namespace powsybl {

namespace iidm {

class Load;

class LoadZipModel : public LoadModel {
public:
    LoadZipModel(double c0p, double c1p, double c2p, double c0q, double c1q, double c2q);

    ~LoadZipModel() noexcept override = default;

    static std::unique_ptr<LoadModel> build(Load& load, double c0p, double c1p, double c2p, double c0q, double c1q, double c2q);

    double getC0p() const;

    double getC1p() const;

    double getC2p() const;

    double getC0q() const;

    double getC1q() const;

    double getC2q() const;

private:  // LoadModel

    const LoadModelType& getType() const override;

private:
    double m_c0p;

    double m_c1p;

    double m_c2p;

    double m_c0q;

    double m_c1q;

    double m_c2q;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADZIPMODEL_HPP
