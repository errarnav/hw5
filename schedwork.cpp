#ifndef RECCHECK
#include <iostream>
#include <vector>
#include <algorithm>
#endif

#include "schedwork.h"
using namespace std;

// helper prototype
bool helper(const AvailabilityMatrix& avail,
            size_t dailyNeed,
            size_t maxShifts,
            DailySchedule& sched,
            vector<size_t>& shiftCount,
            size_t day,
            size_t slot);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched)
{
    if(avail.empty())
        return false;

    size_t n = avail.size();      // days
    size_t k = avail[0].size();   // workers

    sched.assign(n, vector<Worker_T>());
    vector<size_t> shiftCount(k, 0);

    return helper(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);
}

bool helper(const AvailabilityMatrix& avail,
            size_t dailyNeed,
            size_t maxShifts,
            DailySchedule& sched,
            vector<size_t>& shiftCount,
            size_t day,
            size_t slot)
{
    size_t n = avail.size();
    size_t k = avail[0].size();

    // Completed all days
    if(day == n)
        return true;

    // If this day is full, move to next day
    if(slot == dailyNeed)
        return helper(avail, dailyNeed, maxShifts, sched, shiftCount, day + 1, 0);

    // Try assigning each worker
    for(size_t w = 0; w < k; w++) {

        if(!avail[day][w]) continue;
        if(shiftCount[w] >= maxShifts) continue;

        // No duplicates in the same day
        if(find(sched[day].begin(), sched[day].end(), w) != sched[day].end())
            continue;

        // Assign worker
        sched[day].push_back(w);
        shiftCount[w]++;

        // Recurse to fill next slot
        if(helper(avail, dailyNeed, maxShifts, sched, shiftCount, day, slot + 1))
            return true;

        // Backtrack
        sched[day].pop_back();
        shiftCount[w]--;
    }

    // No worker fits here → dead end
    return false;
}