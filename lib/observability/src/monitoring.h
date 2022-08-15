#ifndef ROBOT_BOARDS_MONITORING_H
#define ROBOT_BOARDS_MONITORING_H

#include <list>

// Default monitoring list depth if none is provided
#define DEFAULT_MONITORING_LIST_DEPTH 10

/**
 * Monitoring
 *
 * Monitoring object to observe the health of the board
 */
class Monitoring
{
private:
    // List to store the start times of the main code execution
    std::list<uint32_t> *cycle_start_times;
    // List to stire the end times of the main code execution
    std::list<uint32_t> *cycle_end_times;

    // Depth of the lists used in the instance
    uint8_t depth;

    // Average number of cycles per second calculated by the instance
    float average_cycle_rate;
    // Average cycle duration in milliseconds calculated by the instance
    float average_cycle_time;
    // Average main code execution time in milliseconds calculated by the instance
    float average_execution_time;

public:
    /**
     * @brief Construct a new Monitoring object
     *
     * @param depth Optional depth of the monitoring time lists
     */
    Monitoring(uint8_t depth = DEFAULT_MONITORING_LIST_DEPTH);

    /**
     * @brief Update the monitoring instance at the start of the main code execution
     */
    void start();

    /**
     * @brief Update the monitoring instance at the end of the main code execution
     */
    void end();

    /**
     * @brief Get the Average Cycle Rate
     *
     * @return float Latest average cycle time in milliseconds
     */
    float getAverageCycleRate();

    /**
     * @brief Get the Average Cycle Time
     *
     * @return float Latest average cycle rate
     */
    float getAverageCycleTime();

    /**
     * @brief Get the Average Execution Time
     *
     * @return float Latest average main code execution time in milliseconds
     */
    float getAverageExecutionTime();
};

#endif
