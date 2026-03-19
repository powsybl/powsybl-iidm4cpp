/**
 * Copyright (c) 2026, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef POWSYBL_IIDM_CONVERTER_XML_XMLREADERENDTASK_HPP
#define POWSYBL_IIDM_CONVERTER_XML_XMLREADERENDTASK_HPP

#include <cstdint>
#include <functional>
#include <ostream>

namespace powsybl {

namespace iidm {

namespace converter {

namespace xml {

class XmlReaderEndTask {
public:

    enum class Step : std::uint8_t {
        BEFORE_EXTENSIONS,
        AFTER_EXTENSIONS
    };

    XmlReaderEndTask(const XmlReaderEndTask::Step& step, const std::function<void()>& task);

    const Step& getStep() const;

    const std::function<void()>& getTask() const;

    void runTask();

    bool isProcessed() const;
    void setProcessed(bool processed);

private:
    Step m_step;
    std::function<void()> m_task;
    bool m_processed;
};

std::ostream& operator<<(std::ostream& stream, const XmlReaderEndTask::Step& value);

}  // namespace xml

}  // namespace converter

}  // namespace iidm

}  // namespace powsybl

#endif  // POWSYBL_IIDM_CONVERTER_XML_XMLREADERENDTASK_HPP