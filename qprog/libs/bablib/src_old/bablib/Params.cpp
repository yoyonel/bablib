#include <OpenGL.h>
#include "Params.h"
#include "Message.h"
#include "ColorTypes.h"
#include "ProgGLSL.h"
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"
#include <Dir.h>
#include <BablibConstants.h>
#include <QTextStream>
#include <QFile>
#include <QList>
#include <string>
BABLIB_NAMESPACE_BEGIN

	/***********************************************************************************************/
	// implÃÂ©mentation de Params :

	// fonctions statiques :
	QString Params::defaultFileName() { return BablibConstants::default_params_file(); }
ParamSet& Params::params() { static ParamSet *obj = new ParamSet(); return *obj; }      // warning: <obj> is never deleted

void Params::checkLoaded() { if (!params().defined()) load(); }
void Params::load(QString fileName) { params().load(fileName); }
void Params::reload() { params().reload(); }

bool Params::fileExists() { return QFile(defaultFileName()).exists(); }
/*
void Params::addShadersLocation(QString locationName, QString paramName) {
ProgGLSL::addLocation(locationName, get<QString>(paramName));
}
*/
void Params::parseShaderLocations() {
	checkLoaded(); params().parseShaderLocations();
}


/***********************************************************************************************/
// implÃÂ©mentation de GlobalParams :

ParamSet& GlobalParams::params() { static ParamSet *obj = new ParamSet(); return *obj; }    // warning: <obj> is never deleted

void GlobalParams::checkLoaded() {
	QString fileName = BablibConstants::global_params_file();
	if (Params::fileExists())
		fileName = Params::get<QString>("bablib.global.params.file", fileName, false);
	params().load(fileName);
	if (Params::fileExists())
		params().overwrite(Params::params());
}

QString GlobalParams::getPath(QString name, bool warn) {
	/*
	const bool relative = get<bool>("bablib.global.paths.relative", true, true);
	const QString dirName = get<QString>(name, QString::null, warn);
	return (relative && !dirName.isEmpty()) ? QString("%1/%2").arg(BablibConstants::global_data_path()).arg(dirName) : dirName;
	*/
	const QString dirName = get<QString>(name, QString::null, warn);
	if (dirName.isEmpty()) return QString::null;
	else if (dirName == "<data>")  return QString("%1/%2").arg(BablibConstants::global_data_path()).arg(get<QString>(name, QString::null, 1, warn));
	else if (dirName == "<qprog>") return QString("%1/%2").arg(BablibConstants::global_qprog_path()).arg(get<QString>(name, QString::null, 1, warn));
	else return dirName;
}

/***********************************************************************************************/
// implÃÂ©mentation de ParamSet :

void ParamSet::load(QString fileName) {
	this->fileName = fileName;
	if (defined()) reload();
}

void ParamSet::reload() {
	if (!defined())
		Message::error("aucun fichier n'a encore ÃÂ©tÃÂ© chargÃÂ©");
	else {
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			table.clear();

			int n = 0;
			while(!stream.atEnd()) {
				n++;
				QString line = stream.readLine().simplified().section("//", 0 , 0).section("#", 0, 0);
				if (line.isEmpty()) continue;

				// handle potiential include :
				const QString incTokenBegin = "@include(";
				const QString incTokenEnd   = ")";
				if (line.trimmed().startsWith(incTokenBegin) && line.trimmed().endsWith(incTokenEnd)) {
					QString incFileName = line.trimmed().mid(incTokenBegin.size(), -1);
					incFileName.chop(incTokenEnd.size());
					ParamSet subParamSet;
					subParamSet.load(Dir::dirForFile(fileName).filePath(incFileName));
					insert(subParamSet, true);      //@ todo : handle recursive includes..
					continue;
				}

				if (!line.contains('=')) {
					Message::warning(QString("erreur de syntaxe ÃÂ  la ligne %1 du fichier '%2' : pas de symbole '='").arg(n).arg(fileName));
					continue;
				}

				QString name  = line.section('=', 0, 0).trimmed(),
					value = line.section('=', 1).trimmed();
				if (name.isEmpty()) {
					Message::warning(QString("erreur de syntaxe ÃÂ  la ligne %1 du fichier '%2' : le nom ne peut pas ÃÂªtre vide").arg(n).arg(fileName));
					continue;
				}
				table.insert(name, value);
			}

			file.close();
		}
		else
			Message::error(QString("impossible d'ouvrir le fichier '%1' : %2").arg(fileName).arg(file.errorString()));
	}
}

