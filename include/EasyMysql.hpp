#include<iostream>
#include<mysql/mysql.h>

#include<string>
#include<vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class EasyMysql //单例模式
{
public:

    static EasyMysql* getInstance(const string &ip,
                                  const string &user,
                                  const string &passwd
                                 );

    void write_to_mysql(const string &command);
    vector<vector<string>> read_from_mysql(const string &command);
private:
    static void distroy();
    EasyMysql(const string &ip,
              const string &user,
              const string &passwd
             );//构造函数中初始化MYSQL*
    ~EasyMysql();//析构函数释放MYSQL*
    
    static void autoRelease();//多线程安全的自动释放
    
    void create_new_database();
    void create_new_tables(const string &commmand);
private:
    MYSQL *_mysql;

    static EasyMysql* _pInstance;
};
