#include"../include/EasyMysql.hpp"
#include<pthread.h>

EasyMysql* EasyMysql::getInstance(const string &host,
                                  const string &user,
                                  const string &passwd
                                  /*const string &use_database*/)
{
    if(_pInstance==nullptr)
    {
        _pInstance=new EasyMysql(host,user,passwd);

        pthread_once_t once=PTHREAD_ONCE_INIT;
        pthread_once(&once,autoRelease);
    }
    return _pInstance;
}


void EasyMysql::autoRelease()
{
    atexit(distroy);
}

void EasyMysql::distroy()
{
    if(_pInstance!=nullptr)
    {
        delete _pInstance;
        _pInstance=nullptr;
    }
}

EasyMysql::EasyMysql(const string &host,const string &user,const string &passwd/*const string &use_database*/)
:_mysql(mysql_init(nullptr))
{
    MYSQL *ret=mysql_real_connect(_mysql,host.c_str(),
                                  user.c_str(),
                                  passwd.c_str(),
                                  nullptr,
                                  0,nullptr,0); 
    if(ret==nullptr)
    {
        cout<<"EasyMysql::EasyMysql->mysql_real_connect failed"<<endl;
        exit(1);
    }

    create_new_database();
    create_new_tables("create table if not exists en_dict(id int auto_increment,"
                                                          "word varchar(24) not null,"
                                                          "frequency int not null,"
                                                          "primary key(id));"); 

    create_new_tables("create table if not exists cn_dict(id int auto_increment,"
                                                          "word varchar(16) not null,"
                                                          "frequency int not null,"
                                                          "primary key(id));"); 

    create_new_tables("create table if not exists en_index(alpha varchar(4) not null,"
                      "word_line mediumtext not null,"
                      "primary key(alpha));");

    create_new_tables("create table if not exists cn_index(hanzi varchar(8) not null,"
                      "word_line mediumtext not null,"
                      "primary key(hanzi));");
}

EasyMysql::~EasyMysql()
{
    mysql_close(_mysql);
}

void EasyMysql::create_new_database()
{
    string command("create database if not exists dict_and_index");
    int ret=mysql_query(_mysql,command.c_str());
    if(ret!=0)
    {
        cout<<">>EasyMysql::create_new_database failed"<<endl;
        exit(1);
    }

    ret=mysql_query(_mysql,"use dict_and_index");
    if(ret!=0)
    {
        cout<<"EasyMysql::create_new_database use dict_and_index failed"<<endl;
        exit(1);
    }
    cout<<command<<endl;
}

void EasyMysql::create_new_tables(const string &command)
{
    int ret=mysql_query(_mysql,command.c_str());
    if(ret!=0)
    {
        cout<<"EasyMysql::create_new_tables failed"<<endl;
        return;
    }
    cout<<command<<endl;
}

void EasyMysql::write_to_mysql(const string &command)
{
    int ret=mysql_query(_mysql,command.c_str());
    if(ret!=0)
    {
        cout<<command
        <<">>mysql write failed"<<endl;
        return;
    }
    // cout<<command<<endl;
}

vector<vector<string>> EasyMysql::read_from_mysql(const string &command)
{
    vector<vector<string>> tmp;

    tmp.reserve(256);

    int ret=mysql_query(_mysql,command.c_str());
    if(ret!=0)
    {
        cout<<">> read from mysql failed"<<endl;
        return vector<vector<string>>(); //如果读取失败则vector的size为0
    }

    MYSQL_RES* result=mysql_store_result(_mysql);
    if(result==nullptr)
    {
        return vector<vector<string>>();
    }

    MYSQL_ROW row;
    while((row=mysql_fetch_row(result))!=nullptr)
    {
        vector<string> rows;

		for(int i=0;i<mysql_num_fields(result);i++)
		{
		    rows.push_back(row[i]);
		}

		tmp.push_back(rows);
    }

    mysql_free_result(result);
    return tmp;
}


#if 0  //测试main
EasyMysql* EasyMysql::_pInstance=EasyMysql::getInstance("localhost","root","123");

int main (int argc, char *argv[]) {
     EasyMysql* easymysql=EasyMysql::getInstance("localhost","root","123");

#if 1
    easymysql->write_to_mysql("insert into en_dict(word,frequency) values('ikun3',1)"); 
    vector<vector<string>> ret=easymysql->read_from_mysql("select * from en_dict");

    
    for(auto &row:ret)
    {
		for(auto &field :row)
            cout<<field<<" ";
		cout<<endl;
    }
/*********************************************************************/
    easymysql->write_to_mysql("insert into cn_dict(word,frequency) values('清华',1)"); 
    ret=easymysql->read_from_mysql("select * from cn_dict");

    for(auto &row:ret)
    {
		for(auto &field :row)
            cout<<field<<" ";
		cout<<endl;
    }
#endif
/**********************************************************************/
#if 1
    easymysql->write_to_mysql("insert into en_index(alpha,word_line) values('o','1 2 3 4 5')"); 
    /*vector<std::pair<string,string>>*/ ret=easymysql->read_from_mysql("select alpha,word_line from en_index");
    
    for(auto &row:ret)
    {
		for(auto &field :row)
            cout<<field<<" ";
		cout<<endl;
    }
#endif
/**********************************************************************/
#if 1
    easymysql->write_to_mysql("insert into cn_index(hanzi,word_line) values('我','1 2 3 4 5')"); 
    /*vector<std::pair<string,string>>*/ ret=easymysql->read_from_mysql("select * from cn_index");
    
    for(auto &row:ret)
    {
		for(auto &field :row)
            cout<<field<<" ";
		cout<<endl;
    }
#endif
    return 0;
}
#endif
