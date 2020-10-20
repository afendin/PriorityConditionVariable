#include "PCV.h"

PCV::PCV()
{

}

void PCV::wait(Lock *enclosingLock, int priority)
{
    // For every thread we create an entry for Condition Variable list
    // with corresponding lock and priority
    PriorityCV *pcv = new PriorityCV;
    pcv->priority = priority;
    pcv->lock = enclosingLock;

    // Pushing our enty in the list allows other threads to
    // wake us up while we waiting 
    pushToList(pcv);
    pcv->cv.wait(enclosingLock);

    // After waking up we remove ourselves from cv list
    removeFromList(pcv);
}

void PCV::signal(Lock *enclosingLock)
{
    m_lock.acquire();
    bool atLeastOne = false;

    list<PriorityCV*>::iterator it = m_CVList.begin();
    list<PriorityCV*>::iterator maxIt = it;

    // Wee loop through the whole list, searching for entries with corresponding
    // lock and wake up the one with the highest priority
    for (; it != m_CVList.end(); ++it) {
        if ((*it)->lock == enclosingLock) {
            if ((*it)->priority > (*maxIt)->priority || !atLeastOne) {
                atLeastOne = true;
                maxIt = it;
            }
        }
    }

    if (atLeastOne) (*maxIt)->cv.signal();
    m_lock.release();
}

void PCV::broadcast(Lock *enclosingLock, int priority)
{
    m_lock.acquire();
    list<PriorityCV*>::iterator it;

    // Same logic as with signal() method, the only difference is
    // that we wake up every thread with corresponding or highest priority
    for (it = m_CVList.begin(); it != m_CVList.end(); ++it) {
        if ((*it)->lock == enclosingLock && (*it)->priority >= priority) {
            (*it)->cv.signal();
        }
    }
    m_lock.release();
}

void PCV::pushToList(PriorityCV* pcv)
{
    m_lock.acquire();
    m_CVList.push_back(pcv);
    m_lock.release();
}

void PCV::removeFromList(PriorityCV* pcv)
{
    m_lock.acquire();
    m_CVList.remove(pcv);
    delete pcv;
    m_lock.release();
}
