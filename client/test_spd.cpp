#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <mutex>
#include <string>
// #include "log.h"

class DailyLogger {
public:
    // 获取单例实例，指定日志器名称和日志文件名
    static DailyLogger* getInstance(const std::string& logger_name = "daily_logger", const std::string& log_filename = "logs/log.txt") {

        if(!instance)
            {
                if(!instance)
                    {
                        instance = new DailyLogger(logger_name, log_filename);
                        static ResourceRecycle RR;
                    }
            }
        return instance;
    }
    class ResourceRecycle
    {
    public:
        ~ResourceRecycle()
            {
                if(DailyLogger::instance)
                    {
                        delete DailyLogger::instance;
                        DailyLogger::instance = nullptr;
                    }
            }
    };

    
    // 设置日志级别
    void setLogLevel(spdlog::level::level_enum level)
        {
            logger_->set_level(level);
        }

    // 记录带参数的日志，包括时间戳
    template<typename... Args>
    void log(spdlog::level::level_enum level, const char* fmt, const Args&... args)
            {
                auto now = std::chrono::system_clock::now();
                auto time_now = std::chrono::system_clock::to_time_t(now);
                std::stringstream ss;
                ss << std::put_time(std::localtime(&time_now), "%Y-%m-%d %H:%M:%S");

                std::string time_str = ss.str();
                std::string formatted_msg = "[" + time_str + "] " + fmt;

                std::lock_guard<std::mutex> lock(mutex_);
                logger_->log(level, formatted_msg.c_str(), args...);
            }
    ~DailyLogger()
        {
            spdlog::drop_all();
        }
private:
    std::shared_ptr<spdlog::logger> logger_;
    std::mutex mutex_; // 互斥锁，确保线程安全
    static DailyLogger* instance;

    DailyLogger(const std::string& logger_name, const std::string& log_filename)
                : logger_(spdlog::daily_logger_mt(logger_name, log_filename, 0, 0)) 
                {
        // 设置全局日志级别为信息级
        spdlog::set_level(spdlog::level::debug);
    }
    // 禁止复制和赋值
    DailyLogger(const DailyLogger&) = delete;
    DailyLogger& operator=(const DailyLogger&) = delete;

};
DailyLogger* DailyLogger::instance = nullptr;
int main() {
    // 获取DailyLogger的唯一实例，指定日志名称和日志文件名
    DailyLogger* logger = DailyLogger::getInstance("daily_logger", "logs/daily_log.txt");

    // 设置日志级别为debug级
    logger->setLogLevel(spdlog::level::debug);

    // 记录带参数的日志
    logger->log(spdlog::level::info, "测试单例类");
    logger->log(spdlog::level::info, "This is an info message: {}", 42);
    logger->log(spdlog::level::warn, "This is a warning message: {}", 3.14);
    logger->log(spdlog::level::err, "This is an error message: {}", "error details");

    // 模拟程序运行，每隔一段时间记录一条日志
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        logger->log(spdlog::level::debug, "Logging debug message #{}", i);
    }

    return 0;
}
