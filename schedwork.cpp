#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShiftCount,
    size_t day,
    size_t workerIndex
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    size_t n = avail.size();        // number of days
    size_t k = avail[0].size();     // number of workers
    
    // Initialize schedule with n days, each day empty
    sched.resize(n);
    
    // Track how many shifts each worker has been assigned
    vector<size_t> workerShiftCount(k, 0);
    
    // Start recursive backtracking from day 0, worker 0
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShiftCount, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShiftCount,
    size_t day,
    size_t workerIndex
)
{
    size_t n = avail.size();        // number of days
    size_t k = avail[0].size();     // number of workers
    
    // Base case: if we've filled all days completely
    if(day == n) {
        return true;
    }
    
    // If current day is full, move to next day
    if(sched[day].size() == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShiftCount, day + 1, 0);
    }
    
    // If we've exhausted all workers for this day slot, backtrack
    if(workerIndex == k) {
        return false;
    }
    
    // Try NOT scheduling this worker for this slot (skip to next worker)
    if(scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShiftCount, day, workerIndex + 1)) {
        return true;
    }
    
    // Try scheduling this worker if constraints are met
    // Check if worker is available and hasn't exceeded max shifts
    // and isn't already scheduled for this day
    if(avail[day][workerIndex] && 
       workerShiftCount[workerIndex] < maxShifts &&
       find(sched[day].begin(), sched[day].end(), workerIndex) == sched[day].end()) {
        
        // Schedule this worker
        sched[day].push_back(workerIndex);
        workerShiftCount[workerIndex]++;
        
        // Recurse to fill next slot
        if(scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShiftCount, day, workerIndex + 1)) {
            return true;
        }
        
        // Backtrack: unschedule this worker
        sched[day].pop_back();
        workerShiftCount[workerIndex]--;
    }
    
    return false;
}