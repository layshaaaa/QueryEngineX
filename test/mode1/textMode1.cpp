#include "DicProducer.hpp"
#include "SplitTool.hpp"
#include <memory_resource>


int main(){
    SplitTool *splitTool = new SplitTool();
    DicProducer dicProducer(splitTool);

    return 0;
}
