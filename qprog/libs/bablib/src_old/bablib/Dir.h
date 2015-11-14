#ifndef __OBJECT_DIR__
#define __OBJECT_DIR__

#include <QImage>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN

// representation of a file directory with practical functionality
class Dir {
    public:
        Dir();                              // undefined directory
        Dir(QString dirName);               // absolute path or relative to current directory
        Dir(QDir root, QString dirName);    // <root>/<dirName>
    
        bool isDefined() const;     // true ssi l'objet a été créé avec le constructeur non vide
        bool exists() const;        // true iff the directory actually exists (on the disk)
        
        // operations on files and directories :
        //--------------------------------------
            QString filePath(QString fileName) const;   // returns the path for the file named <fileName> in this directory
            bool contains(QString fileName) const;      // returns true iff this directory contains a file named <fileName>
    
            QString toQS() const;
            
            QString getName() const;        // name of the directory (without path prefix)
            QString getPath() const;        // full path of this directory
            
            QStringList getFileList(QString filter) const;  // list of files matching <filter> 
            QStringList getDirList() const;                 // list of sub-directories
            
            static QString cleanPath(const QString &path);
            static Dir dirForFile(QString fileName);        // returns the directory containing the provided file name
            
            Dir subDir(QString dirName) const;
            bool makeSubDir(QString dirName) const;
            bool checkSubDir(QString dirName) const;
            
            bool remove(QString fileName);
            
        // data save / load operations :
        //------------------------------
            void saveData(QDomDocument data, QString fileName = QString::null, QString ext = ".xml") const; // saves the XML <data> structure into file <fileName>
            QDomDocument loadData(QString fileName = QString::null, QString ext = ".xml") const;            // returns the XML data read from file <fileName>
            // on error, returns a null object (i.e. such that res.isNull())
            
            bool saveImage(QImage image,  QString fileName) const;  // saves an image in PNG format (8 bits)
            QImage loadImage(QString fileName) const;               // displays potential error messages
            // on error, returns a null image (i.e. such that res.isNull())
        
        // new file or dir creation / selection :
        //---------------------------------------
            // selects a sub directory named <objectName> :
            //   - if <objectName> == QString::null a dialog is displayed
            //   - on error (missing directory or dialog canceled), the result is empty (i.e. !res.isDefined())
            Dir readingDir(QString objectName = QString::null) const;
            
            // selects (and potentially creates) a sub directory named <objectName> :
            //   - if <objectName> == QString::null a dialog is displayed
            //   - on error (dialog canceled or writing error), the result is empty (i.e. !res.isDefined())
            //   - if <overwrite> == false and the subdir already exists, the result is empty
            //   - if <overwrite> == true and the subdir already exists, contained files are deleted
            Dir writingDir(QString objectName = QString::null, bool overwrite = true, bool clearDir = false) const;
            
            // selection of a file for reading or writing :
            //  - if fileName == QString::null, a dialog is displayed
            //  - if ext != QString::null, it is appended as a suffix of the returned file name
            //  - returns QString::null if no file name is selected (dialog closed or canceled)
            QString readingFileName(QString fileName = QString::null, QString ext = QString::null) const;
            QString writingFileName(QString fileName = QString::null, QString ext = QString::null) const;
        
    private:
        QDir dir;
        bool defined;   // to keep record of correctly initialized dirs
    };

BABLIB_NAMESPACE_END

#endif

