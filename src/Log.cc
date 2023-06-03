#include "../include/Log.hpp"
using std::ostringstream;
using std::string;




Mylogger *Mylogger::_pMylooger = nullptr;

Mylogger::Mylogger()
    : root(log4cpp::Category::getRoot()),
      sub1(log4cpp::Category::getInstance(std::string("sub1"))) {
    log4cpp::PropertyConfigurator::configure("../conf/myconf.conf");
}

Mylogger::~Mylogger() {}

void Mylogger::emerg(const char *msg) { sub1.emerg(msg); }

void Mylogger::fatal(const char *msg) { sub1.fatal(msg); }

void Mylogger::alert(const char *msg) { sub1.alert(msg); }

void Mylogger::crit(const char *msg) { sub1.crit(msg); }

void Mylogger::error(const char *msg) { sub1.error(msg); }

void Mylogger::warn(const char *msg) { sub1.warn(msg); }

void Mylogger::notice(const char *msg) { sub1.notice(msg); }

void Mylogger::info(const char *msg) { sub1.info(msg); }

void Mylogger::debug(const char *msg) { sub1.debug(msg); }


string Mylogger::int2string(int msg) {
    ostringstream oss;
    oss << msg;
    return oss.str();
}

// 将任意类型的数据转换为字符串
template <typename T>
string Mylogger::int2sting(T n) {
    ostringstream oss;
    oss << n;
    return oss.str();
}

// 将任意类型的数据转换为字符串
template <typename T>
string Mylogger::print(const T &value) {
    ostringstream oss_print;
    oss_print << value;
    return oss_print.str();
}





// 格式化输出，支持多个参数
template <typename... Args, typename T>
string Mylogger::print(const char *str, T &value, Args... args) {
    ostringstream oss_print;
    for (; *str != '\0'; ++str) {
        if (*str == '%' && *(str + 1) != '%') {
            str += 2;
            oss_print << value;
            oss_print << print(str, args...);
            return print(oss_print.str().c_str(), args...);
        }
        if (*str == '%') {
            ++str;
        }
        oss_print << *str;
    }
    return print(oss_print.str().c_str(), args...);
    
}

// 获取单例对象
Mylogger *Mylogger::getInstance() {
    if (_pMylooger == nullptr) {
        _pMylooger = new Mylogger();
    }
    return _pMylooger;
}

void test2() {
    int number = 100;
    const char *pstr = "hello, log4cpp";
    LogInfo("This is an info message. number = %d, str = %s\n", number, pstr);
    LogError("This is an error message. number = %d, str = %s\n", number, pstr);
    LogWarn("This is a warn message. number = %d, str = %s\n", number, pstr);
    LogDebug("This is a debug message. number = %d, str = %s\n", number, pstr);
}

int main() {
    test2();
    log4cpp::Category::shutdown();
}
