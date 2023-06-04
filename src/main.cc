#include"DictProducer.hpp"
#include"splitTool.hpp"

#include<ctime>
#include<iostream>

using std::cout;
using std::endl;

void Ensplit()
{
#if 1  //英文分词部分    
    EnSplitTool ensplit;

    DictProducer dictproducer("/home/aphelions2023/search_engine/create_dictionary/corpus/");//corpus是语料库
    // dictproducer.show();
    dictproducer.readStopFile("stop_words_eng.txt"); //停止文件是相对路径

    string store_frequency_path("word_frequency.conf"); //将词频写入该文件
    dictproducer.buildEnDict();
    dictproducer.write_frequency_file(store_frequency_path);


    dictproducer.buildEnIndex(store_frequency_path);
    dictproducer.write_index_file("word_line.conf");//将索引库写入该文件
#endif

}


void Cnsplit()
{
    CnSplitTool cnsplit;

    //该路径是中文语料库
    DictProducer dictproducer("/home/aphelions2023/search_engine/create_dictionary/cn_corpus/",&cnsplit);
    // dictproducer.show();
    dictproducer.readStopFile("stop_words_cn.txt");//读停止文件
    
    string store_frequency_path("word_frequency_cn.conf");//将词频写入该文件
    dictproducer.buildCnDict();
    dictproducer.write_frequency_file(store_frequency_path);

#if 1
    dictproducer.buildCnIndex(store_frequency_path);
    dictproducer.write_index_file("word_index_cn.conf");//将索引库写入该文件
#endif
}
int main (int argc, char *argv[]) {
    clock_t start=clock();
    Ensplit();//英文分词
    Cnsplit();//中文分词

    clock_t end=clock();

    double time=double(end-start)/CLOCKS_PER_SEC;
    cout<<"执行时间: "<<time<<"秒"<<endl;
    return 0;
}



