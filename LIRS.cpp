#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <string>

#include <boost/shared_ptr.hpp>

#include <muduo/base/Logging.h>

#define NODEBUG

#define LRUStackSLocation int
#define LRUStackLocation  int
#define LRUListQLocation  int
#define InvalidLocation -1
#define BottomLocation 0

class Entry;
typedef boost::shared_ptr<Entry> EntryPtr;

class Entry
{

public:

    enum EntryState
    {
        kLIR = 0,
        kresidentHIR,
        knonResidentHIR,
        kInvalid
    };

    explicit Entry(int value , EntryState state = kInvalid):
                   value_(value) , rValue_(value_) , state_(state)
    {

    }

    void setState(EntryState state)
    {
        state_ = state;
    }

    EntryState getState()
    {
        return state_;
    }

    std::string returnStateStr(EntryState state)
    {
        switch(state)
        {
        case kLIR:
            return "LIR";
        case kresidentHIR:
            return "resident HIR";
        case knonResidentHIR:
            return "no resident HIR";
        default:
            return "Invalid";
        }
    }

public:

    int const& rValue_;

private:

    EntryState state_;
    int value_;
};

class LRUStack
{
public:

    void movToTop(LRUStackLocation location)
    {
        if(location == container_.size() - 1)
            return;
        auto it = container_.begin() + location;
        EntryPtr tmp = container_[location];
        container_.erase(it);
        container_.push_back(tmp);
#ifndef NODEBUG
        debugToString("test");
#endif
    }

    void debugToString(std::string const& name)
    {
        std::cout << "#############" << name << "#############\n";

        std::for_each( container_.begin(), container_.end(), [](EntryPtr& item)
            { std::cout << "<" << item->rValue_ << " , " <<
              item->returnStateStr( item->getState() ) << ">\n";} );

        std::cout << "#############" << name << "#############\n";
    }

    LRUStackLocation find(int value)
    {
        LRUStackLocation location = InvalidLocation;
        int step = 0;
        for(auto it : container_)
        {
            if( it->rValue_ == value )
            {
                location = step;
                break;
            }
            else
                ++step;
        }

        return location;
    }

    void pushEntry(EntryPtr item)
    {
        container_.push_back(item);
    }

    void eraseBottomEntry()
    {
        container_.erase(container_.begin());
    }

    void eraseEntryByLocation(LRUStackLocation location)
    {
        container_.erase(container_.begin() + location);
    }

    void setTopState(Entry::EntryState state)
    {
        setStateByLocation(getContainerSize() - 1 , state);
    }

    void setBottomState(Entry::EntryState state)
    {
        setStateByLocation(BottomLocation , state);
    }

    void setStateByLocation(LRUStackLocation location , Entry::EntryState state)
    {
        (container_[location])->setState(state);
    }

    Entry::EntryState getBottomState()
    {
        return (container_[BottomLocation])->getState();
    }

    Entry::EntryState getStateByLocation(LRUStackLocation location)
    {
        return (container_[location])->getState();
    }


    EntryPtr getBottomEntry()
    {
        return container_[BottomLocation];
    }

    EntryPtr getTopEntry()
    {
        return container_[getContainerSize() - 1];
    }

    EntryPtr getEntryByLocation(LRUStackLocation location)
    {
        return container_[location];
    }

    int getSize()
    {
        return container_.size();
    }

private:

    typedef std::vector<EntryPtr> EntryVec;
    EntryVec container_;

    int getContainerSize()
    {
        return container_.size();
    }
};

class LRUStackS:public LRUStack
{
public:

    void stackPruning()
    {
        while (true)
        {
            Entry::EntryState state = getBottomState();
            assert(state != Entry::kInvalid);
            if(state == Entry::kresidentHIR || state == Entry::knonResidentHIR)
                eraseBottomEntry();
            else
                break;
        }
    }

    void findAndSetState(int value , Entry::EntryState state)
    {
        LRUStackSLocation location = find(value);
        if(location != InvalidLocation)
            setStateByLocation(location , state);
    }
};

class LRUListQ:public LRUStack
{
public:

    void pushToEnd(EntryPtr item)
    {
        pushEntry(item);
    }

