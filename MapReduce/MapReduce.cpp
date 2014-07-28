#include "CLSliceMapper.h"

#include <boost/shared_ptr.hpp>

int main(int argc , char** argv)
{
    boost::shared_ptr<CLSliceMapper> mapper( new CLSliceMapper(argv[1] , argv[2]));
    (mapper.get())->makePairs();
    return 0;
}