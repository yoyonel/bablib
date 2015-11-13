#ifndef QGLVIEWER_QGLVIEWER_H
#define QGLVIEWER_QGLVIEWER_H

#include "camera.h"

#include <QMap>
#include <QClipboard>
#include <QTime>

class QTabWidget;

namespace qglviewer {
class MouseGrabber;
class ManipulatedFrame;
class ManipulatedCameraFrame;
}

/*! \brief A versatile 3D OpenGL viewer based on QGLWidget.
\class QGLViewer qglviewer.h QGLViewer/qglviewer.h

It features many classical viewer functionalities, such as a camera trackball, manipulated objects,
snapshot saving and much <a href="../features.html">more</a>. Its main goal is to ease the development
of new 3D applications.

New users should read the <a href="../introduction.html">introduction page</a> to get familiar with
important notions such as sceneRadius(), sceneCenter() and the world coordinate system. Try the
numerous simple <a href="../examples/index.html">examples</a> to discover the possibilities and
understand how it works.

<h3>Usage</h3>

To use a QGLViewer, derive you viewer class from the QGLViewer and overload its draw() virtual
method. See the <a href="../examples/simpleViewer.html">simpleViewer example</a> for an illustration.

An other option is to connect your drawing methods to the signals emitted by the QGLViewer (Qt's
callback mechanism). See the <a href="../examples/callback.html">callback example</a> for a
complete implementation.

\nosubgrouping */
class QGLVIEWER_EXPORT QGLViewer : public QGLWidget
{
	Q_OBJECT

public:
	// Complete implementation is provided so that the constructor is defined with QT3_SUPPORT when .h is included.
	// (Would not be available otherwise since lib is compiled without QT3_SUPPORT).
#ifdef QT3_SUPPORT
	explicit QGLViewer(QWidget* parent=NULL, const char* name=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0)
		: QGLWidget(parent, name, shareWidget, flags)
	{ defaultConstructor(); }

	explicit QGLViewer(const QGLFormat& format, QWidget* parent=0, const char* name=0, const QGLWidget* shareWidget=0,Qt::WindowFlags flags=0)
		: QGLWidget(format, parent, name, shareWidget, flags)
	{ defaultConstructor(); }

	QGLViewer(QGLContext* context, QWidget* parent, const char* name=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0)
		: QGLWidget(context, parent, name, shareWidget, flags) {
		defaultConstructor(); }

#else

	explicit QGLViewer(QWidget* parent=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0);
	explicit QGLViewer(QGLContext *context, QWidget* parent=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0);
	explicit QGLViewer(const QGLFormat& format, QWidget* parent=0, const QGLWidget* shareWidget=0, Qt::WindowFlags flags=0);
#endif

	virtual ~QGLViewer();

	/*! @name Display of visual hints */
	//@{
public:
	/*! Returns \c true if the world axis is drawn by the viewer.

	Set by setAxisIsDrawn() or toggleAxisIsDrawn(). Default value is \c false. */
	bool axisIsDrawn() const { return axisIsDrawn_; }
	/*! Returns \c true if a XY grid is drawn by the viewer.

	Set by setGridIsDrawn() or toggleGridIsDrawn(). Default value is \c false. */
	bool gridIsDrawn() const { return gridIsDrawn_; }
	/*! Returns \c true if the viewer displays the current frame rate (Frames Per Second).

	Use QApplication::setFont() to define the display font (see drawText()).

	Set by setFPSIsDisplayed() or toggleFPSIsDisplayed(). Use currentFPS() to get the current FPS.
	Default value is \c false. */
	bool FPSIsDisplayed() const { return FPSIsDisplayed_; }
	/*! Returns \c true if text display (see drawText()) is enabled.

	Set by setTextIsEnabled() or toggleTextIsEnabled(). This feature conveniently removes all the
	possibly displayed text, cleaning display. Default value is \c true. */
	bool textIsEnabled() const { return textIsEnabled_; }

	/*! Returns \c true if the camera() is being edited in the viewer.

	Set by setCameraIsEdited() or toggleCameraIsEdited(). Default value is \p false.

	The current implementation is limited: the defined camera() paths (see
	qglviewer::Camera::keyFrameInterpolator()) are simply displayed using
	qglviewer::Camera::drawAllPaths(). Actual camera and path edition will be implemented in the
	future. */
	bool cameraIsEdited() const { return cameraIsEdited_; }


public Q_SLOTS:
	/*! Sets the state of axisIsDrawn(). Emits the axisIsDrawnChanged() signal. See also toggleAxisIsDrawn(). */
	void setAxisIsDrawn(bool draw=true) { axisIsDrawn_ = draw; Q_EMIT axisIsDrawnChanged(draw); update(); }
	/*! Sets the state of gridIsDrawn(). Emits the gridIsDrawnChanged() signal. See also toggleGridIsDrawn(). */
	void setGridIsDrawn(bool draw=true) { gridIsDrawn_ = draw; Q_EMIT gridIsDrawnChanged(draw); update(); }
	/*! Sets the state of FPSIsDisplayed(). Emits the FPSIsDisplayedChanged() signal. See also toggleFPSIsDisplayed(). */
	void setFPSIsDisplayed(bool display=true) { FPSIsDisplayed_ = display; Q_EMIT FPSIsDisplayedChanged(display); update(); }
	/*! Sets the state of textIsEnabled(). Emits the textIsEnabledChanged() signal. See also toggleTextIsEnabled(). */
	void setTextIsEnabled(bool enable=true) { textIsEnabled_ = enable; Q_EMIT textIsEnabledChanged(enable); update(); }
	void setCameraIsEdited(bool edit=true);

	/*! Toggles the state of axisIsDrawn(). See also setAxisIsDrawn(). */
	void toggleAxisIsDrawn() { setAxisIsDrawn(!axisIsDrawn()); }
	/*! Toggles the state of gridIsDrawn(). See also setGridIsDrawn(). */
	void toggleGridIsDrawn() { setGridIsDrawn(!gridIsDrawn()); }
	/*! Toggles the state of FPSIsDisplayed(). See also setFPSIsDisplayed(). */
	void toggleFPSIsDisplayed() { setFPSIsDisplayed(!FPSIsDisplayed()); }
	/*! Toggles the state of textIsEnabled(). See also setTextIsEnabled(). */
	void toggleTextIsEnabled() { setTextIsEnabled(!textIsEnabled()); }
	/*! Toggles the state of cameraIsEdited(). See also setCameraIsEdited(). */
	void toggleCameraIsEdited() { setCameraIsEdited(!cameraIsEdited()); }
	//@}


	/*! @name Viewer's colors */
	//@{
public:
	/*! Returns the background color of the viewer.

	This method is provided for convenience since the background color is an OpenGL state variable
	set with \c glClearColor(). However, this internal representation has the advantage that it is
	saved (resp. restored) with saveStateToFile() (resp. restoreStateFromFile()).

	Use setBackgroundColor() to define and activate a background color.

	\attention Each QColor component is an integer ranging from 0 to 255. This differs from the qreal
	values used by \c glClearColor() which are in the 0.0-1.0 range. Default value is (51, 51, 51)
	(dark gray). You may have to change foregroundColor() accordingly.

	\attention This method does not return the current OpenGL clear color as \c glGet() does. Instead,
	it returns the QGLViewer internal variable. If you directly use \c glClearColor() or \c
	qglClearColor() instead of setBackgroundColor(), the two results will differ. */
	QColor backgroundColor() const { return backgroundColor_; }

	/*! Returns the foreground color used by the viewer.

	This color is used when FPSIsDisplayed(), gridIsDrawn(), to display the camera paths when the
	cameraIsEdited().

	\attention Each QColor component is an integer in the range 0-255. This differs from the qreal
	values used by \c glColor3f() which are in the range 0-1. Default value is (180, 180, 180) (light
	gray).

	Use \c qglColor(foregroundColor()) to set the current OpenGL color to the foregroundColor().

	See also backgroundColor(). */
	QColor foregroundColor() const { return foregroundColor_; }
public Q_SLOTS:
	/*! Sets the backgroundColor() of the viewer and calls \c qglClearColor(). See also
		setForegroundColor(). */
	void setBackgroundColor(const QColor& color) { backgroundColor_=color; qglClearColor(color); }
	/*! Sets the foregroundColor() of the viewer, used to draw visual hints. See also setBackgroundColor(). */
	void setForegroundColor(const QColor& color) { foregroundColor_ = color; }
	//@}