/***********************************************************************************************/

bool ParamSet::save(QString fileName) const {
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
	QTextStream stream(&file);
	foreach (const QString &k, table.keys())
		stream << QString("%1 = %2\n").arg(k).arg(table[k]);
	stream.flush();
	file.close();
	//this->fileName = fileName;    //@ a voir + default fileName if already saved?
	return true;
}

/***********************************************************************************************/

template<>
void ParamSet::add(QString name, QString value, bool overwrite) {
	if (!defines(name)) table.insert(name, value);
	else if (overwrite) table[name] = value;
}

//@ integrate these specializations with the gets
#define DEF_ADD_NUMERIC(base_type)  \
	template<> void ParamSet::add(QString name, base_type v, bool overwrite) { add<QString>(name, QString::number(v), overwrite); }
#define DEF_ADD_NUMERIC_COMP2(comp_type)  \
	template<> void ParamSet::add(QString name, comp_type v, bool overwrite) { add<QString>(name, QString("%1 %2").arg(v.x).arg(v.y), overwrite); }
#define DEF_ADD_NUMERIC_COMP3(comp_type)  \
	template<> void ParamSet::add(QString name, comp_type v, bool overwrite) { add<QString>(name, QString("%1 %2 %3").arg(v.x).arg(v.y).arg(v.z), overwrite); }
#define DEF_ADD_NUMERIC_COMP4(comp_type)  \
	template<> void ParamSet::add(QString name, comp_type v, bool overwrite) { add<QString>(name, QString("%1 %2 %3 %4").arg(v.x).arg(v.y).arg(v.z).arg(v.w), overwrite); }

//template<> void ParamSet::add(QString name, float v, bool overwrite) { add(name, QString::number(v), overwrite); }

DEF_ADD_NUMERIC(int)
	DEF_ADD_NUMERIC(float)
	DEF_ADD_NUMERIC(double)

	DEF_ADD_NUMERIC_COMP2(Vec2)
	DEF_ADD_NUMERIC_COMP3(Vec3)
	DEF_ADD_NUMERIC_COMP4(Vec4)

	DEF_ADD_NUMERIC_COMP2(Vec2d)
	DEF_ADD_NUMERIC_COMP3(Vec3d)
	DEF_ADD_NUMERIC_COMP4(Vec4d)

	DEF_ADD_NUMERIC_COMP2(Float2)
	DEF_ADD_NUMERIC_COMP3(Float3)
	DEF_ADD_NUMERIC_COMP4(Float4)

	//template<>
	//void ParamSet::add(QString name, Vec2 v, bool overwrite) {
	//    add(name, QString("%1 %2").arg(v.x).arg(v.y), overwrite);
	//    }

	void ParamSet::overwrite(const ParamSet &params) {
		QList<QString> keys = table.keys();
		foreach (const QString &k, keys)
			if (params.defines(k)) table[k] = params.table[k];
}

void ParamSet::insert(const ParamSet &params, bool overwrite) {
	QList<QString> keys = params.table.keys();
	foreach (const QString &k, keys) {
		//if (!defines(k)) table.insert(k, params.table[k]);
		//else if (overwrite) table[k] = params.table[k];
		add<QString>(k, params.table[k], overwrite);
	}
}

/***********************************************************************************************/

void ParamSet::parseShaderLocations() const {
	const QString prefix = "shaders.location.";
	for (QHash<QString, QString>::const_iterator i = table.constBegin(); i != table.constEnd(); ++i) {
		if (i.key().startsWith(prefix)) {
			const QString locName = i.key().mid(prefix.size(), -1);
			ProgGLSL::addLocation(locName, i.value());
		}
	}
}

/***********************************************************************************************/

void ParamSet::errorFormat(QString name, QString format) const { Message::error(QString("le paramÃÂ¨tre '%1' doit ÃÂªtre au format <%2>").arg(name).arg(format)); }
void ParamSet::errorNotDef(QString name) const { Message::warning(QString("le paramÃÂ¨tre '%1' n'est pas dÃÂ©fini dans le fichier '%2'").arg(name).arg(fileName)); }

/***********************************************************************************************/

QString ParamSet::getComponent(QString name, int pos, int size) const {
	const QString &value = table[name];
	return pos == -1 ? value : value.section(" ", size * pos, size * (pos + 1) - 1);
}

