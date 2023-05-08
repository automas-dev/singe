#include "singe/Support/log.hpp"

namespace singe::Logging {
    Logger::Ptr Graphics = std::make_shared<Logger>("Graphics");
    Logger::Ptr Core = std::make_shared<Logger>("Core");
}
