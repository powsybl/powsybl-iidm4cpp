/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadZipModel.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

#include <cstdlib>

namespace powsybl {

namespace iidm {

LoadZipModel::LoadZipModel(double c0p, double c1p, double c2p, double c0q, double c1q, double c2q) :
    m_c0p(c0p),
    m_c1p(c1p),
    m_c2p(c2p),
    m_c0q(c0q),
    m_c1q(c1q),
    m_c2q(c2q) {
}

std::unique_ptr<LoadModel> LoadZipModel::build(Load& load, double c0p, double c1p, double c2p, double c0q, double c1q, double c2q) {
    checkCoefficient(load, c0p);
    checkCoefficient(load, c1p);
    checkCoefficient(load, c2p);
    checkCoefficient(load, c0q);
    checkCoefficient(load, c1q);
    checkCoefficient(load, c2q);

    if (std::abs(c0p + c1p + c2p - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(load, "Sum of c0p, c1p and c2p should be 1");
    }
    if (std::abs(c0q + c1q + c2q - 1.) > std::numeric_limits<double>::epsilon()) {
        throw ValidationException(load, "Sum of c0q, c1q and c2q should be 1");
    }

    return stdcxx::make_unique<LoadZipModel>(c0p, c1p, c2p, c0q, c1q, c2q);
}

double LoadZipModel::getC0p() const {
    return m_c0p;
}

double LoadZipModel::getC1p() const {
    return m_c1p;
}

double LoadZipModel::getC2p() const {
    return m_c2p;
}

double LoadZipModel::getC0q() const {
    return m_c0q;
}

double LoadZipModel::getC1q() const {
    return m_c1q;
}

double LoadZipModel::getC2q() const {
    return m_c2q;
}

const LoadModelType& LoadZipModel::getType() const {
    static LoadModelType s_type = LoadModelType::ZIP;
    return s_type;
}

}  // namespace iidm

}  // namespace powsybl
