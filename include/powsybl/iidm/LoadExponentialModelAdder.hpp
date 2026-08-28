/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADEXPONENTIALMODELADDER_HPP
#define POWSYBL_IIDM_LOADEXPONENTIALMODELADDER_HPP

#include <powsybl/iidm/LoadExponentialModel.hpp>

namespace powsybl {

namespace iidm {

class LoadModel;
class LoadAdder;

class LoadExponentialModelAdder : public PropertiesHolder {
public:
    LoadExponentialModelAdder(LoadAdder& parent);

    ~LoadExponentialModelAdder() noexcept = default;
    LoadExponentialModelAdder(const LoadExponentialModelAdder&) = default;
    LoadExponentialModelAdder(LoadExponentialModelAdder&&) = default;
    LoadExponentialModelAdder& operator=(const LoadExponentialModelAdder&) = delete;
    LoadExponentialModelAdder& operator=(LoadExponentialModelAdder&&) = delete;

    LoadAdder& add();
    static void addNewModel(Load& load, double np, double nq);

    LoadExponentialModelAdder& setNp(double np);
    LoadExponentialModelAdder& setNq(double nq);

private:
    std::unique_ptr<LoadModel> build() const;
    friend class LoadAdder;

private:
    double m_np = 0.0;

    double m_nq = 0.0;

    LoadAdder& m_parent;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADEXPONENTIALMODELADDER_HPP