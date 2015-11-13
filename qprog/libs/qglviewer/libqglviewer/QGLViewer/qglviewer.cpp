#include "domUtils.h"
#include "qglviewer.h"
#include "camera.h"
#include "keyFrameInterpolator.h"
#include "manipulatedCameraFrame.h"

# include <QtAlgorithms>
# include <QTextEdit>
# include <QApplication>
# include <QFileInfo>
# include <QDateTime>
# include <QMessageBox>
# include <QPushButton>
# include <QTabWidget>
# include <QTextStream>
# include <QMouseEvent>
# include <QTimer>
# include <QImage>
# include <QDir>
# include <QUrl>

using namespace std;
using namespace qglviewer;

// Static private variable
QList<QGLViewer*> QGLViewer::QGLViewerPool_;


/*! \mainpage

libQGLViewer is a free C++ library based on Qt that enables the quick creation of OpenGL 3D viewers.
It features a powerful camera trackball and simple applications simply require an implementation of
the <code>draw()</code> method. This makes it a tool of choice for OpenGL beginners and
assignments. It provides screenshot saving, mouse manipulated frames, stereo display, interpolated
keyFrames, object selection, and much more. It is fully
customizable and easy to extend to create complex applications, with a possible Qt GUI.

libQGLViewer is <i>not</i> a 3D viewer that can be used directly to view 3D scenes in various
formats. It is more likely to be the starting point for the coding of such a viewer.

libQGLViewer is based on the Qt toolkit and hence compiles on any architecture (Unix-Linux, Mac,
Windows, ...). Full reference documentation and many examples are provided.

See the project main page for details on the project and installation steps. */

void QGLViewer::defaultConstructor()
{
	// Test OpenGL context
	// if (glGetString(GL_VERSION) == 0)
	// qWarning("Unable to get OpenGL version, context may not be available - Check your configuration");

	int poolIndex = QGLViewer::QGLViewerPool_.indexOf(NULL);
	setFocusPolicy(Qt::StrongFocus);

	if (poolIndex >= 0)
		QGLViewer::QGLViewerPool_.replace(poolIndex, this);
	else
		QGLViewer::QGLViewerPool_.append(this);

	camera_ = new Camera();
	setCamera(camera());

	setDefaultShortcuts();
	setDefaultMouseBindings();

	setSnapshotFileName(tr("snapshot", "Default snapshot file name"));
	initializeSnapshotFormats();
	setSnapshotCounter(0);
	setSnapshotQuality(95);

	fpsTime_.start();
	fpsCounter_     = 0;
	f_p_s_          = 0.0;
	fpsString_      = tr("%1Hz", "Frames per seconds, in Hertz").arg("?");
	visualHint_     = 0;
	previousPathId_	= 0;
	// prevPos_ is not initialized since pos() is not meaningful here.
	// It will be set when setFullScreen(false) is called after setFullScreen(true)

	// #CONNECTION# default values in initFromDOMElement()
	manipulatedFrame_ = NULL;
	manipulatedFrameIsACamera_ = false;
	mouseGrabberIsAManipulatedFrame_ = false;
	mouseGrabberIsAManipulatedCameraFrame_ = false;
	displayMessage_ = false;
	connect(&messageTimer_, SIGNAL(timeout()), SLOT(hideMessage()));
	messageTimer_.setSingleShot(true);
	helpWidget_ = NULL;
	setMouseGrabber(NULL);

	setSceneRadius(1.0);
	showEntireScene();
	setStateFileName(".qglviewer.xml");

	// #CONNECTION# default values in initFromDOMElement()
	setAxisIsDrawn(false);
	setGridIsDrawn(false);
	setFPSIsDisplayed(false);
	setCameraIsEdited(false);
	setTextIsEnabled(true);
	setStereoDisplay(false);
	// Make sure move() is not called, which would call initializeGL()
	fullScreen_ = false;
	setFullScreen(false);

	animationTimerId_ = 0;
	stopAnimation();
	setAnimationPeriod(40); // 25Hz

	selectBuffer_ = NULL;
	setSelectBufferSize(4*1000);
	setSelectRegionWidth(3);
	setSelectRegionHeight(3);
	setSelectedName(-1);

	bufferTextureId_ = 0;
	bufferTextureMaxU_ = 0.0;
	bufferTextureMaxV_ = 0.0;
	bufferTextureWidth_ = 0;
	bufferTextureHeight_ = 0;
	previousBufferTextureFormat_ = 0;
	previousBufferTextureInternalFormat_ = 0;
	currentlyPressedKey_ = Qt::Key(0);

	setAttribute(Qt::WA_NoSystemBackground);

	tileRegion_ = NULL;
}

