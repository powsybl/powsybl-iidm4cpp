/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADEXPONENTIALMODEL_HPP
#define POWSYBL_IIDM_LOADEXPONENTIALMODEL_HPP

#include <memory>

#include <powsybl/iidm/LoadModel.hpp>

namespace powsybl {

namespace iidm {

class Load;

class LoadExponentialModel : public LoadModel {
public:
    LoadExponentialModel() = delete;
    ~LoadExponentialModel() noexcept override = default;

    double getNp() const;

    double getNq() const;

private:  // LoadModel

    LoadExponentialModel(double np, double nq);

    friend class LoadExponentialModelAdder;

    const LoadModelType& getType() const override;

private:
    double m_np;

    double m_nq;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADEXPONENTIALMODEL_HPP
