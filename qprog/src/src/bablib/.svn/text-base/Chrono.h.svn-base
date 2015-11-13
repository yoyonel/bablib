#ifndef __CHRONO__
#define __CHRONO__

#include <QTime>

//@ a voir : temps maxi mesurable par QTime = 1 jour?

BABLIB_NAMESPACE_BEGIN

class Chrono {
    public:
        Chrono();
        
        void start();           // démarre le chrono
        int restart();          // retourne le temps (en ms) depuis le dernier [re]start et redémarre le chrono
        int elapsed() const;    // en milisecondes
        float elapsedSecs() const;    // en secondes
        
    private:
        QTime time;
    };

BABLIB_NAMESPACE_END

#endif