	/*! @name Scene dimensions */
	//@{
public:
	/*! Returns the scene radius.

	The entire displayed scene should be included in a sphere of radius sceneRadius(), centered on
	sceneCenter().

	This approximate value is used by the camera() to set qglviewer::Camera::zNear() and
	qglviewer::Camera::zFar(). It is also used to showEntireScene() or to scale the world axis
	display..

	Default value is 1.0. This method is equivalent to camera()->sceneRadius(). See
	setSceneRadius(). */
	qreal sceneRadius() const { return camera()->sceneRadius(); }
	/*! Returns the scene center, defined in world coordinates.

	See sceneRadius() for details.

	Default value is (0,0,0). Simply a wrapper for camera()->sceneCenter(). Set using
	setSceneCenter().

	Do not mismatch this value (that only depends on the scene) with the qglviewer::Camera::pivotPoint(). */
	qglviewer::Vec sceneCenter() const { return camera()->sceneCenter(); }

public Q_SLOTS:
	/*! Sets the sceneRadius().

		The camera() qglviewer::Camera::flySpeed() is set to 1% of this value by this method. Simple
		wrapper around camera()->setSceneRadius(). */
	virtual void setSceneRadius(qreal radius) { camera()->setSceneRadius(radius); }

	/*! Sets the sceneCenter(), defined in world coordinates.

		\attention The qglviewer::Camera::pivotPoint() is set to the sceneCenter() value by this
		method. */
	virtual void setSceneCenter(const qglviewer::Vec& center) { camera()->setSceneCenter(center); }

	/*! Convenient way to call setSceneCenter() and setSceneRadius() from a (world axis aligned) bounding box of the scene.

		This is equivalent to:
		\code
		setSceneCenter((min+max) / 2.0);
		setSceneRadius((max-min).norm() / 2.0);
		\endcode */
	void setSceneBoundingBox(const qglviewer::Vec& min, const qglviewer::Vec& max) { camera()->setSceneBoundingBox(min,max); }

	/*! Moves the camera so that the entire scene is visible.

		Simple wrapper around qglviewer::Camera::showEntireScene(). */
	void showEntireScene() { camera()->showEntireScene(); update(); }
	//@}


	/*! @name Associated objects */
	//@{
public:
	/*! Returns the associated qglviewer::Camera, never \c NULL. */
	qglviewer::Camera* camera() const { return camera_; }

	/*! Returns the viewer's qglviewer::ManipulatedFrame.

	This qglviewer::ManipulatedFrame can be moved with the mouse when the associated mouse bindings
	are used (default is when pressing the \c Control key with any mouse button). Use
	setMouseBinding() to define new bindings.

	See the <a href="../examples/manipulatedFrame.html">manipulatedFrame example</a> for a complete
	implementation.

	Default value is \c NULL, meaning that no qglviewer::ManipulatedFrame is set. */
	qglviewer::ManipulatedFrame* manipulatedFrame() const { return manipulatedFrame_; }

public Q_SLOTS:
	void setCamera(qglviewer::Camera* const camera);
	void setManipulatedFrame(qglviewer::ManipulatedFrame* frame);
	//@}


	/*! @name Mouse grabbers */
	//@{
public:
	/*! Returns the current qglviewer::MouseGrabber, or \c NULL if no qglviewer::MouseGrabber
	currently grabs mouse events.

	When qglviewer::MouseGrabber::grabsMouse(), the different mouse events are sent to the
	mouseGrabber() instead of their usual targets (camera() or manipulatedFrame()).

	See the qglviewer::MouseGrabber documentation for details on MouseGrabber's mode of operation.

	In order to use MouseGrabbers, you need to enable mouse tracking (so that mouseMoveEvent() is
	called even when no mouse button is pressed). Add this line in init() or in your viewer
	constructor:
	\code
	setMouseTracking(true);
	\endcode
	Note that mouse tracking is disabled by default. Use QWidget::hasMouseTracking() to
	retrieve current state. */
	qglviewer::MouseGrabber* mouseGrabber() const { return mouseGrabber_; }

