#include"../include/DictProducer_with_mysql.hpp"
#include"../include/splitTool.h"
#include"../include/EasyMysql.hpp"

#include<sys/time.h>
#include<ctime>
#include<iostream>

using std::cout;
using std::endl;

void Ensplit()
{
#if 1  //英文分词部分    
    EnSplitTool ensplit;

    DictProducer dictproducer("../data/en_corpus/");//en_corpus是语料库
    // dictproducer.show();
    dictproducer.readStopFile("../data/en_stop/stop_words_eng.txt"); //停止文件是相对路径

    string store_frequency_path("../data/dict/en_dict.dat"); //将词频写入该文件
    dictproducer.buildEnDict();
    // dictproducer.write_frequency_mysql(true);


    dictproducer.buildEnIndex();
    // dictproducer.write_index_mysql(true);//将索引库写入该文件
#endif

}


void Cnsplit()
{
    CnSplitTool cnsplit;

    //该路径是中文语料库
    DictProducer dictproducer("../data/cn_corpus/",&cnsplit);
    // dictproducer.show();
    dictproducer.readStopFile("../data/cn_stop/stop_words_cn.txt");//读停止文件
    
    string store_frequency_path("../data/dict/cn_dict.dat");//将词频写入该文件
    dictproducer.buildCnDict();
    // dictproducer.write_frequency_mysql(false);

#if 1
    dictproducer.buildCnIndex(store_frequency_path);
    dictproducer.write_index_mysql(false);
#endif
}

EasyMysql* EasyMysql::_pInstance=getInstance("localhost","root","123");
int main (int argc, char *argv[]) {
    struct timeval start,end;
    ::gettimeofday(&start,nullptr);

    Ensplit();//英文分词
    Cnsplit();//中文分词

    ::gettimeofday(&end,nullptr);

    double time=end.tv_sec-start.tv_sec;
    cout<<"执行时间: "<<time<<"秒"<<endl;
    return 0;
}