QStringList ParamSet::getComponents(QString name, int size) const {
	//return table[name].split(" ", QString::SkipEmptyParts);
	QStringList list = table[name].split(" ", QString::SkipEmptyParts);
	QStringList res;
	while (!list.isEmpty()) {
		QStringList comp;
		for (int i=0; i<size; i++) comp << list.takeFirst();
		res << comp.join(" ");
	}
	return res;
}

/***********************************************************************************************/
/*
#define GET_VAL(type, size, defVal, instructions...) \
template<> type ParamSet::defaultValue() { return defVal; }                         \
template<> type ParamSet::get(QString name, type def, int pos, bool warn) const {   \
if (defines(name)) {                                                            \
QString val = getComponent(name, pos, size);                                \
instructions                                                                \
errorFormat(name, #type);                                                   \
}                                                                           \
else if (warn)                                                                  \
errorNotDef(name);                                                          \
return def;                                                                     \
}
*/
#define GET_VAL(type, size, defVal, instructions, ...) \
	template<> type ParamSet::defaultValue() { return defVal; }                         \
	template<> type ParamSet::get(QString name, type def, int pos, bool warn) const {   \
		if (defines(name)) {                                                            \
			QString val = getComponent(name, pos, size);                                \
			__VA_ARGS__	                                                                \
			errorFormat(name, #type);                                                   \
			}                                                                           \
		else if (warn)                                                                  \
			errorNotDef(name);                                                          \
		return def;                                                                     \
}

#define GET_NUM_VAL(type, size, defVal, convert)            \
	GET_VAL(type, size, defVal,                             \
	bool ok;                                            \
	type res = getComponent(name, pos).convert(&ok);    \
	if (ok) return res;                                 \
	)

/***********************************************************************************************/

GET_NUM_VAL(int,    1, 0, toInt)
	GET_NUM_VAL(float,  1, 0, toFloat)
	GET_NUM_VAL(double, 1, 0, toDouble)

	GET_VAL(bool, 1, false,
	if (val == "true"  || val == "1") return true;
if (val == "false" || val == "0") return false;
)

	GET_VAL(QString, 1, QString::null,
	return val;
)

	GET_VAL(std::string, 1, "",
	return val.toStdString();
)

	/*
	GET_VAL(Vec2, 2, Vec2(0,0),
	QString sx = val.section(" ", 0, 0),
	sy = val.section(" ", 1, 1);
	bool ok1; float x = sx.toFloat(&ok1);
	bool ok2; float y = sy.toFloat(&ok2);
	if (ok1 && ok2) return Vec2(x, y);
	)

	GET_VAL(Vec3, 3, Vec3(0,0,0),
	QString sx = val.section(" ", 0, 0),
	sy = val.section(" ", 1, 1),
	sz = val.section(" ", 2, 2);
	bool ok1; float x = sx.toFloat(&ok1);
	bool ok2; float y = sy.toFloat(&ok2);
	bool ok3; float z = sz.toFloat(&ok3);
	if (ok1 && ok2 && ok3) return Vec3(x, y, z);
	)

	GET_VAL(Float3, 3, Float3(0,0,0),
	QString sx = val.section(" ", 0, 0),
	sy = val.section(" ", 1, 1),
	sz = val.section(" ", 2, 2);
	bool ok1; float x = sx.toFloat(&ok1);
	bool ok2; float y = sy.toFloat(&ok2);
	bool ok3; float z = sz.toFloat(&ok3);
	if (ok1 && ok2 && ok3) return Float3(x, y, z);
	)

	GET_VAL(UByte3, 3, UByte3(0,0,0),
	QString sx = val.section(" ", 0, 0),
	sy = val.section(" ", 1, 1),
	sz = val.section(" ", 2, 2);
	bool ok1; uint x = sx.toUInt(&ok1);
	bool ok2; uint y = sy.toUInt(&ok2);
	bool ok3; uint z = sz.toUInt(&ok3);
	if (ok1 && ok2 && ok3) return UByte3(x, y, z);
	)

	GET_VAL(Float4, 4, Float4(0,0,0,0),
	bool ok1; float x = val.section(" ", 0, 0).toFloat(&ok1);
	bool ok2; float y = val.section(" ", 1, 1).toFloat(&ok2);
	bool ok3; float z = val.section(" ", 2, 2).toFloat(&ok3);
	bool ok4; float w = val.section(" ", 3, 3).toFloat(&ok4);
	if (ok1 && ok2 && ok3 && ok4) return Float4(x, y, z, w);
	)

	*/

#define GET_VAL_COMP2(vec_class_name, base_type, qstring_convert_method) \
	GET_VAL(vec_class_name, 2, vec_class_name(0,0),                      \
	QString sx = val.section(" ", 0, 0);                             \
	QString sy = val.section(" ", 1, 1);                             \
	bool ok1; base_type x = sx.qstring_convert_method(&ok1);         \
	bool ok2; base_type y = sy.qstring_convert_method(&ok2);         \
	if (ok1 && ok2) return vec_class_name(x, y);                     \
	)

#define GET_VAL_COMP3(vec_class_name, base_type, qstring_convert_method) \
	GET_VAL(vec_class_name, 3, vec_class_name(0,0,0),                    \
	QString sx = val.section(" ", 0, 0);                             \
	QString sy = val.section(" ", 1, 1);                             \
	QString sz = val.section(" ", 2, 2);                             \
	bool ok1; base_type x = sx.qstring_convert_method(&ok1);         \
	bool ok2; base_type y = sy.qstring_convert_method(&ok2);         \
	bool ok3; base_type z = sz.qstring_convert_method(&ok3);         \
	if (ok1 && ok2 && ok3) return vec_class_name(x, y, z);           \
	)

#define GET_VAL_COMP4(vec_class_name, base_type, qstring_convert_method) \
	GET_VAL(vec_class_name, 4, vec_class_name(0,0,0,0),                  \
	QString sx = val.section(" ", 0, 0);                             \
	QString sy = val.section(" ", 1, 1);                             \
	QString sz = val.section(" ", 2, 2);                             \
	QString sw = val.section(" ", 3, 3);                             \
	bool ok1; base_type x = sx.qstring_convert_method(&ok1);         \
	bool ok2; base_type y = sy.qstring_convert_method(&ok2);         \
	bool ok3; base_type z = sz.qstring_convert_method(&ok3);         \
	bool ok4; base_type w = sw.qstring_convert_method(&ok4);         \
	if (ok1 && ok2 && ok3 && ok4) return vec_class_name(x, y, z, w); \
	)


	GET_VAL_COMP2(Vec2, float, toFloat)
	GET_VAL_COMP3(Vec3, float, toFloat)
	GET_VAL_COMP4(Vec4, float, toFloat)

	GET_VAL_COMP2(Float2, float, toFloat)
	GET_VAL_COMP3(Float3, float, toFloat)
	GET_VAL_COMP4(Float4, float, toFloat)

	GET_VAL_COMP2(Vec2d, double, toDouble)
	GET_VAL_COMP3(Vec3d, double, toDouble)
	GET_VAL_COMP4(Vec4d, double, toDouble)

	GET_VAL_COMP3(UByte3, uint, toUInt)
	GET_VAL_COMP4(UByte4, uint, toUInt)

	#define _(c) if (val == #c) return c;

	GET_VAL(GLenum, 1, GL_FALSE,
	// formats d'interpolation de texture :
	_(GL_NEAREST)_(GL_LINEAR)_(GL_LINEAR_MIPMAP_LINEAR)
 
	// formats de wraping de texture :
	_(GL_REPEAT)_(GL_CLAMP)_(GL_CLAMP_TO_EDGE)_(GL_CLAMP_TO_BORDER)_(GL_MIRRORED_REPEAT)

	// opengl 1.1 :
	_(GL_ALPHA4)_(GL_ALPHA8)_(GL_ALPHA12)_(GL_ALPHA16)
	_(GL_LUMINANCE4)_(GL_LUMINANCE8)_(GL_LUMINANCE12)_(GL_LUMINANCE16)
	_(GL_LUMINANCE4_ALPHA4)_(GL_LUMINANCE6_ALPHA2)_(GL_LUMINANCE8_ALPHA8)_(GL_LUMINANCE12_ALPHA4)_(GL_LUMINANCE12_ALPHA12)_(GL_LUMINANCE16_ALPHA16)
	_(GL_INTENSITY)_(GL_INTENSITY4)_(GL_INTENSITY8)_(GL_INTENSITY12)_(GL_INTENSITY16)
	_(GL_RGB)_(GL_R3_G3_B2)_(GL_RGB4)_(GL_RGB5)_(GL_RGB8)_(GL_RGB10)_(GL_RGB12)_(GL_RGB16)_(GL_RGBA2)_(GL_RGBA4)_(GL_RGB5_A1)
	_(GL_RGBA)_(GL_RGBA8)_(GL_RGB10_A2)_(GL_RGBA12)_(GL_RGBA16)

	// opengl 1.4 :
	_(GL_DEPTH_COMPONENT16)_(GL_DEPTH_COMPONENT24)_(GL_DEPTH_COMPONENT32)

	// opengl 2.1 :
	_(GL_SRGB)_(GL_SRGB8)_(GL_SRGB_ALPHA)_(GL_SRGB8_ALPHA8)
	_(GL_SLUMINANCE_ALPHA)_(GL_SLUMINANCE8_ALPHA8)_(GL_SLUMINANCE)_(GL_SLUMINANCE8)
	_(GL_COMPRESSED_SRGB)_(GL_COMPRESSED_SRGB_ALPHA)_(GL_COMPRESSED_SLUMINANCE)_(GL_COMPRESSED_SLUMINANCE_ALPHA)

	// opengl 3.0 :
	_(GL_COMPRESSED_RED)_(GL_COMPRESSED_RG)
	_(GL_RGBA32F)_(GL_RGB32F)_(GL_RGBA16F)_(GL_RGB16F)
	_(GL_R11F_G11F_B10F)_(GL_UNSIGNED_INT_10F_11F_11F_REV)_(GL_RGB9_E5)_(GL_UNSIGNED_INT_5_9_9_9_REV)
	_(GL_RGBA32UI)_(GL_RGB32UI)_(GL_RGBA16UI)_(GL_RGB16UI)_(GL_RGBA8UI)_(GL_RGB8UI)
	_(GL_RGBA32I)_(GL_RGB32I)_(GL_RGBA16I)_(GL_RGB16I)_(GL_RGBA8I)_(GL_RGB8I)
	_(GL_RED_INTEGER)_(GL_GREEN_INTEGER)_(GL_BLUE_INTEGER)_(GL_ALPHA_INTEGER)
	_(GL_RGB_INTEGER)_(GL_RGBA_INTEGER)_(GL_BGR_INTEGER)_(GL_BGRA_INTEGER)

	// extension ARB_depth_buffer_float :
	_(GL_DEPTH_COMPONENT32F)_(GL_DEPTH32F_STENCIL8)_(GL_FLOAT_32_UNSIGNED_INT_24_8_REV)

	// extension ATI_texture_float :
	_(GL_RGBA_FLOAT16_ATI)_(GL_RGB_FLOAT16_ATI)_(GL_ALPHA_FLOAT16_ATI)_(GL_INTENSITY_FLOAT16_ATI)_(GL_LUMINANCE_FLOAT16_ATI)_(GL_LUMINANCE_ALPHA_FLOAT16_ATI)
	_(GL_RGBA_FLOAT32_ATI)_(GL_RGB_FLOAT32_ATI)_(GL_ALPHA_FLOAT32_ATI)_(GL_INTENSITY_FLOAT32_ATI)_(GL_LUMINANCE_FLOAT32_ATI)_(GL_LUMINANCE_ALPHA_FLOAT32_ATI)

	// extension ARB_texture_rg :
	_(GL_RED)_(GL_R8)_(GL_R16)
	_(GL_R16F)_(GL_R32F)
	_(GL_R8I)_(GL_R16I)_(GL_R32I)
	_(GL_R8UI)_(GL_R16UI)_(GL_R32UI)
	_(GL_RG)_(GL_RG_INTEGER)
	_(GL_RG8)_(GL_RG16)
	_(GL_RG16F)_(GL_RG32F)
	_(GL_RG8I)_(GL_RG16I)_(GL_RG32I)
	_(GL_RG8UI)_(GL_RG16UI)_(GL_RG32UI)

	// extension ARB_texture_float :
	_(GL_RGBA32F_ARB)_(GL_RGB32F_ARB)_(GL_ALPHA32F_ARB)_(GL_INTENSITY32F_ARB)_(GL_LUMINANCE32F_ARB)_(GL_LUMINANCE_ALPHA32F_ARB)
	_(GL_RGBA16F_ARB)_(GL_RGB16F_ARB)_(GL_ALPHA16F_ARB)_(GL_INTENSITY16F_ARB)_(GL_LUMINANCE16F_ARB)_(GL_LUMINANCE_ALPHA16F_ARB)

	// extension EXT_texture_integer :
	_(GL_RGBA32UI_EXT)_(GL_RGB32UI_EXT)_(GL_ALPHA32UI_EXT)_(GL_INTENSITY32UI_EXT)_(GL_LUMINANCE32UI_EXT)_(GL_LUMINANCE_ALPHA32UI_EXT)
	_(GL_RGBA16UI_EXT)_(GL_RGB16UI_EXT)_(GL_ALPHA16UI_EXT)_(GL_INTENSITY16UI_EXT)_(GL_LUMINANCE16UI_EXT)_(GL_LUMINANCE_ALPHA16UI_EXT)
	_(GL_RGBA8UI_EXT)_(GL_RGB8UI_EXT)_(GL_ALPHA8UI_EXT)_(GL_INTENSITY8UI_EXT)_(GL_LUMINANCE8UI_EXT)_(GL_LUMINANCE_ALPHA8UI_EXT)
	_(GL_RGBA32I_EXT)_(GL_RGB32I_EXT)_(GL_ALPHA32I_EXT)_(GL_INTENSITY32I_EXT)_(GL_LUMINANCE32I_EXT)_(GL_LUMINANCE_ALPHA32I_EXT)
	_(GL_RGBA16I_EXT)_(GL_RGB16I_EXT)_(GL_ALPHA16I_EXT)_(GL_INTENSITY16I_EXT)_(GL_LUMINANCE16I_EXT)_(GL_LUMINANCE_ALPHA16I_EXT)
	_(GL_RGBA8I_EXT)_(GL_RGB8I_EXT)_(GL_ALPHA8I_EXT)_(GL_INTENSITY8I_EXT)_(GL_LUMINANCE8I_EXT)_(GL_LUMINANCE_ALPHA8I_EXT)
	_(GL_RED_INTEGER_EXT)_(GL_GREEN_INTEGER_EXT)_(GL_BLUE_INTEGER_EXT)
	_(GL_RGB_INTEGER_EXT)_(GL_BGR_INTEGER_EXT)
	_(GL_RGBA_INTEGER_EXT)_(GL_BGRA_INTEGER_EXT)
	_(GL_ALPHA_INTEGER_EXT)_(GL_LUMINANCE_INTEGER_EXT)_(GL_LUMINANCE_ALPHA_INTEGER_EXT)
	)

	/***********************************************************************************************/

	/*
	#define GET_LIST(type, size, instructions...) \
	template<> QList<type> ParamSet::defaultValue() { return QList<type>(); }                               \
	template<> QList<type> ParamSet::get(QString name, QList<type> def, int , bool warn) const {     \
	if (defines(name)) {                                                        \
	QStringList values = getComponents(name, size);                         \
	QList<type> res;                                                        \
	bool ok = true;                                                         \
	foreach (QString val, values) {                                         \
	type v;                                                             \
	instructions                                                        \
	if (!ok) break;                                                     \
	res << v;                                                           \
	}                                                                   \
	if (ok) return res;                                                     \
	errorFormat(name, "QList<"#type">");                                    \
	}                                                                       \
	else if (warn)                                                              \
	errorNotDef(name);                                                      \
	return def;                                                                 \
	}
	*/
#define GET_LIST(type, size, instructions, ...) \
	template<> QList<type> ParamSet::defaultValue() { return QList<type>(); }                               \
	template<> QList<type> ParamSet::get(QString name, QList<type> def, int /*pos*/, bool warn) const {     \
		if (defines(name)) {                                                        \
			QStringList values = getComponents(name, size);                         \
			QList<type> res;                                                        \
			bool ok = true;                                                         \
			foreach (QString val, values) {                                         \
				type v;                                                             \
				__VA_ARGS__															\
				if (!ok) break;                                                     \
				res << v;                                                           \
				}                                                                   \
			if (ok) return res;                                                     \
			errorFormat(name, "QList<"#type">");                                    \
			}                                                                       \
		else if (warn)                                                              \
			errorNotDef(name);                                                      \
		return def;                                                                 \
	}

	GET_LIST(int,    1, v = val.toInt(&ok); )
	GET_LIST(float,  1, v = val.toFloat(&ok); )
	GET_LIST(double, 1, v = val.toDouble(&ok); )

	GET_LIST(bool, 1,
	if (val == "true"  || val == "1") v = true;
	else if (val == "false" || val == "0") v = false;
	else ok = false;
	)

		GET_LIST(QString, 1, v = val; )

		BABLIB_NAMESPACE_END