	void setMouseGrabberIsEnabled(const qglviewer::MouseGrabber* const mouseGrabber, bool enabled=true);
	/*! Returns \c true if \p mouseGrabber is enabled.

	Default value is \c true for all MouseGrabbers. When set to \c false using
	setMouseGrabberIsEnabled(), the specified \p mouseGrabber will never become the mouseGrabber() of
	this QGLViewer. This is useful when you use several viewers: some MouseGrabbers may only have a
	meaning for some specific viewers and should not be selectable in others.

	You can also use qglviewer::MouseGrabber::removeFromMouseGrabberPool() to completely disable a
	MouseGrabber in all the QGLViewers. */
	bool mouseGrabberIsEnabled(const qglviewer::MouseGrabber* const mouseGrabber) { return !disabledMouseGrabbers_.contains(reinterpret_cast<size_t>(mouseGrabber)); }
public Q_SLOTS:
	void setMouseGrabber(qglviewer::MouseGrabber* mouseGrabber);
	//@}


	/*! @name State of the viewer */
	//@{
public:
	/*! Returns the aspect ratio of the viewer's widget (width() / height()). */
	qreal aspectRatio() const { return width() / static_cast<qreal>(height()); }
	/*! Returns the current averaged viewer frame rate.

	This value is computed and averaged over 20 successive frames. It only changes every 20 draw()
	(previously computed value is otherwise returned).

	This method is useful for true real-time applications that may adapt their computational load
	accordingly in order to maintain a given frequency.

	This value is meaningful only when draw() is regularly called, either using a \c QTimer, when
	animationIsStarted() or when the camera is manipulated with the mouse.  */
	qreal currentFPS() { return f_p_s_; }
	/*! Returns \c true if the viewer is in fullScreen mode.

	Default value is \c false. Set by setFullScreen() or toggleFullScreen().

	Note that if the QGLViewer is embedded in an other QWidget, it returns \c true when the top level
	widget is in full screen mode. */
	bool isFullScreen() const { return fullScreen_; }
	/*! Returns \c true if the viewer displays in stereo.

	The QGLViewer object must be created with a stereo format to handle stereovision:
	\code
	QGLFormat format;
	format.setStereoDisplay( TRUE );
	QGLViewer viewer(format);
	\endcode
	The hardware needs to support stereo display. Try the <a
	href="../examples/stereoViewer.html">stereoViewer example</a> to check.

	Set by setStereoDisplay() or toggleStereoDisplay(). Default value is \c false.

	Stereo is performed using the Parallel axis asymmetric frustum perspective projection method.
	See Camera::loadProjectionMatrixStereo() and Camera::loadModelViewMatrixStereo().

	The stereo parameters are defined by the camera(). See qglviewer::Camera::setIODistance(),
	qglviewer::Camera::setPhysicalScreenWidth() and
	qglviewer::Camera::setFocusDistance(). */
	bool displaysInStereo() const { return stereo_; }
	/*! Returns the recommended size for the QGLViewer. Default value is 600x400 pixels. */
	virtual QSize sizeHint() const { return QSize(600, 400); }

public Q_SLOTS:
	void setFullScreen(bool fullScreen=true);
	void setStereoDisplay(bool stereo=true);
	/*! Toggles the state of isFullScreen(). See also setFullScreen(). */
	void toggleFullScreen() { setFullScreen(!isFullScreen()); }
	/*! Toggles the state of displaysInStereo(). See setStereoDisplay(). */
	void toggleStereoDisplay() { setStereoDisplay(!stereo_); }
	void toggleCameraMode();

private:
	bool cameraIsInRotateMode() const;
	//@}


	/*! @name Display methods */
	//@{
public:
	static void drawArrow(qreal length=1.0, qreal radius=-1.0, int nbSubdivisions=12);
	static void drawArrow(const qglviewer::Vec& from, const qglviewer::Vec& to, qreal radius=-1.0, int nbSubdivisions=12);
	static void drawAxis(qreal length=1.0);
	static void drawGrid(qreal size=1.0, int nbSubdivisions=10);

	virtual void startScreenCoordinatesSystem(bool upward=false) const;
	virtual void stopScreenCoordinatesSystem() const;