#if !defined QT3_SUPPORT
/*! Constructor. See \c QGLWidget documentation for details.

All viewer parameters (display flags, scene parameters, associated objects...) are set to their default values. See
the associated documentation.

If the \p shareWidget parameter points to a valid \c QGLWidget, the QGLViewer will share the OpenGL
context with \p shareWidget (see isSharing()). */
QGLViewer::QGLViewer(QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags flags)
	: QGLWidget(parent, shareWidget, flags)
{ defaultConstructor(); }

/*! Same as QGLViewer(), but a \c QGLContext can be provided so that viewers share GL contexts, even
with \c QGLContext sub-classes (use \p shareWidget otherwise). */
QGLViewer::QGLViewer(QGLContext *context, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags flags)
	: QGLWidget(context, parent, shareWidget, flags)
{ defaultConstructor(); }

/*! Same as QGLViewer(), but a specific \c QGLFormat can be provided.

This is for instance needed to ask for a stencil buffer or for stereo display (as is illustrated in
the <a href="../examples/stereoViewer.html">stereoViewer example</a>). */
QGLViewer::QGLViewer(const QGLFormat& format, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags flags)
	: QGLWidget(format, parent, shareWidget, flags)
{ defaultConstructor(); }
#endif // QT3_SUPPORT

/*! Virtual destructor.

The viewer is replaced by \c NULL in the QGLViewerPool() (in order to preserve other viewer's indexes) and allocated
memory is released. The camera() is deleted and should be copied before if it is shared by an other viewer. */
QGLViewer::~QGLViewer()
{
	// See closeEvent comment. Destructor is called (and not closeEvent) only when the widget is embedded.
	// Hence we saveToFile here. It is however a bad idea if virtual domElement() has been overloaded !
	// if (parent())
	// saveStateToFileForAllViewers();

	QGLViewer::QGLViewerPool_.replace(QGLViewer::QGLViewerPool_.indexOf(this), NULL);

	delete camera();
	delete[] selectBuffer_;
	if (helpWidget())
	{
		// Needed for Qt 4 which has no main widget.
		helpWidget()->close();
		delete helpWidget_;
	}
}


static QString QGLViewerVersionString()
{
	return QString::number((QGLVIEWER_VERSION & 0xff0000) >> 16) + "." +
			QString::number((QGLVIEWER_VERSION & 0x00ff00) >> 8) + "." +
			QString::number(QGLVIEWER_VERSION & 0x0000ff);
}

static Qt::KeyboardModifiers keyboardModifiersFromState(unsigned int state) {
	// Convertion of keyboard modifiers and mouse buttons as an int is no longer supported : emulate
	return Qt::KeyboardModifiers(int(state & 0xFF000000));
}


static Qt::MouseButton mouseButtonFromState(unsigned int state) {
	// Convertion of keyboard modifiers and mouse buttons as an int is no longer supported : emulate
	return Qt::MouseButton(state & 0xFFFF);
}

