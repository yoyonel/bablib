#include "ObjectXML.h"
BABLIB_NAMESPACE_USE

int ObjectXML::getIntAttrib(const QDomElement &e, const QString &name, int defaultValue) {
    bool ok;
    int res = e.attribute(name, QString::number(defaultValue)).toInt(&ok);
    return ok ? res : defaultValue;
    }

float ObjectXML::getFloatAttrib(const QDomElement &e, const QString &name, float defaultValue) {
    bool ok;
    float res = e.attribute(name, QString::number(defaultValue)).toFloat(&ok);
    return ok ? res : defaultValue;
    }

double ObjectXML::getDoubleAttrib(const QDomElement &e, const QString &name, double defaultValue) {
    bool ok;
    double res = e.attribute(name, QString::number(defaultValue)).toDouble(&ok);
    return ok ? res : defaultValue;
    }

