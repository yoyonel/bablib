#ifndef __BABLIB_QGLV__
#define __BABLIB_QGLV__

#include <QGLViewer/qglviewer.h>
#include <QKeyEvent>

BABLIB_NAMESPACE_BEGIN

//@ fenetre params + touche toggle + save
//@ fps using timer GPU

class BablibQGLV : public QGLViewer {
    public:
        //@@ constructeurs
        /*
        setKeyDescription(Qt::Key_B, tr("Toggle display of the bounding box"));
        setKeyDescription(Qt::Key_R, tr("Reload the GLSL shaders"));
        setKeyDescription(Qt::Key_D, tr("Toggle display of the direction sphere"));
        setKeyDescription(Qt::Key_I, tr("Hide/show the interface"));
        setKeyDescription(Qt::Key_L, tr("Load a new relief object"));
        */
        
    protected:
        void keyPressEvent(QKeyEvent *e) {
            switch(e->key()) {
                /*
                case Qt::Key_1 :
                case Qt::Key_2 :
                case Qt::Key_3 :
                case Qt::Key_4 :
                    mode = e->key() - Qt::Key_1;
                    updateGL();
                    break;
                    
                case Qt::Key_R :
                    initGeom();
                    updateGL();
                    break;
                    
                case Qt::Key_N : {
                    bool ok;
                    int m = QInputDialog::getInteger(NULL, "param", "n", n, 0, 1000000000, 1, &ok);
                    if (ok) {
                        n = m;
                        initGeom();
                        }
                    updateGL();
                    break;
                    }
                */
                case Qt::Key_O:
                    // toggle camera ortho / perspective :
                    camera()->setType(camera()->type() == Camera::ORTHOGRAPHIC ? Camera::PERSPECTIVE : Camera::ORTHOGRAPHIC);
                    updateGL();
                    break;
                    
                case Qt::Key_F5:
                    Params::reload();
                    updateGL();
                    break;
                    
                case Qt::Key_Backspace : {
                    //@@ syntaxe lecture params
                    static bool whiteBackground = false;
                    setBackgroundColor(whiteBackground ? QColor(90,90,90,0) : QColor(255,255,255,0));
                    whiteBackground = !whiteBackground;
                    updateGL();
                    break;
                    }
                
                default:
                    QGLViewer::keyPressEvent(e);
                }
            }
    };

    /*
    // affichage du texte :
    if (drawInfoText) {
        glPushAttrib(GL_ENABLE_BIT);
        
        relief->infoList.add("taille ecran", "(" + QString::number(camera()->screenWidth()) + "," + QString::number(camera()->screenHeight()) + ")");
        
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        QStringList textList = relief->infoList.toText();
        int x = 15, y = 15;
        foreach (QString text, textList) {
            drawText(x, camera()->screenHeight() - y, text);
            y += 15;
            }
        
        glPopAttrib();
        }
    */
    //OpenGL::init();
    /*
    setBackgroundColor(QColor(90,90,90));
    setSnapshotFormat("PNG");   // format par défaut pour les images sauvegardées
    setAnimationPeriod(PARAM(bool, anim.fps.fixed) ? (int)floorf(1000.0f / PARAM(int, anim.fps)) : 0);
    if (PARAM(bool, anim.autostart)) startAnimation();
    */
BABLIB_NAMESPACE_END

#endif