/*! Initializes the QGLViewer OpenGL context and then calls user-defined init().

This method is automatically called once, before the first call to paintGL().

Overload init() instead of this method to modify viewer specific OpenGL state or to create display
lists.

To make beginners' life easier and to simplify the examples, this method slightly modifies the
standard OpenGL state:
\code
glEnable(GL_LIGHT0);
glEnable(GL_LIGHTING);
glEnable(GL_DEPTH_TEST);
glEnable(GL_COLOR_MATERIAL);
\endcode

If you port an existing application to QGLViewer and your display changes, you probably want to
disable these flags in init() to get back to a standard OpenGL state. */
void QGLViewer::initializeGL()
{
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	// Default colors
	setForegroundColor(QColor(180, 180, 180));
	setBackgroundColor(QColor(51, 51, 51));

	// Clear the buffer where we're going to draw
	if (format().stereo())
	{
		glDrawBuffer(GL_BACK_RIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawBuffer(GL_BACK_LEFT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calls user defined method. Default emits a signal.
	init();

	// Give time to glInit to finish and then call setFullScreen().
	if (isFullScreen())
		QTimer::singleShot( 100, this, SLOT(delayedFullScreen()) );
}

/*! Main paint method, inherited from \c QGLWidget.

Calls the following methods, in that order:
\arg preDraw() (or preDrawStereo() if viewer displaysInStereo()) : places the camera in the world coordinate system.
\arg draw() (or fastDraw() when the camera is manipulated) : main drawing method. Should be overloaded.
\arg postDraw() : display of visual hints (world axis, FPS...) */
void QGLViewer::paintGL()
{
	if (displaysInStereo())
	{
		for (int view=1; view>=0; --view)
		{
			// Clears screen, set model view matrix with shifted matrix for ith buffer
			preDrawStereo(view);
			// Used defined method. Default is empty
			if (camera()->frame()->isManipulated())
				fastDraw();
			else
				draw();
			postDraw();
		}
	}
	else
	{
		// Clears screen, set model view matrix...
		preDraw();
		// Used defined method. Default calls draw()
		if (camera()->frame()->isManipulated())
			fastDraw();
		else
			draw();
		// Add visual hints: axis, camera, grid...
		postDraw();
	}
	Q_EMIT drawFinished(true);
}

/*! Sets OpenGL state before draw().

Default behavior clears screen and sets the projection and modelView matrices:
\code
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

camera()->loadProjectionMatrix();
camera()->loadModelViewMatrix();
\endcode

Emits the drawNeeded() signal once this is done (see the <a href="../examples/callback.html">callback example</a>). */
void QGLViewer::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// GL_PROJECTION matrix
	camera()->loadProjectionMatrix();
	// GL_MODELVIEW matrix
	camera()->loadModelViewMatrix();

	Q_EMIT drawNeeded();
}

/*! Called after draw() to draw viewer visual hints.

Default implementation displays axis, grid, FPS... when the respective flags are sets.

See the <a href="../examples/multiSelect.html">multiSelect</a> and <a
href="../examples/contribs.html#thumbnail">thumbnail</a> examples for an overloading illustration.

The GLContext (color, LIGHTING, BLEND...) is \e not modified by this method, so that in
draw(), the user can rely on the OpenGL context he defined. Respect this convention (by pushing/popping the
different attributes) if you overload this method. */
void QGLViewer::postDraw()
{
	// Reset model view matrix to world coordinates origin
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	camera()->loadModelViewMatrix();
	// TODO restore model loadProjectionMatrixStereo

	// Save OpenGL state
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Set neutral GL state
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
#ifdef GL_TEXTURE_3D  // OpenGL 1.2 Only...
	glDisable(GL_TEXTURE_3D);
#endif

	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

#ifdef GL_RESCALE_NORMAL  // OpenGL 1.2 Only...
	glEnable(GL_RESCALE_NORMAL);
#endif

	glDisable(GL_COLOR_MATERIAL);
	qglColor(foregroundColor());

	if (cameraIsEdited())
		camera()->drawAllPaths();

	// Pivot point, line when camera rolls, zoom region
	drawVisualHints();

	if (gridIsDrawn()) { glLineWidth(1.0); drawGrid(camera()->sceneRadius()); }
	if (axisIsDrawn()) { glLineWidth(2.0); drawAxis(camera()->sceneRadius()); }

	// FPS computation
	const unsigned int maxCounter = 20;
	if (++fpsCounter_ == maxCounter)
	{
		f_p_s_ = 1000.0 * maxCounter / fpsTime_.restart();
		fpsString_ = tr("%1Hz", "Frames per seconds, in Hertz").arg(f_p_s_, 0, 'f', ((f_p_s_ < 10.0)?1:0));
		fpsCounter_ = 0;
	}

	// Restore foregroundColor
	float color[4];
	color[0] = foregroundColor().red()   / 255.0f;
	color[1] = foregroundColor().green() / 255.0f;
	color[2] = foregroundColor().blue()  / 255.0f;
	color[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	if (FPSIsDisplayed()) displayFPS();
	if (displayMessage_) drawText(10, height()-10,  message_);

	// Restore GL state
	glPopAttrib();
	glPopMatrix();
}

/*! Called before draw() (instead of preDraw()) when viewer displaysInStereo().

Same as preDraw() except that the glDrawBuffer() is set to \c GL_BACK_LEFT or \c GL_BACK_RIGHT
depending on \p leftBuffer, and it uses qglviewer::Camera::loadProjectionMatrixStereo() and
qglviewer::Camera::loadModelViewMatrixStereo() instead. */
void QGLViewer::preDrawStereo(bool leftBuffer)
{
	// Set buffer to draw in
	// Seems that SGI and Crystal Eyes are not synchronized correctly !
	// That's why we don't draw in the appropriate buffer...
	if (!leftBuffer)
		glDrawBuffer(GL_BACK_LEFT);
	else
		glDrawBuffer(GL_BACK_RIGHT);

	// Clear the buffer where we're going to draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// GL_PROJECTION matrix
	camera()->loadProjectionMatrixStereo(leftBuffer);
	// GL_MODELVIEW matrix
	camera()->loadModelViewMatrixStereo(leftBuffer);

	Q_EMIT drawNeeded();
}

/*! Draws a simplified version of the scene to guarantee interactive camera displacements.

This method is called instead of draw() when the qglviewer::Camera::frame() is
qglviewer::ManipulatedCameraFrame::isManipulated(). Default implementation simply calls draw().

Overload this method if your scene is too complex to allow for interactive camera manipulation. See
the <a href="../examples/fastDraw.html">fastDraw example</a> for an illustration. */
void QGLViewer::fastDraw()
{
	draw();
}

/*! Starts (\p edit = \c true, default) or stops (\p edit=\c false) the edition of the camera().

Current implementation is limited to paths display. Get current state using cameraIsEdited().

\attention This method sets the qglviewer::Camera::zClippingCoefficient() to 5.0 when \p edit is \c
true, so that the Camera paths (see qglviewer::Camera::keyFrameInterpolator()) are not clipped. It
restores the previous value when \p edit is \c false. */
void QGLViewer::setCameraIsEdited(bool edit)
{
	cameraIsEdited_ = edit;
	if (edit)
	{
		previousCameraZClippingCoefficient_ = camera()->zClippingCoefficient();
		// #CONNECTION# 5.0 also used in domElement() and in initFromDOMElement().
		camera()->setZClippingCoefficient(5.0);
	}
	else
		camera()->setZClippingCoefficient(previousCameraZClippingCoefficient_);

	Q_EMIT cameraIsEditedChanged(edit);

	update();
}

// Key bindings. 0 means not defined
void QGLViewer::setDefaultShortcuts()
{
	// D e f a u l t   a c c e l e r a t o r s
	setShortcut(DRAW_AXIS, Qt::Key_A);
	setShortcut(DRAW_GRID, Qt::Key_G);
	setShortcut(DISPLAY_FPS, Qt::Key_F);
	setShortcut(ENABLE_TEXT, Qt::SHIFT+Qt::Key_Question);
	setShortcut(EXIT_VIEWER, Qt::Key_Escape);
	setShortcut(SAVE_SCREENSHOT, Qt::CTRL+Qt::Key_S);
	setShortcut(CAMERA_MODE, Qt::Key_Space);
	setShortcut(FULL_SCREEN, Qt::ALT+Qt::Key_Return);
	setShortcut(STEREO, Qt::Key_S);
	setShortcut(ANIMATION, Qt::Key_Return);
	setShortcut(HELP, Qt::Key_H);
	setShortcut(EDIT_CAMERA, Qt::Key_C);
	setShortcut(MOVE_CAMERA_LEFT, Qt::Key_Left);
	setShortcut(MOVE_CAMERA_RIGHT,Qt::Key_Right);
	setShortcut(MOVE_CAMERA_UP, Qt::Key_Up);
	setShortcut(MOVE_CAMERA_DOWN, Qt::Key_Down);
	setShortcut(INCREASE_FLYSPEED, Qt::Key_Plus);
	setShortcut(DECREASE_FLYSPEED, Qt::Key_Minus);
	setShortcut(SNAPSHOT_TO_CLIPBOARD, Qt::CTRL+Qt::Key_C);

	keyboardActionDescription_[DISPLAY_FPS] = tr("Toggles the display of the FPS", "DISPLAY_FPS action description");
	keyboardActionDescription_[SAVE_SCREENSHOT] = tr("Saves a screenshot", "SAVE_SCREENSHOT action description");
	keyboardActionDescription_[FULL_SCREEN] = tr("Toggles full screen display", "FULL_SCREEN action description");
	keyboardActionDescription_[DRAW_AXIS] = tr("Toggles the display of the world axis", "DRAW_AXIS action description");
	keyboardActionDescription_[DRAW_GRID] = tr("Toggles the display of the XY grid", "DRAW_GRID action description");
	keyboardActionDescription_[CAMERA_MODE] = tr("Changes camera mode (observe or fly)", "CAMERA_MODE action description");
	keyboardActionDescription_[STEREO] = tr("Toggles stereo display", "STEREO action description");
	keyboardActionDescription_[HELP] = tr("Opens this help window", "HELP action description");
	keyboardActionDescription_[ANIMATION] = tr("Starts/stops the animation", "ANIMATION action description");
	keyboardActionDescription_[EDIT_CAMERA] = tr("Toggles camera paths display", "EDIT_CAMERA action description"); // TODO change
	keyboardActionDescription_[ENABLE_TEXT] = tr("Toggles the display of the text", "ENABLE_TEXT action description");
	keyboardActionDescription_[EXIT_VIEWER] = tr("Exits program", "EXIT_VIEWER action description");
	keyboardActionDescription_[MOVE_CAMERA_LEFT] = tr("Moves camera left", "MOVE_CAMERA_LEFT action description");
	keyboardActionDescription_[MOVE_CAMERA_RIGHT] = tr("Moves camera right", "MOVE_CAMERA_RIGHT action description");
	keyboardActionDescription_[MOVE_CAMERA_UP] = tr("Moves camera up", "MOVE_CAMERA_UP action description");
	keyboardActionDescription_[MOVE_CAMERA_DOWN] = tr("Moves camera down", "MOVE_CAMERA_DOWN action description");
	keyboardActionDescription_[INCREASE_FLYSPEED] = tr("Increases fly speed", "INCREASE_FLYSPEED action description");
	keyboardActionDescription_[DECREASE_FLYSPEED] = tr("Decreases fly speed", "DECREASE_FLYSPEED action description");
	keyboardActionDescription_[SNAPSHOT_TO_CLIPBOARD] = tr("Copies a snapshot to clipboard", "SNAPSHOT_TO_CLIPBOARD action description");

	// K e y f r a m e s   s h o r t c u t   k e y s
	setPathKey(Qt::Key_F1,   1);
	setPathKey(Qt::Key_F2,   2);
	setPathKey(Qt::Key_F3,   3);
	setPathKey(Qt::Key_F4,   4);
	setPathKey(Qt::Key_F5,   5);
	setPathKey(Qt::Key_F6,   6);
	setPathKey(Qt::Key_F7,   7);
	setPathKey(Qt::Key_F8,   8);
	setPathKey(Qt::Key_F9,   9);
	setPathKey(Qt::Key_F10, 10);
	setPathKey(Qt::Key_F11, 11);
	setPathKey(Qt::Key_F12, 12);

	setAddKeyFrameKeyboardModifiers(Qt::AltModifier);
	setPlayPathKeyboardModifiers(Qt::NoModifier);
}

// M o u s e   b e h a v i o r
void QGLViewer::setDefaultMouseBindings()
{
	const Qt::KeyboardModifiers cameraKeyboardModifiers = Qt::NoModifier;
	const Qt::KeyboardModifiers frameKeyboardModifiers = Qt::ControlModifier;

	//#CONNECTION# toggleCameraMode()
	for (int handler=0; handler<2; ++handler)
	{
		MouseHandler mh = (MouseHandler)(handler);
		Qt::KeyboardModifiers modifiers = (mh == FRAME) ? frameKeyboardModifiers : cameraKeyboardModifiers;

		setMouseBinding(modifiers, Qt::LeftButton,  mh, ROTATE);
		setMouseBinding(modifiers, Qt::MidButton,   mh, ZOOM);
		setMouseBinding(modifiers, Qt::RightButton, mh, TRANSLATE);

		setMouseBinding(Qt::Key_R, modifiers, Qt::LeftButton, mh, SCREEN_ROTATE);

		setWheelBinding(modifiers, mh, ZOOM);
	}

	// Z o o m   o n   r e g i o n
	setMouseBinding(Qt::ShiftModifier, Qt::MidButton, CAMERA, ZOOM_ON_REGION);

	// S e l e c t
	setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, SELECT);

	setMouseBinding(Qt::ShiftModifier, Qt::RightButton, RAP_FROM_PIXEL);
	// D o u b l e   c l i c k
	setMouseBinding(Qt::NoModifier, Qt::LeftButton, ALIGN_CAMERA,      true);
	setMouseBinding(Qt::NoModifier, Qt::MidButton,  SHOW_ENTIRE_SCENE, true);
	setMouseBinding(Qt::NoModifier, Qt::RightButton, CENTER_SCENE,     true);

	setMouseBinding(frameKeyboardModifiers, Qt::LeftButton, ALIGN_FRAME, true);
	// middle double click makes no sense for manipulated frame
	setMouseBinding(frameKeyboardModifiers, Qt::RightButton, CENTER_FRAME, true);

	// A c t i o n s   w i t h   k e y   m o d i f i e r s
	setMouseBinding(Qt::Key_Z, Qt::NoModifier, Qt::LeftButton, ZOOM_ON_PIXEL);
	setMouseBinding(Qt::Key_Z, Qt::NoModifier, Qt::RightButton, ZOOM_TO_FIT);

#ifdef Q_OS_MAC
	// Specific Mac bindings for touchpads. Two fingers emulate a wheelEvent which zooms.
	// There is no right button available : make Option key + left emulate the right button.
	// A Control+Left indeed emulates a right click (OS X system configuration), but it does
	// no seem to support dragging.
	// Done at the end to override previous settings.
	const Qt::KeyboardModifiers macKeyboardModifiers = Qt::AltModifier;

	setMouseBinding(macKeyboardModifiers, Qt::LeftButton, CAMERA, TRANSLATE);
	setMouseBinding(macKeyboardModifiers, Qt::LeftButton, CENTER_SCENE, true);
	setMouseBinding(frameKeyboardModifiers | macKeyboardModifiers, Qt::LeftButton, CENTER_FRAME, true);
	setMouseBinding(frameKeyboardModifiers | macKeyboardModifiers, Qt::LeftButton, FRAME, TRANSLATE);
#endif
}

/*! Associates a new qglviewer::Camera to the viewer.

You should only use this method when you derive a new class from qglviewer::Camera and want to use
one of its instances instead of the original class.

It you simply want to save and restore Camera positions, use qglviewer::Camera::addKeyFrameToPath()
and qglviewer::Camera::playPath() instead.

This method silently ignores \c NULL \p camera pointers. The calling method is responsible for deleting
the previous camera pointer in order to prevent memory leaks if needed.

The sceneRadius() and sceneCenter() of \p camera are set to the \e current QGLViewer values.

All the \p camera qglviewer::Camera::keyFrameInterpolator()
qglviewer::KeyFrameInterpolator::interpolated() signals are connected to the viewer update() slot.
The connections with the previous viewer's camera are removed. */
void QGLViewer::setCamera(Camera* const camera)
{
	if (!camera)
		return;

	camera->setSceneRadius(sceneRadius());
	camera->setSceneCenter(sceneCenter());
	camera->setScreenWidthAndHeight(width(), height());

	// Disconnect current camera from this viewer.
	disconnect(this->camera()->frame(), SIGNAL(manipulated()), this, SLOT(update()));
	disconnect(this->camera()->frame(), SIGNAL(spun()), this, SLOT(update()));

	// Connect camera frame to this viewer.
	connect(camera->frame(), SIGNAL(manipulated()), SLOT(update()));
	connect(camera->frame(), SIGNAL(spun()), SLOT(update()));

	connectAllCameraKFIInterpolatedSignals(false);
	camera_ = camera;
	connectAllCameraKFIInterpolatedSignals();

	previousCameraZClippingCoefficient_ = this->camera()->zClippingCoefficient();
}

void QGLViewer::connectAllCameraKFIInterpolatedSignals(bool connection)
{
	for (QMap<unsigned int, KeyFrameInterpolator*>::ConstIterator it = camera()->kfi_.begin(), end=camera()->kfi_.end(); it != end; ++it)
	{
		if (connection)
			connect(camera()->keyFrameInterpolator(it.key()), SIGNAL(interpolated()), SLOT(update()));
		else
			disconnect(camera()->keyFrameInterpolator(it.key()), SIGNAL(interpolated()), this, SLOT(update()));
	}

	if (connection)
		connect(camera()->interpolationKfi_, SIGNAL(interpolated()), SLOT(update()));
	else
		disconnect(camera()->interpolationKfi_, SIGNAL(interpolated()), this, SLOT(update()));
}

/*! Draws a representation of \p light.

Called in draw(), this method is useful to debug or display your light setup. Light drawing depends
on the type of light (point, spot, directional).

The method retrieves the light setup using \c glGetLightfv. Position and define your lights before
calling this method.

Light is drawn using its diffuse color. Disabled lights are not displayed.

Drawing size is proportional to sceneRadius(). Use \p scale to rescale it.

See the <a href="../examples/drawLight.html">drawLight example</a> for an illustration.

\attention You need to enable \c GL_COLOR_MATERIAL before calling this method. \c glColor is set to
the light diffuse color. */
void QGLViewer::drawLight(GLenum light, qreal scale) const
{
	static GLUquadric* quadric = gluNewQuadric();

	const qreal length = sceneRadius() / 5.0 * scale;

	GLboolean lightIsOn;
	glGetBooleanv(light, &lightIsOn);

	if (lightIsOn)
	{
		// All light values are given in eye coordinates
		glPushMatrix();
		glLoadIdentity();

		float color[4];
		glGetLightfv(light, GL_DIFFUSE, color);
		glColor4fv(color);

		float pos[4];
		glGetLightfv(light, GL_POSITION, pos);

		if (pos[3] != 0.0)
		{
			glTranslatef(pos[0]/pos[3], pos[1]/pos[3], pos[2]/pos[3]);

			GLfloat cutOff;
			glGetLightfv(light, GL_SPOT_CUTOFF, &cutOff);
			if (cutOff != 180.0)
			{
				GLfloat dir[4];
				glGetLightfv(light, GL_SPOT_DIRECTION, dir);
				glMultMatrixd(Quaternion(Vec(0,0,1), Vec(dir)).matrix());
				QGLViewer::drawArrow(length);
				gluCylinder(quadric, 0.0, 0.7 * length * sin(cutOff * M_PI / 180.0), 0.7 * length * cos(cutOff * M_PI / 180.0), 12, 1);
			}
			else
				gluSphere(quadric, 0.2*length, 10, 10);
		}
		else
		{
			// Directional light.
			Vec dir(pos[0], pos[1], pos[2]);
			dir.normalize();
			Frame fr=Frame(camera()->cameraCoordinatesOf(4.0 * length * camera()->frame()->inverseTransformOf(dir)),
						   Quaternion(Vec(0,0,-1), dir));
			glMultMatrixd(fr.matrix());
			drawArrow(length);
		}

		glPopMatrix();
	}
}


/*! Draws \p text at position \p x, \p y (expressed in screen coordinates pixels, origin in the
upper left corner of the widget).

The default QApplication::font() is used to render the text when no \p fnt is specified. Use
QApplication::setFont() to define this default font.

You should disable \c GL_LIGHTING and \c GL_DEPTH_TEST before this method so that colors are properly rendered.

This method can be used in conjunction with the qglviewer::Camera::projectedCoordinatesOf()
method to display a text attached to an object. In your draw() method use:
\code
qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(myFrame.position());
drawText((int)screenPos[0], (int)screenPos[1], "My Object");
\endcode
See the <a href="../examples/screenCoordSystem.html">screenCoordSystem example</a> for an illustration.

Text is displayed only when textIsEnabled() (default). This mechanism allows the user to
conveniently remove all the displayed text with a single keyboard shortcut.

See also displayMessage() to drawText() for only a short amount of time.

Use QGLWidget::renderText(x,y,z, text) instead if you want to draw a text located
 at a specific 3D position instead of 2D screen coordinates (fixed size text, facing the camera).

The \c GL_MODELVIEW and \c GL_PROJECTION matrices are not modified by this method.

\attention This method uses display lists to render the characters, with an index that starts at
2000 by default (see the QGLWidget::renderText() documentation). If you use more than 2000 Display
Lists, they may overlap with these. Directly use QGLWidget::renderText() in that case, with a
higher \c listBase parameter (or overload <code>fontDisplayListBase</code>).*/
void QGLViewer::drawText(int x, int y, const QString& text, const QFont& fnt)
{
	if (!textIsEnabled())
		return;

	if (tileRegion_ != NULL) {
		renderText(int((x-tileRegion_->xMin) * width() / (tileRegion_->xMax - tileRegion_->xMin)),
				   int((y-tileRegion_->yMin) * height() / (tileRegion_->yMax - tileRegion_->yMin)), text, scaledFont(fnt));
	} else
		renderText(x, y, text, fnt);
}

/*! Briefly displays a message in the lower left corner of the widget. Convenient to provide
feedback to the user.

\p message is displayed during \p delay milliseconds (default is 2 seconds) using drawText().

This method should not be called in draw(). If you want to display a text in each draw(), use
drawText() instead.

If this method is called when a message is already displayed, the new message replaces the old one.
Use setTextIsEnabled() (default shortcut is '?') to enable or disable text (and hence messages)
display. */
void QGLViewer::displayMessage(const QString& message, int delay)
{
	message_ = message;
	displayMessage_ = true;
	// Was set to single shot in defaultConstructor.
	messageTimer_.start(delay);
	if (textIsEnabled())
		update();
}

void QGLViewer::hideMessage()
{
	displayMessage_ = false;
	if (textIsEnabled())
		update();
}


/*! Displays the averaged currentFPS() frame rate in the upper left corner of the widget.

update() should be called in a loop in order to have a meaningful value (this is the case when
you continuously move the camera using the mouse or when animationIsStarted()).
setAnimationPeriod(0) to make this loop as fast as possible in order to reach and measure the
maximum available frame rate.

When FPSIsDisplayed() is \c true (default is \c false), this method is called by postDraw() to
display the currentFPS(). Use QApplication::setFont() to define the font (see drawText()). */
void QGLViewer::displayFPS()
{
	drawText(10, int(1.5*((QApplication::font().pixelSize()>0)?QApplication::font().pixelSize():QApplication::font().pointSize())), fpsString_);
}

/*! Modify the projection matrix so that drawing can be done directly with 2D screen coordinates.

Once called, the \p x and \p y coordinates passed to \c glVertex are expressed in pixels screen
coordinates. The origin (0,0) is in the upper left corner of the widget by default. This follows
the Qt standards, so that you can directly use the \c pos() provided by for instance \c
QMouseEvent. Set \p upward to \c true to place the origin in the \e lower left corner, thus
following the OpenGL and mathematical standards. It is always possible to switch between the two
representations using \c newY = height() - \c y.

You need to call stopScreenCoordinatesSystem() at the end of the drawing block to restore the
previous camera matrix.

In practice, this method should be used in draw(). It sets an appropriate orthographic projection
matrix and then sets \c glMatrixMode to \c GL_MODELVIEW.

See the <a href="../examples/screenCoordSystem.html">screenCoordSystem</a>, <a
href="../examples/multiSelect.html">multiSelect</a> and <a
href="../examples/contribs.html#backgroundImage">backgroundImage</a> examples for an illustration.

You may want to disable \c GL_LIGHTING, to enable \c GL_LINE_SMOOTH or \c GL_BLEND to draw when
this method is used.

If you want to link 2D drawings to 3D objects, use qglviewer::Camera::projectedCoordinatesOf() to
compute the 2D projection on screen of a 3D point (see the <a
href="../examples/screenCoordSystem.html">screenCoordSystem</a> example). See also drawText().

In this mode, you should use z values that are in the [0.0, 1.0[ range (0.0 corresponding to the
near clipping plane and 1.0 being just beyond the far clipping plane). This interval matches the
values that can be read from the z-buffer. Note that if you use the convenient \c glVertex2i() to
provide coordinates, the implicit 0.0 z coordinate will make your drawings appear \e on \e top of
the rest of the scene. */
void QGLViewer::startScreenCoordinatesSystem(bool upward) const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if (tileRegion_ != NULL)
		if (upward)
			glOrtho(tileRegion_->xMin, tileRegion_->xMax, tileRegion_->yMin, tileRegion_->yMax, 0.0, -1.0);
		else
			glOrtho(tileRegion_->xMin, tileRegion_->xMax, tileRegion_->yMax, tileRegion_->yMin, 0.0, -1.0);
	else
		if (upward)
			glOrtho(0, width(), 0, height(), 0.0, -1.0);
		else
			glOrtho(0, width(), height(), 0, 0.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

/*! Stops the pixel coordinate drawing block started by startScreenCoordinatesSystem().

The \c GL_MODELVIEW and \c GL_PROJECTION matrices modified in
startScreenCoordinatesSystem() are restored. \c glMatrixMode is set to \c GL_MODELVIEW. */
void QGLViewer::stopScreenCoordinatesSystem() const
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

/*! Overloading of the \c QObject method.

If animationIsStarted(), calls animate() and draw(). */
void QGLViewer::timerEvent(QTimerEvent *)
{
	if (animationIsStarted())
	{
		animate();
		update();
	}
}

/*! Starts the animation loop. See animationIsStarted(). */
void QGLViewer::startAnimation()
{
	animationTimerId_ = startTimer(animationPeriod());
	animationStarted_ = true;
}

/*! Stops animation. See animationIsStarted(). */
void QGLViewer::stopAnimation()
{
	animationStarted_ = false;
	if (animationTimerId_ != 0)
		killTimer(animationTimerId_);
}

/*! Overloading of the \c QWidget method.

Saves the viewer state using saveStateToFile() and then calls QGLWidget::closeEvent(). */
void QGLViewer::closeEvent(QCloseEvent *e)
{
	// When the user clicks on the window close (x) button:
	// - If the viewer is a top level window, closeEvent is called and then saves to file.
	// - Otherwise, nothing happen s:(
	// When the user press the EXIT_VIEWER keyboard shortcut:
	// - If the viewer is a top level window, saveStateToFile() is also called
	// - Otherwise, closeEvent is NOT called and keyPressEvent does the job.

	/* After tests:
	E : Embedded widget
	N : Widget created with new
	C : closeEvent called
	D : destructor called

	E	N	C	D
	y	y
	y	n		y
	n	y	y
	n	n	y	y

	closeEvent is called iif the widget is NOT embedded.

	Destructor is called iif the widget is created on the stack
	or if widget (resp. parent if embedded) is created with WDestructiveClose flag.

	closeEvent always before destructor.

	Close using qApp->closeAllWindows or (x) is identical