	void drawText(int x, int y, const QString& text, const QFont& fnt=QFont());
	void displayMessage(const QString& message, int delay=2000);
	// void draw3DText(const qglviewer::Vec& pos, const qglviewer::Vec& normal, const QString& string, GLfloat height=0.1);

protected:
	virtual void drawLight(GLenum light, qreal scale = 1.0) const;

private:
	void displayFPS();
	/*! Vectorial rendering callback method. */
	void drawVectorial() { paintGL(); }

#ifndef DOXYGEN
	friend void drawVectorial(void* param);
#endif
	//@}


#ifdef DOXYGEN
	/*! @name Useful inherited methods */
	//@{
public:
	/*! Returns viewer's widget width (in pixels). See QGLWidget documentation. */
	int width() const;
	/*! Returns viewer's widget height (in pixels). See QGLWidget documentation. */
	int height() const;
	/*! Updates the display. Do not call draw() directly, use this method instead. See QGLWidget documentation. */
	virtual void updateGL();
	/*! Converts \p image into the unnamed format expected by OpenGL methods such as glTexImage2D().
	See QGLWidget documentation. */
	static QImage convertToGLFormat(const QImage & image);
	/*! Calls \c glColor3. See QGLWidget::qglColor(). */
	void qglColor(const QColor& color) const;
	/*! Calls \c glClearColor. See QGLWidget documentation. */
	void qglClearColor(const QColor& color) const;
	/*! Returns \c true if the widget has a valid GL rendering context. See QGLWidget
	documentation. */
	bool isValid() const;
	/*! Returns \c true if display list sharing with another QGLWidget was requested in the
	constructor. See QGLWidget documentation. */
	bool isSharing() const;
	/*! Makes this widget's rendering context the current OpenGL rendering context. Useful with
	several viewers. See QGLWidget documentation. */
	virtual void makeCurrent();
	/*! Returns \c true if mouseMoveEvent() is called even when no mouse button is pressed.

	You need to setMouseTracking() to \c true in order to use MouseGrabber (see mouseGrabber()). See
	details in the QWidget documentation. */
	bool hasMouseTracking () const;
public Q_SLOTS:
	/*! Resizes the widget to size \p width by \p height pixels. See also width() and height(). */
	virtual void resize(int width, int height);
	/*! Sets the hasMouseTracking() value. */
	virtual void setMouseTracking(bool enable);
protected:
	/*! Returns \c true when buffers are automatically swapped (default). See details in the QGLWidget
	documentation. */
	bool autoBufferSwap() const;
protected Q_SLOTS:
	/*! Sets the autoBufferSwap() value. */
	void setAutoBufferSwap(bool on);
	//@}
#endif


	/*! @name Snapshots */
	//@{
public:
	/*! Returns the snapshot file name used by saveSnapshot().

	This value is used in \p automatic mode (see saveSnapshot()). A dialog is otherwise popped-up to
	set it.

	You can also directly provide a file name using saveSnapshot(const QString&, bool).

	If the file name is relative, the current working directory at the moment of the method call is
	used. Set using setSnapshotFileName(). */
	const QString& snapshotFileName() const { return snapshotFileName_; }
#ifndef DOXYGEN
	const QString& snapshotFilename() const;
#endif
	/*! Returns the snapshot file format used by saveSnapshot().

	This value is used when saveSnapshot() is passed the \p automatic flag. It is defined using a
	saveAs pop-up dialog otherwise.

	The available formats are those handled by Qt. Classical values are \c "JPEG", \c "PNG",
	\c "PPM", \c "BMP". Use the following code to get the actual list:
	\code
	QList<QByteArray> formatList = QImageReader::supportedImageFormats();
	// or with Qt version 2 or 3:
	QStringList formatList = QImage::outputFormatList();
	\endcode

	If the library was compiled with the vectorial rendering option (default), three additional
	vectorial formats are available: \c "EPS", \c "PS" and \c "XFIG". \c "SVG" and \c "PDF" formats
	should soon be available. The <a href="http://artis.imag.fr/Software/VRender">VRender library</a>
	was created by Cyril Soler.

	Note that the VRender library has some limitations: vertex shader effects are not reproduced and
	\c PASS_THROUGH tokens are not handled so one can not change point and line size in the middle of
	a drawing.

	Default value is the first supported among "JPEG, PNG, EPS, PS, PPM, BMP", in that order.

	This value is set using setSnapshotFormat() or with openSnapshotFormatDialog().

	\attention No verification is performed on the provided format validity. The next call to
	saveSnapshot() may fail if the format string is not supported. */
	const QString& snapshotFormat() const { return snapshotFormat_; }
	/*! Returns the value of the counter used to name snapshots in saveSnapshot() when \p automatic is
	\c true.

	Set using setSnapshotCounter(). Default value is 0, and it is incremented after each \p automatic
	snapshot. See saveSnapshot() for details. */
	int snapshotCounter() const { return snapshotCounter_; }
	/*! Defines the image quality of the snapshots produced with saveSnapshot().

	Values must be in the range -1..100. Use 0 for lowest quality and 100 for highest quality (and
	larger files). -1 means use Qt default quality. Default value is 95.

	Set using setSnapshotQuality(). See also the QImage::save() documentation.

	\note This value has no impact on the images produced in vectorial format. */
	int snapshotQuality() { return snapshotQuality_; }

