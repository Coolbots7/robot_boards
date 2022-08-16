#include <list>

#include "Arduino.h"

#include "./monitoring.h"

Monitoring::Monitoring(uint8_t depth)
{
    // Set the list depth for the instance
    this->depth = depth;

    // Initialize instance data members
    this->cycle_start_times = new std::list<uint32_t>();
    this->cycle_end_times = new std::list<uint32_t>();
    this->average_cycle_rate = 0.0f;
    this->average_cycle_time = 0.0f;
    this->average_execution_time = 0.0f;
}

void Monitoring::start()
{
    // Push the current time on to the list
    this->cycle_start_times->push_front(millis());

    // If the list length is greater than the required depth, drop the last item
    if (this->cycle_start_times->size() > this->depth)
    {
        this->cycle_start_times->pop_back();
    }

    // Need at least 2 items in the list to calculate the duration between them
    if (this->cycle_start_times->size() > 1)
    {
        // Sum the durations between start times
        uint32_t sum = 0;
        uint32_t prev = 0;
        std::list<uint32_t>::iterator it;
        for (it = this->cycle_start_times->begin(); it != this->cycle_start_times->end(); it++)
        {
            // Skip the fist item in the list
            if (it != this->cycle_start_times->begin())
            {
                // Get the duration between the list items and add it to the sum
                sum += (prev - *it);
            }
            prev = *it;
        }

        // Calculate the average cycle time
        this->average_cycle_time = sum / (this->cycle_start_times->size() - 1);
        // Use the average cycle time to calculate the average cycle rate
        this->average_cycle_rate = 1000 / this->average_cycle_time;
    }
}

void Monitoring::end()
{
    // Push the current time on to the list
    this->cycle_end_times->push_front(millis());

    // If the list length is greater than the required depth, drop the last item
    if (this->cycle_end_times->size() > this->depth)
    {
        this->cycle_end_times->pop_back();
    }

    // Sum the duration between start and end times
    uint32_t sum = 0;
    std::list<uint32_t>::iterator start_it = this->cycle_start_times->begin();
    std::list<uint32_t>::iterator end_it;
    for (end_it = this->cycle_end_times->begin(); end_it != this->cycle_end_times->end(); end_it++)
    {
        // Get the duration between the start and end items and add it to the sum
        sum += (*end_it - *start_it);

        // Increment the start time iterator to match the end iterator
        start_it++;
    }

    // Calculate the average execution time
    this->average_execution_time = sum / this->cycle_end_times->size();
}

float Monitoring::getAverageCycleRate()
{
    return this->average_cycle_rate;
}

float Monitoring::getAverageCycleTime()
{
    return this->average_cycle_time;
}

float Monitoring::getAverageExecutionTime()
{
    return this->average_execution_time;
}
