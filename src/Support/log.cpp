#include "s3e/Support/log.hpp"

namespace Tom::s3e::Logging {
    Logger::Ptr Support = std::make_shared<Logger>("Support");
    Logger::Ptr Graphics = std::make_shared<Logger>("Graphics");
    Logger::Ptr Core = std::make_shared<Logger>("Core");
}
