#ifndef __TIMER_GPU__
#define __TIMER_GPU__

#include <OpenGL.h>
#include "Chrono.h"
#include "Message.h"
#include <math.h>

BABLIB_NAMESPACE_BEGIN

// timer using the OpenGL EXT_timer_query extension
class TimerGPU {
    public:
        TimerGPU() : t(0), n(0) {
            glGenQueries(1, &queryID);
            }
        
        ~TimerGPU() {
            glDeleteQueries(1, &queryID);
            }
        
        void begin() {
            glBeginQuery(GL_TIME_ELAPSED_EXT, queryID);
            }
        
        void end() {
            GLuint64EXT dt = 0;
            glEndQuery(GL_TIME_ELAPSED_EXT);

            //bool available = false;
            //while (!available)
            //    glGetQueryObjectiv(queryID, GL_QUERY_RESULT_AVAILABLE, &available);

            glGetQueryObjectui64vEXT(queryID, GL_QUERY_RESULT, &dt);
            t += dt;
            n++;
            }
        
        void reset() {
            t = 0;
            n = 0;
            }
        
        int         numQueries()    const { return n; }
        GLuint64EXT totalTimeNano() const { return t; }                             // in nanoseconds
        double      totalTime()     const { return double(t) / 1e6; }               // in miliseconds
        double      averageTime()   const { return double(t) / double(1e6 * n); }   // in miliseconds
        //@ precision coud be improved using better conversion from (uint64) (nanos) to double (milis)

        // number of bits used by GL to represent time values in nanoseconds
        // spec specifies that it should be more than 30 (so that at leasty one second can be represented)        
        static int precision() {
            int bits;
            glGetQueryiv(GL_TIME_ELAPSED_EXT, GL_QUERY_COUNTER_BITS, &bits);
            return bits;
            }
        
        static double maxPeriod() {     // maximum representable time period in miliseconds
            int bits = precision();
            return pow(2.0, bits) / 1e6;    //@ precision could be improved using better conversions
            }
        
    private:
        GLuint queryID;
        GLuint64EXT t;  // time in nanoseconds
        int n;          // number of queries since creation or last reset()
    };

#define CHRONO_GPU_ENFORCE_PERIOD
#ifndef CHRONO_GPU_ENFORCE_PERIOD
    // version where only the time when the timer is activated is compared against the specified period
    // it does not allow to guarantee a specific fequency for the display of messages
    class ChronoGPU {
        public:
            // period in miliseconds
            ChronoGPU(double period) : period(static_cast<GLuint64EXT>(period * 1e6)), n(0) {
                if (period > TimerGPU::maxPeriod())
                    MSG_ERROR("précision des timer queries (%1 bits) insuffisante pour la période spécifiée", (TimerGPU::precision()));
                }
            
            void begin() { timer.begin(); }
            void end() {
                timer.end();
                if (timer.totalTimeNano() > period) {
                    time = timer.averageTime();
                    MSG_INFO("# chrono total : %1 ms --> %2 Hz", (time) (1000.0 / time));
                    timer.reset();
                    updated = true;
                    n ++;
                    }
                }
            
            void reset() { n = 0; }
            int numDisp() const { return n; }
            
            bool wasUpdated() const { return updated; }
            double lastTime() const { return time; }
            
        private:
            const GLuint64EXT period;
            TimerGPU timer;
            int n;
            double time;
            bool updated;
        };
#else
    // version where the effective time is compared against the specified period
    // it allows to guarantee a specific fequency for the display of messages (which will be 1 / period)
    class ChronoGPU {
        public:
            // periode in miliseconds
            ChronoGPU(float period) : period(period), n(0) {
                if (period > TimerGPU::maxPeriod())
                    MSG_WARNING("précision des timer queries (%1 bits) potientiellement insuffisante pour la période spécifiée", (TimerGPU::precision()));
                // just a warning, because only the (a priori unknown) fraction of the period where the timer is activated matters
                }
            
            void begin() { timer.begin(); }
            void end(bool print=true) {
                timer.end();
                if (chrono.elapsed() > period) {
                    time = timer.averageTime();
                    if (print) MSG_INFO("> timer : %1 ms --> %2 Hz", (time) (1000.0 / time));
                    timer.reset();
                    chrono.restart();
                    updated = true;
                    n ++;
                    }
                else
                    updated = false;
                }
            
            void reset() { n = 0; }
            int numDisp() const { return n; }
            
            bool wasUpdated() const { return updated; }
            double lastTime() const { return time; }
            
        private:
            const float period;
            TimerGPU timer;
            Chrono chrono;
            int n;              // number of times the time was displayed since creation / last reset
            double time;        // in miliseconds
            bool updated;       // true iff the last 'end' call caused an update of the time
        };
#endif
    
//@ maybe make the two definitions available:
// abstract class PeriodEvaluator as template arg of ChronoGPU
//@ or just a bool param in constructor

    
BABLIB_NAMESPACE_END

#endif

