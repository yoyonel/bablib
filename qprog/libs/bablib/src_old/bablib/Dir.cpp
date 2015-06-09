#include "Dir.h"
#include "Message.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QInputDialog>
#include <QFileDialog>
BABLIB_NAMESPACE_USE

Dir::Dir() : defined(false) {}

Dir::Dir(QString objectDirName) : defined(true) {
    dir = QDir(objectDirName);
    }

Dir::Dir(QDir objectsDir, QString objectName) : defined(true) {
    dir = objectsDir;
    dir.cd(objectName);
    }

/***********************************************************************************************/

bool Dir::isDefined() const {
    return defined;
    }

bool Dir::exists() const {
    return dir.exists();
    }

QString Dir::filePath(QString fileName) const {
    return dir.filePath(fileName);
    }

bool Dir::contains(QString fileName) const {
    return dir.exists(fileName);
    }

QString Dir::toQS() const {
    return dir.path();
    }
            
QString Dir::getName() const {
    return dir.dirName();
    }

QString Dir::getPath() const {
    return dir.path();
    }

QString Dir::cleanPath(const QString &path) {
    return QDir::cleanPath(path);
    }

Dir Dir::subDir(QString dirName) const {
    return Dir(dir, dirName);
    }

bool Dir::makeSubDir(QString dirName) const {
    return dir.mkdir(dirName);
    }

bool Dir::checkSubDir(QString dirName) const {
    return contains(dirName) || makeSubDir(dirName);
    }

bool Dir::remove(QString fileName) {
    return dir.remove(fileName);
    }
/***********************************************************************************************/

Dir Dir::dirForFile(QString fileName) {
    int i = fileName.lastIndexOf("/");
    return Dir(i == -1 ? "." : fileName.left(i));
    }

/***********************************************************************************************/

bool Dir::saveImage(QImage image, QString fileName) const {
    const bool ok = image.save(filePath(fileName), "PNG");
    if (!ok) MSG_ERROR("problem on writing file '%1'", (fileName));
    return ok;
    }

QImage Dir::loadImage(QString fileName) const {
    QImage image;
    if (!image.load(filePath(fileName))) {
        MSG_ERROR("problem on reading file '%1'", (fileName));
        return QImage();
        }
    else
        return image;
    }

/***********************************************************************************************/

QDomDocument Dir::loadData(QString fileName, QString ext) const {
    fileName = readingFileName(fileName, ext);
    if (fileName == QString::null)
        return QDomDocument();
    
    QFile dataFile(filePath(fileName));
    if (dataFile.open(QIODevice::ReadOnly) == true) {
        QDomDocument doc;
        doc.setContent(&dataFile);
        dataFile.close();
        return doc;
        }
    else {
        MSG_ERROR("problem on reading file '%1' : %2", (fileName) (dataFile.errorString()));
        return QDomDocument();
        }
    }

void Dir::saveData(QDomDocument doc, QString fileName, QString ext) const {
    fileName = writingFileName(fileName, ext);
    if (fileName == QString::null) return;
    
    QFile dataFile(filePath(fileName));
    if (dataFile.open(QIODevice::WriteOnly)) {
        QTextStream dataStream(&dataFile);
        doc.save(dataStream, 2);
        dataFile.flush();
        dataFile.close();
        //Message::info("> objet enregistre.");
        }
    else
        MSG_ERROR("problem on writing file '%1' : %2", (fileName) (dataFile.errorString()));
    }

/***********************************************************************************************/

QStringList Dir::getFileList(QString filter) const {
    return dir.entryList(QStringList(filter), QDir::Files, QDir::Name);
    }

QStringList Dir::getDirList() const {
    return dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    }

/***********************************************************************************************/


QString Dir::readingFileName(QString fileName, QString ext) const {
    if (fileName == QString::null) fileName = QFileDialog::getOpenFileName(NULL, "Select a file name", filePath("."), "Data files (*" + ext + ")");
    if (fileName != QString::null && !fileName.endsWith(ext)) fileName += ext;
    return fileName;
    }

QString Dir::writingFileName(QString fileName, QString ext) const {
    if (fileName == QString::null) fileName = QFileDialog::getSaveFileName(NULL, "Select a file name", filePath("."), "Data files (*" + ext + ")");
    if (fileName != QString::null && !fileName.endsWith(ext)) fileName += ext;
    return fileName;
    }

/*
QString Dir::readingFileName(QString fileName, QString ext) const {
    if (fileName == QString::null) return QFileDialog::getOpenFileName(NULL, "Select a file name", filePath("."), "Data files (*" + ext + ")");
    if (!fileName.endsWith(ext)) fileName += ext;
    return filePath(fileName);
    }

QString Dir::writingFileName(QString fileName, QString ext) const {
    if (fileName == QString::null) return QFileDialog::getSaveFileName(NULL, "Select a file name", filePath("."), "Data files (*" + ext + ")");
    if (!fileName.endsWith(ext)) fileName += ext;
    return filePath(fileName);
    }
*/

/***********************************************************************************************/

Dir Dir::readingDir(QString objectName) const {
    QString objectDirName = objectName;
    if (objectName.isNull()) {
        QStringList objectList = getDirList();

        if (objectList.isEmpty()) {
            MSG_ERROR("no existing sub-directory in directory '%1'", (dir.dirName()));
            return Dir();
            }
        
        // choice of the object:
        bool ok;
        objectDirName = QInputDialog::getItem(NULL, "Object loading", "Select an object", objectList, 0, false, &ok);
        if (!ok) return Dir();
        }
    
    if (!contains(objectDirName)) {
        MSG_ERROR("no subdirectory '%1' found in directory '%2'", (objectName)(getPath()))
        return Dir();
        }
    
    //return contains(objectDirName) ? Dir(dir, objectDirName) : Dir();
    return Dir(dir, objectDirName);
    }

Dir Dir::writingDir(QString objectName, bool overwrite, bool clearDir) const {
    QString objectDirName = objectName;
    if (objectName.isNull()) {
        // choice of the object name using a dialog:
        bool ok;
        objectDirName = QInputDialog::getText(NULL, "Object saving", "Type a directory name", QLineEdit::Normal, "object", &ok);
        if (!ok) return Dir();
        }
    
    if (dir.exists(objectDirName)) {
        if (!overwrite) {
            MSG_ERROR("the sub-directory '%1' already exists", (objectDirName));
            return Dir();
            }
        else if (clearDir) {
            // delete files contained in the directory:
            QDir subDir = dir;
            subDir.cd(objectDirName);
            QStringList files = subDir.entryList(QDir::Files | QDir::NoSymLinks);
            foreach (QString file, files) subDir.remove(file);
            }
        }
    else if (!dir.mkdir(objectDirName)) {
        MSG_ERROR("cannot create directory '%1'", (objectDirName));
        return Dir();
        }
    
    return Dir(dir, objectDirName);
    }