    void popFront()
    {
        eraseBottomEntry();
    }

    void movToEnd(LRUListQLocation location)
    {
        movToTop(location);
    }

    void findAndRemove(int value)
    {
        LRUListQLocation location = find(value);
        if(location != InvalidLocation)
            eraseEntryByLocation(location);
    }

    EntryPtr getAndRemoveFrontEntry()
    {
        EntryPtr tmp = getBottomEntry();
        eraseBottomEntry();
        return tmp;
    }
};

class LIRSExecutor
{
public:

    LIRSExecutor(int lirSize , int hirSize):lirSize_(lirSize),
                                            hirSize_(hirSize),
                                            hitTimes_(0)
    {
        cache_.reserve(lirSize_ + hirSize_);
    }

    void access(int value)
    {
        auto ptr = std::find(cache_.begin() , cache_.end()
            , value);
        if( ptr != cache_.end() )
        {
            LRUStackSLocation location = stackS_.find(value);
            if(location >= 0)
            {
                Entry::EntryState state = stackS_.getStateByLocation(location);
                if(state == Entry::kLIR)
                {
                    stackS_.movToTop(location);
                    stackS_.stackPruning();
                }
                else
                {
                    hitHIRInStackS(location);
                    listQ_.findAndRemove(value);
                }
            }
            else
            {
                location = listQ_.find(value);
                if(location >= 0)
                {
                    stackS_.pushEntry(listQ_.getEntryByLocation(location));
                    listQ_.movToEnd(location);
                }
                else
                    LOG_FATAL << "hit but there is not such a man in LRU S stack\n";
            }
            hitTimes_++;
        }
        else
        {
            if( (lirSize_--) > 0)
            {
                cache_.push_back(value);
                EntryPtr tmp(new Entry(value , Entry::kLIR));
                stackS_.pushEntry(tmp);
            }
            else if( (hirSize_--) > 0)
            {
                cache_.push_back(value);
                addAResidentHIREntry(value);
            }
            else
            {
                EntryPtr tmp = listQ_.getAndRemoveFrontEntry();
                stackS_.findAndSetState(tmp->rValue_ , Entry::knonResidentHIR);//if find will set
                std::vector<int>::iterator ptr = std::find(cache_.begin() ,
                    cache_.end() , tmp->rValue_);
                if( ptr != cache_.end() )
                {
                    *ptr = value;
                }
                else
                    LOG_FATAL << "miss error\n";

                LRUStackSLocation location = stackS_.find(value);
                if(location >= 0)
                {
                    hitHIRInStackS(location);
                }
                else
                {
                    addAResidentHIREntry(value);
                }

            }
        }
        stackS_.debugToString("LRU stack S");
        std::cout << "\n";
        listQ_.debugToString("LRU list Q");
        std::cout << "\n";
    }

    inline int getHitTimes()
    {
        return hitTimes_;
    }

private:

    typedef std::vector<int> intVec;
    intVec cache_;
    int hitTimes_;
    int lirSize_;
    int hirSize_;

    LRUStackS stackS_;
    LRUListQ  listQ_;

    void addAResidentHIREntry(int value)
    {
        EntryPtr tmp(new Entry(value , Entry::kresidentHIR));
        stackS_.pushEntry(tmp);
        listQ_.pushToEnd(tmp);
    }

    void hitHIRInStackS(LRUStackSLocation location)
    {
        stackS_.movToTop(location);
        stackS_.setTopState(Entry::kLIR);
        stackS_.setBottomState(Entry::kresidentHIR);
        listQ_.pushToEnd( stackS_.getBottomEntry() );
        stackS_.stackPruning();
    }
};

int main(void)
{
    LIRSExecutor executor(3 , 2);
    int a[] = {1 , 2 , 3 , 4 , 5  , 1 , 2 , 3 , 4};
    for(int i = 0 ; i != 1 ; ++i)
    {
        for(int j = 0 ; j != sizeof(a)/sizeof(int) ; ++j)
        {
            executor.access(a[j]);
        }
    }
    std::cout << executor.getHitTimes() << "\n";
    return 0;
}