	// Qt 2.3 does not support qreal default value parameters in slots.
	// Remove "Q_SLOTS" from the following line to compile with Qt 2.3
public Q_SLOTS:
	void saveSnapshot(bool automatic=true, bool overwrite=false);

public Q_SLOTS:
	void saveSnapshot(const QString& fileName, bool overwrite=false);
	void setSnapshotFileName(const QString& name);

	/*! Sets the snapshotFormat(). */
	void setSnapshotFormat(const QString& format) { snapshotFormat_ = format; }
	/*! Sets the snapshotCounter(). */
	void setSnapshotCounter(int counter) { snapshotCounter_ = counter; }
	/*! Sets the snapshotQuality(). */
	void setSnapshotQuality(int quality) { snapshotQuality_ = quality; }
	bool openSnapshotFormatDialog();
	void snapshotToClipboard();

private:
	bool saveImageSnapshot(const QString& fileName);

#ifndef DOXYGEN
	/* This class is used internally for screenshot that require tiling (image size size different
	from window size). Only in that case, is the private tileRegion_ pointer non null.
	It then contains the current tiled region, which is used by startScreenCoordinatesSystem
	to adapt the coordinate system. Not using it would result in a tiled drawing of the parts
	that use startScreenCoordinatesSystem. Also used by scaledFont for same purposes. */
	class TileRegion { public : qreal xMin, yMin, xMax, yMax, textScale; };
#endif

public:
	/*! Return a possibly scaled version of \p font, used for snapshot rendering.

	From a user's point of view, this method simply returns \p font and can be used transparently.

	However when internally rendering a screen snapshot using saveSnapshot(), it returns a scaled version
	of the font, so that the size of the rendered text on the snapshot is identical to what is displayed on screen,
	even if the snapshot uses image tiling to create an image of dimensions different from those of the
	current window. This scaled version will only be used when saveSnapshot() calls your draw() method
	to generate the snapshot.

	All your calls to QGLWidget::renderText() function hence should use this method.
	\code
	renderText(x, y, z, "My Text", scaledFont(QFont()));
	\endcode
	will guarantee that this text will be properly displayed on arbitrary sized snapshots.

	Note that this method is not needed if you use drawText() which already calls it internally. */
	QFont scaledFont(const QFont& font) const {
		if (tileRegion_ == NULL)
			return font;
		else {
			QFont f(font);
			if (f.pixelSize() == -1)
				f.setPointSizeF(f.pointSizeF() * tileRegion_->textScale);
			else
				f.setPixelSize(int(f.pixelSize() * tileRegion_->textScale));
			return f;
		}
	}
	//@}


	/*! @name Buffer to texture */
	//@{
public:
	GLuint bufferTextureId() const;
	/*! Returns the texture coordinate corresponding to the u extremum of the bufferTexture.

	The bufferTexture is created by copyBufferToTexture(). The texture size has powers of two
	dimensions and the buffer image hence only fills a part of it. This value corresponds to the u
	coordinate of the extremum right side of the buffer image.

	Use (0,0) to (bufferTextureMaxU(), bufferTextureMaxV()) texture coordinates to map the entire
	texture on a quad. */
	qreal bufferTextureMaxU() const { return bufferTextureMaxU_; }
	/*! Same as bufferTextureMaxU(), but for the v texture coordinate. */
	qreal bufferTextureMaxV() const { return bufferTextureMaxV_; }
public Q_SLOTS:
	void copyBufferToTexture(GLint internalFormat, GLenum format=GL_NONE);
	//@}

