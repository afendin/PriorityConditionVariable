#include <list>
using namespace std;
#include "3rdparty/CV.h"
#include "3rdparty/Lock.h"

struct PriorityCV
{
    int priority;
    CV cv;
    Lock *lock;
};

class PCV
{
public:
    PCV();

    void wait(Lock *enclosingLock, int priority);
    void signal(Lock *enclosingLock);
    void broadcast(Lock *enclosingLock, int priority);
private:
    void pushToList(PriorityCV* pcv);
    void removeFromList(PriorityCV* pcv);
    
    list<PriorityCV*> m_CVList;
    Lock m_lock;
};