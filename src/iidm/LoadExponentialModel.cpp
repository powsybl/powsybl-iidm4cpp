/**
 * Copyright (c) 2023, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <powsybl/iidm/LoadExponentialModel.hpp>

#include <powsybl/iidm/Load.hpp>
#include <powsybl/iidm/ValidationUtils.hpp>
#include <powsybl/stdcxx/make_unique.hpp>

namespace powsybl {

namespace iidm {

LoadExponentialModel::LoadExponentialModel(double np, double nq) :
    m_np(np),
    m_nq(nq) {
}

std::unique_ptr<LoadModel> LoadExponentialModel::build(Load& load, double np, double nq) {
    checkExponent(load, np);
    checkExponent(load, nq);

    return stdcxx::make_unique<LoadExponentialModel>(np, nq);
}

double LoadExponentialModel::getNp() const {
    return m_np;
}

double LoadExponentialModel::getNq() const {
    return m_nq;
}

const LoadModelType& LoadExponentialModel::getType() const {
    static LoadModelType s_type = LoadModelType::EXPONENTIAL;
    return s_type;
}

}  // namespace iidm

}  // namespace powsybl
