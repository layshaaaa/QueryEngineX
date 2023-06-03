#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <iostream>
#include <sstream>
#include <stddef.h>
#include <string.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#define LogEmerg(msg, ...)                                                     \
    Mylogger::getInstance()->emerg(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogFatal(msg, ...)                                                     \
    Mylogger::getInstance()->fatal(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogAlert(msg, ...)                                                     \
    Mylogger::getInstance()->alert(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogCrit(msg, ...)                                                      \
    Mylogger::getInstance()->crit(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogError(msg, ...)                                                     \
    Mylogger::getInstance()->error(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogWarn(msg, ...)                                                      \
    Mylogger::getInstance()->warn(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogNotice(msg, ...)                                                    \
    Mylogger::getInstance()->notice(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogInfo(msg, ...)                                                      \
    Mylogger::getInstance()->info(Mylogger::print(msg, __VA_ARGS__).c_str())

#define LogDebug(msg, ...)                                                     \
    do {                                                                       \
        std::stringstream ss;                                                  \
        ss << "[" << __FILE__ << ":" << Mylogger::int2string(__LINE__) << " (" \
           << __FUNCTION__ << ")"                                              \
           << "] " << msg;                                                     \
        Mylogger::getInstance()->debug(                                        \
            Mylogger::print(ss.str().c_str(), __VA_ARGS__).c_str());           \
    } while (false)

class Mylogger {
  public:
    void emerg(const char *msg);
    void fatal(const char *msg);
    void alert(const char *msg);
    void crit(const char *msg);
    void error(const char *msg);
    void warn(const char *msg);
    void notice(const char *msg);
    void info(const char *msg);
    void debug(const char *msg);
    static Mylogger *getInstance();

    static std::string int2string(int msg);

    template <typename T> static std::string int2sting(T n);

    template <typename T> static std::string print(const T &value);

    template <typename... Args, typename T>
    static std::string print(const char *str, T &value, Args... args);

  private:
    Mylogger();
    ~Mylogger();
    std::string packMesage(const char *msg);

  private:
    log4cpp::Category &root;
    log4cpp::Category &sub1;
    static Mylogger *_pMylooger;
};

#endif // !__LOG_HPP__
