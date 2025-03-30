#ifndef MONITOR_HPP
#define MONITOR_HPP

class Monitor 
{
    private:
        static void monitor(void *pvParameters);
    public:
        static void monitorTask();
};

#endif //MONITOR_HPP