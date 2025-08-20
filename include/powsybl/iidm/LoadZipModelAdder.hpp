/**
 * Copyright (c) 2025, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOADZIPMODELADDER_HPP
#define POWSYBL_IIDM_LOADZIPMODELADDER_HPP

#include <powsybl/iidm/LoadZipModel.hpp>

namespace powsybl {

namespace iidm {

class LoadModel;
class LoadAdder;

class LoadZipModelAdder {
public:
    LoadZipModelAdder(LoadAdder& parent);

    ~LoadZipModelAdder() noexcept = default;
    LoadZipModelAdder(const LoadZipModelAdder&) = default;
    LoadZipModelAdder(LoadZipModelAdder&&) = default;
    LoadZipModelAdder& operator=(const LoadZipModelAdder&) = delete;
    LoadZipModelAdder& operator=(LoadZipModelAdder&&) = delete;

    LoadAdder& add();
    static void addNewModel(Load& load, double c0p, double c1p, double c2p, double c0q, double c1q, double c2q);

    LoadZipModelAdder& setC0p(double c0p);
    LoadZipModelAdder& setC1p(double c1p);
    LoadZipModelAdder& setC2p(double c2p);
    LoadZipModelAdder& setC0q(double c0q);
    LoadZipModelAdder& setC1q(double c1q);
    LoadZipModelAdder& setC2q(double c2q);

private:
    std::unique_ptr<LoadModel> build() const;
    friend class LoadAdder;

private:
    double m_c0p = 1.0;

    double m_c1p = 0.0;

    double m_c2p = 0.0;

    double m_c0q = 1.0;

    double m_c1q = 0.0;

    double m_c2q = 0.0;

    LoadAdder& m_parent;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOADZIPMODELADDER_HPP