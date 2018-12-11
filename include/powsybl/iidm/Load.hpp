/**
 * Copyright (c) 2017, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_LOAD_HPP
#define POWSYBL_IIDM_LOAD_HPP

#include <vector>

#include <powsybl/iidm/Injection.hpp>
#include <powsybl/iidm/LoadType.hpp>
#include <powsybl/iidm/VariantManagerHolder.hpp>
#include <powsybl/stdcxx/reference_wrapper.hpp>

namespace powsybl {

namespace iidm {

class Load : public Injection {
public:
    Load(VariantManagerHolder& network, const std::string& id, const std::string& name, const LoadType& loadType,
         double p0, double q0);

    ~Load() noexcept override = default;

    const LoadType& getLoadType() const;

    double getP0() const;

    Load& setP0(double p0);

    double getQ0() const;

    Load& setQ0(double q0);

protected: // MultiVariantObject
    void allocateVariantArrayElement(const std::set<unsigned long>& indexes, unsigned long sourceIndex) override;

    void deleteVariantArrayElement(unsigned long index) override;

    void extendVariantArraySize(unsigned long initVariantArraySize, unsigned long number, unsigned long sourceIndex) override;

    void reduceVariantArraySize(unsigned long number) override;

private: // Identifiable
    const std::string& getTypeDescription() const override;

private:
    stdcxx::Reference<VariantManagerHolder> m_network;

    LoadType m_loadType;

    std::vector<double> m_p0;

    std::vector<double> m_q0;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_LOAD_HPP
