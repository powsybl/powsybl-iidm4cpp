/**
 * Copyright (c) 2021, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_THREADLOCALMULTIPLEVARIANTCONTEXT_HPP
#define POWSYBL_IIDM_THREADLOCALMULTIPLEVARIANTCONTEXT_HPP

#include <powsybl/iidm/VariantContext.hpp>
#include <powsybl/stdcxx/optional.hpp>

namespace powsybl {

namespace iidm {

class ThreadLocalMultipleVariantContext : public VariantContext {
public:  // VariantContext
    unsigned long getVariantIndex() const override;

    bool isIndexSet() const override;

    void resetIfVariantIndexIs(unsigned long index) override;

    void setVariantIndex(unsigned long index) override;

public:
    ThreadLocalMultipleVariantContext() = default;

    ~ThreadLocalMultipleVariantContext() noexcept override = default;

    ThreadLocalMultipleVariantContext(const ThreadLocalMultipleVariantContext&) = default;

    ThreadLocalMultipleVariantContext(ThreadLocalMultipleVariantContext&&) = default;

    ThreadLocalMultipleVariantContext& operator=(const ThreadLocalMultipleVariantContext&) = default;

    ThreadLocalMultipleVariantContext& operator=(ThreadLocalMultipleVariantContext&&) = default;

    void reset();

private:
    static thread_local stdcxx::optional<unsigned long> m_index;
};

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_THREADLOCALMULTIPLEVARIANTCONTEXT_HPP