	/*! @name Animation */
	//@{
public:
	/*! Return \c true when the animation loop is started.

	During animation, an infinite loop calls animate() and draw() and then waits for animationPeriod()
	milliseconds before calling animate() and draw() again. And again.

	Use startAnimation(), stopAnimation() or toggleAnimation() to change this value.

	See the <a href="../examples/animation.html">animation example</a> for illustration. */
	bool animationIsStarted() const { return animationStarted_; }
	/*! The animation loop period, in milliseconds.

	When animationIsStarted(), this is delay waited after draw() to call animate() and draw() again.
	Default value is 40 milliseconds (25 Hz).

	This value will define the currentFPS() when animationIsStarted() (provided that your animate()
	and draw() methods are fast enough).

	If you want to know the maximum possible frame rate of your machine on a given scene,
	setAnimationPeriod() to \c 0, and startAnimation() (keyboard shortcut is \c Enter). The display
	will then be updated as often as possible, and the frame rate will be meaningful.

	\note This value is taken into account only the next time you call startAnimation(). If
	animationIsStarted(), you should stopAnimation() first. */
	int animationPeriod() const { return animationPeriod_; }

public Q_SLOTS:
	/*! Sets the animationPeriod(), in milliseconds. */
	void setAnimationPeriod(int period) { animationPeriod_ = period; }
	virtual void startAnimation();
	virtual void stopAnimation();
	/*! Scene animation method.

		When animationIsStarted(), this method is in charge of the scene update before each draw().
		Overload it to define how your scene evolves over time. The time should either be regularly
		incremented in this method (frame-rate independent animation) or computed from actual time (for
		instance using QTime::elapsed()) for real-time animations.

				Note that KeyFrameInterpolator (which regularly updates a Frame) does not use this method
				to animate a Frame, but rather rely on a QTimer signal-slot mechanism.

		See the <a href="../examples/animation.html">animation example</a> for an illustration. */
	virtual void animate() { Q_EMIT animateNeeded(); }
	/*! Calls startAnimation() or stopAnimation(), depending on animationIsStarted(). */
	void toggleAnimation() { if (animationIsStarted()) stopAnimation(); else startAnimation(); }
	//@}

public:
Q_SIGNALS:
	/*! Signal emitted by the default init() method.

	Connect this signal to the methods that need to be called to initialize your viewer or overload init(). */
	void viewerInitialized();

	/*! Signal emitted by the default draw() method.

	Connect this signal to your main drawing method or overload draw(). See the <a
	href="../examples/callback.html">callback example</a> for an illustration. */
	void drawNeeded();

	/*! Signal emitted at the end of the QGLViewer::paintGL() method, when frame is drawn.

	Can be used to notify an image grabbing process that the image is ready. A typical example is to
	connect this signal to the saveSnapshot() method, so that a (numbered) snapshot is generated after
	each new display, in order to create a movie:
	\code
	connect(viewer, SIGNAL(drawFinished(bool)), SLOT(saveSnapshot(bool)));
	\endcode

	The \p automatic bool variable is always \c true and has been added so that the signal can be
	connected to saveSnapshot() with an \c automatic value set to \c true. */
	void drawFinished(bool automatic);

	/*! Signal emitted by the default animate() method.

	Connect this signal to your scene animation method or overload animate(). */
	void animateNeeded();

	/*! Signal emitted by the default QGLViewer::help() method.

	Connect this signal to your own help method or overload help(). */
	void helpRequired();

