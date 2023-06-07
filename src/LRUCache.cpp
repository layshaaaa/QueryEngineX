#include"../include/LRUCache.hpp"
#include<fstream>
#include<iostream>
#include<sstream>

using std::istringstream;
using std::ostringstream;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


LRUCache::LRUCache(int capcity)
:_capcity(capcity)
,_mutex()
,_ready_to_write(capcity/2)
{

}

LRUCache::~LRUCache()
{

}

json LRUCache::readRecord(const key &_key)
{
    /*map<key.it>::iterator*/auto it=_key_index.find(_key);
    if(it==_key_index.end())
    {
        return json();
    }


    json &tmp=it->second->_cjson;//用tmp保存读到的json
    
    /*****************更新最热数据************************/
    _result_list.splice(_result_list.begin(),_result_list,it->second);
    return tmp;
}

int LRUCache::addRecord(const key &_key,const json &_json)
{
    if(_key.size()==0 | _json.size()==0)
    {
        //查询词为空，或者推荐词内容为空，都没必要插入
        return 0;//错误返回0
    }

    if(_key_index.find(_key)!=_key_index.end())
    {
        //说明该key已存在与map当中
        return 0;
    }


    _mutex.lock();
    if(_result_list.size()==_capcity)
    {
        //说明_result_list已满
        CacheNode tmp=_result_list.back();
        _result_list.pop_back();
        _key_index.erase(tmp._ckey);
    }
    
    //增加记录时，需要更新3个数据结构
    _result_list.push_front(CacheNode(_key,_json));
    _key_index.insert(make_pair(_key,_result_list.begin()));
    _new_result_persec.push_front(CacheNode(_key,_json));

    _mutex.unlock();


    if(_result_list.size()>=_ready_to_write)//缓存数量一旦大于阈值，就触发写入持久化文件
    {
        list<CacheNode> result_list=_result_list;
        write_to_File();
        _ready_to_write+=(_capcity/5);//每次触发后，更新阈值
    }
    //针对cache1的新写入列表，
    //因为cache1的新写入列表暂无实际用处，
    //所以内容一旦大于size()*2就将数据清空
    if(_new_result_persec.size()>_result_list.size()*2)
    {
        _new_result_persec.clear();
    }
    
    return 1;
}

void LRUCache::show()//测试函数
{
    for(auto &elem:_result_list)
    {
        cout<<elem._ckey<<":"<<elem._cjson<<endl;
    }

    for(auto &elem:_key_index)
    {
        cout<<elem.first<<" CacheNode-> "<<elem.second->_ckey<<" "<<elem.second->_cjson<<endl;
    }

    cout<<"************************\n"<<endl;
}

const list<CacheNode>& LRUCache::getNew_result_persec() const
{
    return _new_result_persec;
}

const list<CacheNode>& LRUCache::getResutlt() const
{
    return _result_list;
}

const map<key,list<CacheNode>::iterator>& LRUCache::getKey_index() const
{
    return _key_index;
}

void LRUCache::sync_with_cache1(const LRUCache &cache1)
{
    _mutex.lock();
    //获得cache1的节点列表和key的索引列表拿过来
    const list<CacheNode> & result_list=cache1.getResutlt();
    const map<key,list<CacheNode>::iterator> &key_index=cache1.getKey_index();

    _result_list.clear();
    _key_index.clear();

    _result_list.insert(_result_list.begin(),result_list.begin(),result_list.end());
    _key_index.insert(key_index.begin(),key_index.end()); 
    _mutex.unlock();
}

void LRUCache::write_to_cache1(LRUCache &cache1)//获得cache1
{
     //设置一种同步策略
     list<CacheNode>::iterator it=_new_result_persec.begin();
     if(_new_result_persec.size()>2)
     {
         for(int i=0;i<2;++i)
         {
            cache1.addRecord(it->_ckey,it->_cjson);
            ++it;
         }
     }
     else{
         for(int i=0;i<_result_list.size();++i)
         {
             cache1.addRecord(it->_ckey,it->_cjson);
             ++it;
         }
     }

     _new_result_persec.clear();
}

int LRUCache::getFromFile(const string& filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        cout<<"no cache configure yet"<<endl;
        return -1;
    }

    string line;
    while(std::getline(ifs,line))
    {
        istringstream iss(line);
        string _key,_json;
        iss>>_key;
        iss>>_json;

        addRecord(_key,_json);
    }

    return 0;
}

//应设置2处触发，timerfd就绪后触发，或者size>=某个阈值后触发
void LRUCache::write_to_File(const string &filename)
{
    ofstream ofs(filename,std::ios_base::out);
    if(!ofs.good())
    {
        cout<<"持久化文件不可写"<<endl;
        return;
    }
    
    ostringstream oss;
    for(auto &CacheNodes:_result_list)
    {
        oss<<CacheNodes._ckey<<" "<<CacheNodes._cjson<<endl;
    }

    if(oss.str().size()!=0)
    {
        ofs<<oss.str();
    }
}
