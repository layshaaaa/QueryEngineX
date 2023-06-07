#include<vector>
#include<map>
#include<set>
#include<string>

using std::vector;
using std::map;
using std::set;
using std::string;

class SplitTool;

class DictProducer
{
public:
    DictProducer(const string &dirpath);
    DictProducer(const string &dirpath,SplitTool *);
    ~DictProducer();


    void buildEnDict();
    void buildCnDict();
    void buildEnIndex(const string&);
    void buildCnIndex(const string&);


    void readStopFile(const string &filepath);
    void write_frequency_file(const string &path);//将词频写入文件
    void write_index_file(const string &path);//将字母行号写入文件
    void show();

private:
    void readDirpath(const string &dirpath);
    void Preprocess(string &line);//将每行的非字母改为空
                      //将每个大写字母改为小写

    vector<string> readFile(const string &filepath);//将文件以行为单位读入内存
    vector<string> readCnFile(const string &filepath);

    
private:
    vector<string> _filepath; //文件路径
    map<string,int> _word_frequency;
    map<string,set<int>> _alpha_index; //字母行号从0开始
    SplitTool *_cuttor;

    set<string> _stopfile;
};
//aa