	/*! This signal is emitted whenever axisIsDrawn() changes value. */
	void axisIsDrawnChanged(bool drawn);
	/*! This signal is emitted whenever gridIsDrawn() changes value. */
	void gridIsDrawnChanged(bool drawn);
	/*! This signal is emitted whenever FPSIsDisplayed() changes value. */
	void FPSIsDisplayedChanged(bool displayed);
	/*! This signal is emitted whenever textIsEnabled() changes value. */
	void textIsEnabledChanged(bool enabled);
	/*! This signal is emitted whenever cameraIsEdited() changes value.. */
	void cameraIsEditedChanged(bool edited);
	/*! This signal is emitted whenever displaysInStereo() changes value. */
	void stereoChanged(bool on);
	/*! Signal emitted by select().

	Connect this signal to your selection method or overload select(), or more probably simply
	drawWithNames(). */
	void pointSelected(const QMouseEvent* e);

	/*! Signal emitted by setMouseGrabber() when the mouseGrabber() is changed.

	\p mouseGrabber is a pointer to the new MouseGrabber. Note that this signal is emitted with a \c
	NULL parameter each time a MouseGrabber stops grabbing mouse. */
	void mouseGrabberChanged(qglviewer::MouseGrabber* mouseGrabber);

	/*! @name Help window */
	//@{
public:
	/*! Returns the QString displayed in the help() window main tab.

	Overload this method to define your own help string, which should shortly describe your
	application and explain how it works. Rich-text (HTML) tags can be used (see QStyleSheet()
	documentation for available tags):
	\code
	QString myViewer::helpString() const
	{
	QString text("<h2>M y V i e w e r</h2>");
	text += "Displays a <b>Scene</b> using OpenGL. Move the camera using the mouse.";
	return text;
	}
	\endcode

	See also mouseString() and keyboardString(). */
	virtual QString helpString() const { return tr("No help available."); }

	virtual QString mouseString() const;
	virtual QString keyboardString() const;

#ifndef DOXYGEN
	/*! This method is deprecated, use mouseString() instead. */
	virtual QString mouseBindingsString () const { return mouseString(); }
	/*! This method is deprecated, use keyboardString() instead. */
	virtual QString shortcutBindingsString () const { return keyboardString(); }
#endif

public Q_SLOTS:
	virtual void help();
	virtual void aboutQGLViewer();

protected:
	/*! Returns a pointer to the help widget.

	Use this only if you want to directly modify the help widget. Otherwise use helpString(),
	setKeyDescription() and setMouseBindingDescription() to customize the text displayed in the help
	window tabs. */
	QTabWidget* helpWidget() { return helpWidget_; }
	//@}


	/*! @name Drawing methods */
	//@{
protected:
	virtual void resizeGL(int width, int height);
	virtual void initializeGL();

	/*! Initializes the viewer OpenGL context.

	This method is called before the first drawing and should be overloaded to initialize some of the
	OpenGL flags. The default implementation is empty. See initializeGL().

	Typical usage include camera() initialization (showEntireScene()), previous viewer state
	restoration (restoreStateFromFile()), OpenGL state modification and display list creation.

	Note that initializeGL() modifies the standard OpenGL context. These values can be restored back
	in this method.

	\attention You should not call updateGL() (or any method that calls it) in this method, as it will
	result in an infinite loop. The different QGLViewer set methods (setAxisIsDrawn(),
	setFPSIsDisplayed()...) are protected against this problem and can safely be called.

	\note All the OpenGL specific initializations must be done in this method: the OpenGL context is
	not yet available in your viewer constructor. */
	virtual void init() { Q_EMIT viewerInitialized(); }

	virtual void paintGL();
	virtual void preDraw();
	virtual void preDrawStereo(bool leftBuffer=true);

	/*! The core method of the viewer, that draws the scene.

	If you build a class that inherits from QGLViewer, this is the method you want to overload. See
	the <a href="../examples/simpleViewer.html">simpleViewer example</a> for an illustration.

	The camera modelView matrix set in preDraw() converts from the world to the camera coordinate
	systems. Vertices given in draw() can then be considered as being given in the world coordinate
	system. The camera is moved in this world using the mouse. This representation is much more
	intuitive than the default camera-centric OpenGL standard.

	\attention The \c GL_PROJECTION matrix should not be modified by this method, to correctly display
	visual hints (axis, grid, FPS...) in postDraw(). Use push/pop or call
	camera()->loadProjectionMatrix() at the end of draw() if you need to change the projection matrix
	(unlikely). On t