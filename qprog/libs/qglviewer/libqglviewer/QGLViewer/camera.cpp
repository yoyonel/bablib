#include "domUtils.h"
#include "camera.h"
#include "qglviewer.h"
#include "manipulatedCameraFrame.h"

using namespace std;
using namespace qglviewer;

/*! Default constructor.

 sceneCenter() is set to (0,0,0) and sceneRadius() is set to 1.0. type() is Camera::PERSPECTIVE,
 with a \c M_PI/4 fieldOfView().

 See IODistance(), physicalDistanceToScreen(), physicalScreenWidth() and focusDistance()
 documentations for default stereo parameter values. */
Camera::Camera()
	: frame_(NULL), fieldOfView_(M_PI/4.0), modelViewMatrixIsUpToDate_(false), projectionMatrixIsUpToDate_(false)
{
	// #CONNECTION# Camera copy constructor
	interpolationKfi_ = new KeyFrameInterpolator;
	// Requires the interpolationKfi_
	setFrame(new ManipulatedCameraFrame());

	// #CONNECTION# All these default values identical in initFromDOMElement.

	// Requires fieldOfView() to define focusDistance()
	setSceneRadius(1.0);

	// Initial value (only scaled after this)
	orthoCoef_ = tan(fieldOfView()/2.0);

	// Also defines the pivotPoint(), which changes orthoCoef_. Requires a frame().
	setSceneCenter(Vec(0.0, 0.0, 0.0));

	// Requires fieldOfView() when called with ORTHOGRAPHIC. Attention to projectionMatrix_ below.
	setType(PERSPECTIVE);

	// #CONNECTION# initFromDOMElement default values
	setZNearCoefficient(0.005);
	setZClippingCoefficient(sqrt(3.0));

	// Dummy values
	setScreenWidthAndHeight(600, 400);

	// Stereo parameters
	setIODistance(0.062);
	setPhysicalScreenWidth(0.5);
	// focusDistance is set from setFieldOfView()

	// #CONNECTION# Camera copy constructor
	for (unsigned short j=0; j<16; ++j)
	{
		modelViewMatrix_[j] = ((j%5 == 0) ? 1.0 : 0.0);
		// #CONNECTION# computeProjectionMatrix() is lazy and assumes 0.0 almost everywhere.
		projectionMatrix_[j] = 0.0;
	}
	computeProjectionMatrix();
}

/*! Virtual destructor.

 The frame() is deleted, but the different keyFrameInterpolator() are \e not deleted (in case they
 are shared). */
Camera::~Camera()
{
	delete frame_;
	delete interpolationKfi_;
}


/*! Copy constructor. Performs a deep copy using operator=(). */
Camera::Camera(const Camera& camera)
	: QObject(), frame_(NULL)
{
	// #CONNECTION# Camera constructor
	interpolationKfi_ = new KeyFrameInterpolator;
	// Requires the interpolationKfi_
	setFrame(new ManipulatedCameraFrame(*camera.frame()));

	for (unsigned short j=0; j<16; ++j)
	{
		modelViewMatrix_[j] = ((j%5 == 0) ? 1.0 : 0.0);
		// #CONNECTION# computeProjectionMatrix() is lazy and assumes 0.0 almost everywhere.
		projectionMatrix_[j] = 0.0;
	}

	(*this)=camera;
}

/*! Equal operator.

 All the parameters of \p camera are copied. The frame() pointer is not modified, but its
 Frame::position() and Frame::orientation() are set to those of \p camera.

 \attention The Camera screenWidth() and screenHeight() are set to those of \p camera. If your
 Camera is associated with a QGLViewer, you should update these value after the call to this method:
 \code
 *(camera()) = otherCamera;
 camera()->setScreenWidthAndHeight(width(), height());
 \endcode
 The same applies to sceneCenter() and sceneRadius(), if needed. */
Camera& Camera::operator=(const Camera& camera)
{
	setScreenWidthAndHeight(camera.screenWidth(), camera.screenHeight());
	setFieldOfView(camera.fieldOfView());
	setSceneRadius(camera.sceneRadius());
	setSceneCenter(camera.sceneCenter());
	setZNearCoefficient(camera.zNearCoefficient());
	setZClippingCoefficient(camera.zClippingCoefficient());
	setType(camera.type());

	// Stereo parameters
	setIODistance(camera.IODistance());
	setFocusDistance(camera.focusDistance());
	setPhysicalScreenWidth(camera.physicalScreenWidth());

	orthoCoef_ = camera.orthoCoef_;
	projectionMatrixIsUpToDate_ = false;

	// frame_ and interpolationKfi_ pointers are not shared.
	frame_->setReferenceFrame(NULL);
	frame_->setPosition(camera.position());
	frame_->setOrientation(camera.orientation());

	interpolationKfi_->resetInterpolation();

	kfi_ = camera.kfi_;

	computeProjectionMatrix();
	computeModelViewMatrix();

	return *this;
}

/*! Sets Camera screenWidth() and screenHeight() (expressed in pixels).

You should not call this method when the Camera is associated with a QGLViewer, since the
latter automatically updates these values when it is resized (hence overwritting your values).

Non-positive dimension are silently replaced by a 1 pixel value to ensure frustrum coherence.

If your Camera is used without a QGLViewer (offscreen rendering, shadow maps), use setAspectRatio()
instead to define the projection matrix. */
void Camera::setScreenWidthAndHeight(int width, int height)
{
	// Prevent negative and zero dimensions that would cause divisions by zero.
	screenWidth_  = width > 0 ? width : 1;
	screenHeight_ = height > 0 ? height : 1;
	projectionMatrixIsUpToDate_ = false;
}

/*! Returns the near clipping plane distance used by the Camera projection matrix.

 The clipping planes' positions depend on the sceneRadius() and sceneCenter() rather than being fixed
 small-enough and large-enough values. A good scene dimension approximation will hence result in an
 optimal precision of the z-buffer.

 The near clipping plane is positioned at a distance equal to zClippingCoefficient() * sceneRadius()
 in front of the sceneCenter():
 \code
 zNear = distanceToSceneCenter() - zClippingCoefficient()*sceneRadius();
 \endcode

 In order to prevent negative or too small zNear() values (which would degrade the z precision),
 zNearCoefficient() is used when the Camera is inside the sceneRadius() sphere:
 \code
 const qreal zMin = zNearCoefficient() * zClippingCoefficient() * sceneRadius();
 if (zNear < zMin)
   zNear = zMin;
 // With an ORTHOGRAPHIC type, the value is simply clamped to 0.0
 \endcode

 See also the zFar(), zClippingCoefficient() and zNearCoefficient() documentations.

 If you need a completely different zNear computation, overload the zNear() and zFar() methods in a
 new class that publicly inherits from Camera and use QGLViewer::setCamera():
 \code
 class myCamera :: public qglviewer::Camera
 {
   virtual qreal Camera::zNear() const { return 0.001; };
   virtual qreal Camera::zFar() const { return 100.0; };
 }
 \endcode

 See the <a href="../examples/standardCamera.html">standardCamera example</a> for an application.

 \attention The value is always positive although the clipping plane is positioned at a negative z
 value in the Camera coordinate system. This follows the \c gluPerspective standard. */
qreal Camera::zNear() const
{
	const qreal zNearScene = zClippingCoefficient() * sceneRadius();
	qreal z = distanceToSceneCenter() - zNearScene;

	// Prevents negative or null zNear values.
	const qreal zMin = zNearCoefficient() * zNearScene;
	if (z < zMin)
		switch (type())
		{
			case Camera::PERSPECTIVE  : z = zMin; break;
			case Camera::ORTHOGRAPHIC : z = 0.0;  break;
		}
	return z;
}

/*! Returns the far clipping plane distance used by the Camera projection matrix.

The far clipping plane is positioned at a distance equal to zClippingCoefficient() * sceneRadius()
behind the sceneCenter():
\code
zFar = distanceToSceneCenter() + zClippingCoefficient()*sceneRadius();
\endcode

See the zNear() documentation for details. */
qreal Camera::zFar() const
{
	return distanceToSceneCenter() + zClippingCoefficient() * sceneRadius();
}


/*! Sets the vertical fieldOfView() of the Camera (in radians).

Note that focusDistance() is set to sceneRadius() / tan(fieldOfView()/2) by this method. */
void Camera::setFieldOfView(qreal fov) {
	fieldOfView_ = fov;
	setFocusDistance(sceneRadius() / tan(fov/2.0));
	projectionMatrixIsUpToDate_ = false;
}

/*! Defines the Camera type().

Changing the camera Type alters the viewport and the objects' sizes can be changed.
This method garantees that the two frustum match in a plane normal to viewDirection(), passing through the pivotPoint().

Prefix the type with \c Camera if needed, as in:
\code
camera()->setType(Camera::ORTHOGRAPHIC);
// or even qglviewer::Camera::ORTHOGRAPHIC if you do not use namespace
\endcode */
void Camera::setType(Type type)
{
	// make ORTHOGRAPHIC frustum fit PERSPECTIVE (at least in plane normal to viewDirection(), passing
	// through RAP). Done only when CHANGING type since orthoCoef_ may have been changed with a
	// setPivotPoint() in the meantime.
	if ( (type == Camera::ORTHOGRAPHIC) && (type_ == Camera::PERSPECTIVE) )
		orthoCoef_ = tan(fieldOfView()/2.0);
	type_ = type;
	projectionMatrixIsUpToDate_ = false;
}

/*! Sets the Camera frame().

If you want to move the Camera, use setPosition() and setOrientation() or one of the Camera
positioning methods (lookAt(), fitSphere(), showEntireScene()...) instead.

If you want to save the Camera position(), there's no need to call this method either. Use
addKeyFrameToPath() and playPath() instead.

This method is actually mainly useful if you derive the ManipulatedCameraFrame class and want to
use an instance of your new class to move the Camera.

A \c NULL \p mcf pointer will silently be ignored. The calling method is responsible for
deleting the previous frame() pointer if needed in order to prevent memory leaks. */
void Camera::setFrame(ManipulatedCameraFrame* const mcf)
{
	if (!mcf)
		return;

	if (frame_) {
		disconnect(frame_, SIGNAL(modified()), this, SLOT(onFrameModified()));
	}

	frame_ = mcf;
	interpolationKfi_->setFrame(frame());

	connect(frame_, SIGNAL(modified()), this, SLOT(onFrameModified()));
	onFrameModified();
}

/*! Returns the distance from the Camera center to sceneCenter(), projected along the Camera Z axis.
  Used by zNear() and zFar() to optimize the Z range. */
qreal Camera::distanceToSceneCenter() const
{
	return fabs((frame()->coordinatesOf(sceneCenter())).z);
}


/*! Returns the \p halfWidth and \p halfHeight of the Camera orthographic frustum.

 These values are only valid and used when the Camera is of type() Camera::ORTHOGRAPHIC. They are
 expressed in OpenGL units and are used by loadProjectionMatrix() to define the projection matrix
 using:
 \code
 glOrtho( -halfWidth, halfWidth, -halfHeight, halfHeight, zNear(), zFar() )
 \endcode

 These values are proportional to the Camera (z projected) distance to the pivotPoint().
 When zooming on the object, the Camera is translated forward \e and its frustum is narrowed, making
 the object appear bigger on screen, as intuitively expected.

 Overload this method to change this behavior if desired, as is done in the
 <a href="../examples/standardCamera.html">standardCamera example</a>. */
void Camera::getOrthoWidthHeight(GLdouble& halfWidth, GLdouble& halfHeight) const
{
	const qreal dist = orthoCoef_ * fabs(cameraCoordinatesOf(pivotPoint()).z);
	//#CONNECTION# fitScreenRegion
	halfWidth  = dist * ((aspectRatio() < 1.0) ? 1.0 : aspectRatio());
	halfHeight = dist * ((aspectRatio() < 1.0) ? 1.0/aspectRatio() : 1.0);
}


/*! Computes the projection matrix associated with the Camera.

 If type() is Camera::PERSPECTIVE, defines a \c GL_PROJECTION matrix similar to what would \c
 gluPerspective() do using the fieldOfView(), window aspectRatio(), zNear() and zFar() parameters.

 If type() is Camera::ORTHOGRAPHIC, the projection matrix is as what \c glOrtho() would do.
 Frustum's width and height are set using getOrthoWidthHeight().

 Both types use zNear() and zFar() to place clipping planes. These values are determined from
 sceneRadius() and sceneCenter() so that they best fit the scene size.

 Use getProjectionMatrix() to retrieve this matrix. Overload loadProjectionMatrix() if you want your
 Camera to use an exotic projection matrix.

 \note You must call this method if your Camera is not associated with a QGLViewer and is used for
 offscreen computations (using (un)projectedCoordinatesOf() for instance). loadProjectionMatrix()
 does it otherwise. */
void Camera::computeProjectionMatrix() const
{
	if (projectionMatrixIsUpToDate_) return;

	const qreal ZNear = zNear();
	const qreal ZFar  = zFar();

	switch (type())
	{
		case Camera::PERSPECTIVE:
		{
			// #CONNECTION# all non null coefficients were set to 0.0 in constructor.
			const qreal f = 1.0/tan(fieldOfView()/2.0);
			projectionMatrix_[0]  = f/aspectRatio();
			projectionMatrix_[5]  = f;
			projectionMatrix_[10] = (ZNear + ZFar) / (ZNear - ZFar);
			projectionMatrix_[11] = -1.0;
			projectionMatrix_[14] = 2.0 * ZNear * ZFar / (ZNear - ZFar);
			projectionMatrix_[15] = 0.0;
			// same as gluPerspective( 180.0*fieldOfView()/M_PI, aspectRatio(), zNear(), zFar() );
			break;
		}
		case Camera::ORTHOGRAPHIC:
		{
			GLdouble w, h;
			getOrthoWidthHeight(w,h);
			projectionMatrix_[0]  = 1.0/w;
			projectionMatrix_[5]  = 1.0/h;
			projectionMatrix_[10] = -2.0/(ZFar - ZNear);
			projectionMatrix_[11] = 0.0;
			projectionMatrix_[14] = -(ZFar + ZNear)/(ZFar - ZNear);
			projectionMatrix_[15] = 1.0;
			// same as glOrtho( -w, w, -h, h, zNear(), zFar() );
			break;
		}
	}

	projectionMatrixIsUpToDate_ = true;
}

/*! Computes the modelView matrix associated with the Camera's position() and orientation().

 This matrix converts from the world coordinates system to the Camera coordinates system, so that
 coordinates can then be projected on screen using the projection matrix (see computeProjectionMatrix()).

 Use getModelViewMatrix() to retrieve this matrix.

 \note You must call this method if your Camera is not associated with a QGLViewer and is used for
 offscreen computations (using (un)projectedCoordinatesOf() for instance). loadModelViewMatrix()
 does it otherwise. */
void Camera::computeModelViewMatrix() const
{
	if (modelViewMatrixIsUpToDate_) return;

	const Quaternion q = frame()->orientation();

	const qreal q00 = 2.0 * q[0] * q[0];
	const qreal q11 = 2.0 * q[1] * q[1];
	const qreal q22 = 2.0 * q[2] * q[2];

	const qreal q01 = 2.0 * q[0] * q[1];
	const qreal q02 = 2.0 * q[0] * q[2];
	const qreal q03 = 2.0 * q[0] * q[3];

	const qreal q12 = 2.0 * q[1] * q[2];
	const qreal q13 = 2.0 * q[1] * q[3];

	const qreal q23 = 2.0 * q[2] * q[3];

	modelViewMatrix_[0] = 1.0 - q11 - q22;
	modelViewMatrix_[1] =       q01 - q23;
	modelViewMatrix_[2] =       q02 + q13;
	modelViewMatrix_[3] = 0.0;

	modelViewMatrix_[4] =       q01 + q23;
	modelViewMatrix_[5] = 1.0 - q22 - q00;
	modelViewMatrix_[6] =       q12 - q03;
	modelViewMatrix_[7] = 0.0;

	modelViewMatrix_[8] =        q02 - q13;
	modelViewMatrix_[9] =        q12 + q03;
	modelViewMatrix_[10] = 1.0 - q11 - q00;
	modelViewMatrix_[11] = 0.0;

	const Vec t = q.inverseRotate(frame()->position());

	modelViewMatrix_[12] = -t.x;
	modelViewMatrix_[13] = -t.y;
	modelViewMatrix_[14] = -t.z;
	modelViewMatrix_[15] = 1.0;

	modelViewMatrixIsUpToDate_ = true;
}


/*! Loads the OpenGL \c GL_PROJECTION matrix with the Camera projection matrix.

 The Camera projection matrix is computed using computeProjectionMatrix().

 When \p reset is \c true (default), the method clears the previous projection matrix by calling \c
 glLoadIdentity before setting the matrix. Setting \p reset to \c false is useful for \c GL_SELECT
 mode, to combine the pushed matrix with a picking matrix. See QGLViewer::beginSelection() for details.

 This method is used by QGLViewer::preDraw() (called before user's QGLViewer::draw() method) to
 set the \c GL_PROJECTION matrix according to the viewer's QGLViewer::camera() settings.

 Use getProjectionMatrix() to retrieve this matrix. Overload this method if you want your Camera to
 use an exotic projection matrix. See also loadModelViewMatrix().

 \attention \c glMatrixMode is set to \c GL_PROJECTION.

 \attention If you use several OpenGL contexts and bypass the Qt main refresh loop, you should call
 QGLWidget::makeCurrent() before this method in order to activate the right OpenGL context. */
void Camera::loadProjectionMatrix(bool reset) const
{
	// WARNING: makeCurrent must be called by every calling method
	glMatrixMode(GL_PROJECTION);

	if (reset)
		glLoadIdentity();

	computeProjectionMatrix();

	glMultMatrixd(projectionMatrix_);
}

/*! Loads the OpenGL \c GL_MODELVIEW matrix with the modelView matrix corresponding to the Camera.

 Calls computeModelViewMatrix() to compute the Camera's modelView matrix.

 This method is used by QGLViewer::preDraw() (called before user's QGLViewer::draw() method) to
 set the \c GL_MODELVIEW matrix according to the viewer's QGLViewer::camera() position() and
 orientation().

 As a result, the vertices used in QGLViewer::draw() can be defined in the so called world
 coordinate system. They are multiplied by this matrix to get converted to the Camera coordinate
 system, before getting projected using the \c GL_PROJECTION matrix (see loadProjectionMatrix()).

 When \p reset is \c true (default), the method loads (overwrites) the \c GL_MODELVIEW matrix. Setting
 \p reset to \c false simply calls \c glMultMatrixd (might be useful for some applications).

 Overload this method or simply call glLoadMatrixd() at the beginning of QGLViewer::draw() if you
 want your Camera to use an exotic modelView matrix. See also loadProjectionMatrix().

 getModelViewMatrix() returns the 4x4 modelView matrix.

 \attention glMatrixMode is set to \c GL_MODELVIEW

 \attention If you use several OpenGL contexts and bypass the Qt main refresh loop, you should call
 QGLWidget::makeCurrent() before this method in order to activate the right OpenGL context. */
void Camera::loadModelViewMatrix(bool reset) const
{
	// WARNING: makeCurrent must be called by every calling method
	glMatrixMode(GL_MODELVIEW);
	computeModelViewMatrix();
	if (reset)
		glLoadMatrixd(modelViewMatrix_);
	else
		glMultMatrixd(modelViewMatrix_);
}

/*! Same as loadProjectionMatrix() but for a stereo setup.

 Only the Camera::PERSPECTIVE type() is supported for stereo mode. See
 QGLViewer::setStereoDisplay().

 Uses focusDistance(), IODistance(), and physicalScreenWidth() to compute cameras
 offset and asymmetric frustums.

 When \p leftBuffer is \c true, computes the projection matrix associated to the left eye (right eye
 otherwise). See also loadModelViewMatrixStereo().

 See the <a href="../examples/stereoViewer.html">stereoViewer</a> and the <a
 href="../examples/contribs.html#anaglyph">anaglyph</a> examples for an illustration.

 To retrieve this matrix, use a code like:
 \code
 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
 loadProjectionMatrixStereo(left_or_right);
 glGetDoublev(GL_PROJECTION_MATRIX, m);
 glPopMatrix();
 \endcode
 Note that getProjectionMatrix() always returns the mono-vision matrix.

 \attention glMatrixMode is set to \c GL_PROJECTION. */
void Camera::loadProjectionMatrixStereo(bool leftBuffer) const
{
	qreal left, right, bottom, top;
	qreal screenHalfWidth, halfWidth, side, shift, delta;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (type())
	{
		case Camera::PERSPECTIVE:
			// compute half width of screen,
			// corresponding to zero parallax plane to deduce decay of cameras
			screenHalfWidth = focusDistance() * tan(horizontalFieldOfView() / 2.0);
			shift = screenHalfWidth * IODistance() / physicalScreenWidth();
			// should be * current y  / y total
			// to take into account that the window doesn't cover the entire screen

			// compute half width of "view" at znear and the delta corresponding to
			// the shifted camera to deduce what to set for asymmetric frustums
			halfWidth = zNear() * tan(horizontalFieldOfView() / 2.0);
			delta  = shift * zNear() / focusDistance();
			side   = leftBuffer ? -1.0 : 1.0;

			left   = -halfWidth + side * delta;
			right  =  halfWidth + side * delta;
			top    = halfWidth / aspectRatio();
			bottom = -top;
			glFrustum(left, right, bottom, top, zNear(), zFar() );
			break;

		case Camera::ORTHOGRAPHIC:
			qWarning("Camera::setProjectionMatrixStereo: Stereo not available with Ortho mode");
			break;
	}
}

/*! Same as loadModelViewMatrix() but for a stereo setup.

 Only the Camera::PERSPECTIVE type() is supported for stereo mode. See
 QGLViewer::setStereoDisplay().

 The modelView matrix is almost identical to the mono-vision one. It is simply translated along its
 horizontal axis by a value that depends on stereo parameters (see focusDistance(),
 IODistance(), and physicalScreenWidth()).

 When \p leftBuffer is \c true, computes the modelView matrix associated to the left eye (right eye
 otherwise).

 loadProjectionMatrixStereo() explains how to retrieve to resulting matrix.

 See the <a href="../examples/stereoViewer.html">stereoViewer</a> and the <a
 href="../examples/contribs.html#anaglyph">anaglyph</a> examples for an illustration.

 \attention glMatrixMode is set to \c GL_MODELVIEW. */
void Camera::loadModelViewMatrixStereo(bool leftBuffer) const
{
	// WARNING: makeCurrent must be called by every calling method
	glMatrixMode(GL_MODELVIEW);

	qreal halfWidth = focusDistance() * tan(horizontalFieldOfView() / 2.0);
	qreal shift     = halfWidth * IODistance() / physicalScreenWidth(); // * current window width / full screen width

	computeModelViewMatrix();
	if (leftBuffer)
		modelViewMatrix_[12] -= shift;
	else
		modelViewMatrix_[12] += shift;
	glLoadMatrixd(modelViewMatrix_);
}

/*! Fills \p m with the Camera projection matrix values.

 Based on computeProjectionMatrix() to make sure the Camera projection matrix is up to date.

 This matrix only reflects the Camera's internal parameters and it may differ from the \c
 GL_PROJECTION matrix retrieved using \c glGetDoublev(GL_PROJECTION_MATRIX, m). It actually
 represents the state of the \c GL_PROJECTION after QGLViewer::preDraw(), at the beginning of
 QGLViewer::draw(). If you modified the \c GL_PROJECTION matrix (for instance using
 QGLViewer::startScreenCoordinatesSystem()), the two results differ.

 The result is an OpenGL 4x4 matrix, which is given in \e column-major order (see \c glMultMatrix
 man page for details).

 See also getModelViewMatrix() and setFromProjectionMatrix(). */
void Camera::getProjectionMatrix(GLdouble m[16]) const
{
	computeProjectionMatrix();
	for (unsigned short i=0; i<16; ++i)
		m[i] = projectionMatrix_[i];
}

/*! Overloaded getProjectionMatrix(GLdouble m[16]) method using a \c GLfloat array instead. */
void Camera::getProjectionMatrix(GLfloat m[16]) const
{
	static GLdouble mat[16];
	getProjectionMatrix(mat);
	for (unsigned short i=0; i<16; ++i)
		m[i] = float(mat[i]);
}

/*! Fills \p m with the Camera modelView matrix values.

 First calls computeModelViewMatrix() to define the Camera modelView matrix.

 Note that this matrix may \e not be the one you would get from a \c
 glGetDoublev(GL_MODELVIEW_MATRIX, m). It actually represents the state of the \c
 GL_MODELVIEW after QGLViewer::preDraw(), at the \e beginning of QGLViewer::draw(). It converts from
 the world to the Camera coordinate system. As soon as you modify the \c GL_MODELVIEW in your
 QGLViewer::draw() method (using glTranslate, glRotate... or similar methods), the two matrices differ.

 The result is an OpenGL 4x4 matrix, which is given in \e column-major order (see \c glMultMatrix
 man page for details).

 See also getProjectionMatrix() and setFromModelViewMatrix(). */
void Camera::getModelViewMatrix(GLdouble m[16]) const
{
	// May not be needed, but easier like this.
	// Prevents from retrieving matrix in stereo mode -> overwrites shifted value.
	computeModelViewMatrix();
	for (unsigned short i=0; i<16; ++i)
		m[i] = modelViewMatrix_[i];
}


/*! Overloaded getModelViewMatrix(GLdouble m[16]) method using a \c GLfloat array instead. */
void Camera::getModelViewMatrix(GLfloat m[16]) const
{
	static GLdouble mat[16];
	getModelViewMatrix(mat);
	for (unsigned short i=0; i<16; ++i)
		m[i] = float(mat[i]);
}

/*! Fills \p m with the product of the ModelView and Projection matrices.

  Calls getModelViewMatrix() and getProjectionMatrix() and then fills \p m with the product of these two matrices. */
void Camera::getModelViewProjectionMatrix(GLdouble m[16]) const
{
	GLdouble mv[16];
	GLdouble proj[16];
	getModelViewMatrix(mv);
	getProjectionMatrix(proj);

	for (unsigned short i=0; i<4; ++i)
	{
		for (unsigned short j=0; j<4; ++j)
		{
			qreal sum = 0.0;
			for (unsigned short k=0; k<4; ++k)
				sum += proj[i+4*k]*mv[k+4*j];
			m[i+4*j] = sum;
		}
	}
}

/*! Overloaded getModelViewProjectionMatrix(GLdouble m[16]) method using a \c GLfloat array instead. */
void Camera::getModelViewProjectionMatrix(GLfloat m[16]) const
{
	static GLdouble mat[16];
	getModelViewProjectionMatrix(mat);
	for (unsigned short i=0; i<16; ++i)
		m[i] = float(mat[i]);
}

/*! Sets the sceneRadius() value. Negative values are ignored.

\attention This methods also sets focusDistance() to sceneRadius() / tan(fieldOfView()/2) and
flySpeed() to 1% of sceneRadius(). */
void Camera::setSceneRadius(qreal radius)
{
	if (radius <= 0.0)
	{
		qWarning("Scene radius must be positive - Ignoring value");
		return;
	}

	sceneRadius_ = radius;
	projectionMatrixIsUpToDate_ = false;

	setFocusDistance(sceneRadius() / tan(fieldOfView()/2.0));

	frame()->setFlySpeed(0.01*sceneRadius());
}

/*! Similar to setSceneRadius() and setSceneCenter(), but the scene limits are defined by a (world
  axis aligned) bounding box. */
void Camera::setSceneBoundingBox(const Vec& min, const Vec& max)
{
	setSceneCenter((min+max)/2.0);
	setSceneRadius(0.5*(max-min).norm());
}


/*! Sets the sceneCenter().

 \attention This method also sets the pivotPoint() to sceneCenter(). */
void Camera::setSceneCenter(const Vec& center)
{
	sceneCenter_ = center;
	setPivotPoint(sceneCenter());
	projectionMatrixIsUpToDate_ = false;
}

/*! setSceneCenter() to the result of pointUnderPixel(\p pixel).

  Returns \c true if a pointUnderPixel() was found and sceneCenter() was actually changed.

  See also setPivotPointFromPixel(). See the pointUnderPixel() documentation. */
bool Camera::setSceneCenterFromPixel(const QPoint& pixel)
{
	bool found;
	Vec point = pointUnderPixel(pixel, found);
	if (found)
		setSceneCenter(point);
	return found;
}

#ifndef DOXYGEN
void Camera::setRevolveAroundPoint(const Vec& point) {
	qWarning("setRevolveAroundPoint() is deprecated, use setPivotPoint() instead");
	setPivotPoint(point);
}
bool Camera::setRevolveAroundPointFromPixel(const QPoint& pixel) {
	qWarning("setRevolveAroundPointFromPixel() is deprecated, use setPivotPointFromPixel() instead");
	return setPivotPointFromPixel(pixel);
}
Vec Camera::revolveAroundPoint() const {
	qWarning("revolveAroundPoint() is deprecated, use pivotPoint() instead");
	return pivotPoint();
}
#endif

/*! Changes the pivotPoint() to \p point (defined in the world coordinate system). */
void Camera::setPivotPoint(const Vec& point)
{
	const qreal prevDist = fabs(cameraCoordinatesOf(pivotPoint()).z);

	// If frame's RAP is set directly, projectionMatrixIsUpToDate_ should also be
	// set to false to ensure proper recomputation of the ORTHO projection matrix.
	frame()->setPivotPoint(point);

	// orthoCoef_ is used to compensate for changes of the pivotPoint, so that the image does
	// not change when the pivotPoint is changed in ORTHOGRAPHIC mode.
	const qreal newDist = fabs(cameraCoordinatesOf(pivotPoint()).z);
	// Prevents division by zero when rap is set to camera position
	if ((prevDist > 1E-9) && (newDist > 1E-9))
		orthoCoef_ *= prevDist / newDist;
	projectionMatrixIsUpToDate_ = false;
}

/*! The pivotPoint() is set to the point located under \p pixel on screen.

Returns \c true if a pointUnderPixel() was found. If no point was found under \p pixel, the
pivotPoint() is left unchanged.

\p pixel is expressed in Qt format (origin in the upper left corner of the window). See
pointUnderPixel().

See also setSceneCenterFromPixel(). */
bool Camera::setPivotPointFromPixel(const QPoint& pixel)
{
	bool found;
	Vec point = pointUnderPixel(pixel, found);
	if (found)
		setPivotPoint(point);
	return found;
}

/*! Returns the ratio between pixel and OpenGL units at \p position.

 A line of \c n * pixelGLRatio() OpenGL units, located at \p position in the world coordinates
 system, will be projected with a length of \c n pixels on screen.

 Use this method to scale objects so that they have a constant pixel size on screen. The following
 code will draw a 20 pixel line, starting at sceneCenter() and always directed along the screen
 vertical direction:
 \code
 glBegin(GL_LINES);
 glVertex3fv(sceneCenter());
 glVertex3fv(sceneCenter() + 20 * pixelGLRatio(sceneCenter()) * camera()->upVector());
 glEnd();
 \endcode */
qreal Camera::pixelGLRatio(const Vec& position) const
{
	switch (type())
	{
		case Camera::PERSPECTIVE :
			return 2.0 * fabs((frame()->coordinatesOf(position)).z) * tan(fieldOfView()/2.0) / screenHeight();
		case Camera::ORTHOGRAPHIC :
		{
			GLdouble w, h;
			getOrthoWidthHeight(w,h);
			return 2.0 * h / screenHeight();
		}
	}
	// Bad compilers complain
	return 1.0;
}

/*! Changes the Camera fieldOfView() so that the entire scene (defined by QGLViewer::sceneCenter()
 and QGLViewer::sceneRadius()) is visible from the Camera position().

 The position() and orientation() of the Camera are not modified and you first have to orientate the
 Camera in order to actually see the scene (see lookAt(), showEntireScene() or fitSphere()).

 This method is especially useful for \e shadow \e maps computation. Use the Camera positioning
 tools (setPosition(), lookAt()) to position a Camera at the light position. Then use this method to
 define the fieldOfView() so that the shadow map resolution is optimally used:
 \code
 // The light camera needs size hints in order to optimize its fieldOfView
 lightCamera->setSceneRadius(sceneRadius());
 lightCamera->setSceneCenter(sceneCenter());

 // Place the light camera.
 lightCamera->setPosition(lightFrame->position());
 lightCamera->lookAt(sceneCenter());
 lightCamera->setFOVToFitScene();
 \endcode

 See the (soon available) shadowMap contribution example for a practical implementation.

 \attention The fieldOfView() is clamped to M_PI/2.0. This happens when the Camera is at a distance
 lower than sqrt(2.0) * sceneRadius() from the sceneCenter(). It optimizes the shadow map
 resolution, although it may miss some parts of the scene. */
void Camera::setFOVToFitScene()
{
	if (distanceToSceneCenter() > sqrt(2.0)*sceneRadius())
		setFieldOfView(2.0 * asin(sceneRadius() / distanceToSceneCenter()));
	else
		setFieldOfView(M_PI / 2.0);
}

/*! Makes the Camera smoothly zoom on the pointUnderPixel() \p pixel.

 Nothing happens if no pointUnderPixel() is found. Otherwise a KeyFrameInterpolator is created that
 animates the Camera on a one second path that brings the Camera closer to the point under \p pixel.

 See also interpolateToFitScene(). */
void Camera::interpolateToZoomOnPixel(const QPoint& pixel)
{
	const qreal coef = 0.1;

	bool found;
	Vec target = pointUnderPixel(pixel, found);

	if (!found)
		return;

	if (interpolationKfi_->interpolationIsStarted())
		interpolationKfi_->stopInterpolation();

	interpolationKfi_->deletePath();
	interpolationKfi_->addKeyFrame(*(frame()));

	interpolationKfi_->addKeyFrame(Frame(0.3*frame()->position() + 0.7*target, frame()->orientation()), 0.4);

	// Small hack: attach a temporary frame to take advantage of lookAt without modifying frame
	static ManipulatedCameraFrame* tempFrame = new ManipulatedCameraFrame();
	ManipulatedCameraFrame* const originalFrame = frame();
	tempFrame->setPosition(coef*frame()->position() + (1.0-coef)*target);
	tempFrame->setOrientation(frame()->orientation());
	setFrame(tempFrame);
	lookAt(target);
	setFrame(originalFrame);

	interpolationKfi_->addKeyFrame(*(tempFrame), 1.0);

	interpolationKfi_->startInterpolation();
}

/*! Interpolates the Camera on a one second KeyFrameInterpolator path so that the entire scene fits
 the screen at the end.

 The scene is defined by its sceneCenter() and its sceneRadius(). See showEntireScene().

 The orientation() of the Camera is not modified. See also interpolateToZoomOnPixel(). */
void Camera::interpolateToFitScene()
{
	if (interpolationKfi_->interpolationIsStarted())
		interpolationKfi_->stopInterpolation();

	interpolationKfi_->deletePath();
	interpolationKfi_->addKeyFrame(*(frame()));

	// Small hack:  attach a temporary frame to take advantage of lookAt without modifying frame
	static ManipulatedCameraFrame* tempFrame = new ManipulatedCameraFrame();
	ManipulatedCameraFrame* const originalFrame = frame();
	tempFrame->setPosition(frame()->position());
	tempFrame->setOrientation(frame()->orientation());
	setFrame(tempFrame);
	showEntireScene();
	setFrame(originalFrame);

	interpolationKfi_->addKeyFrame(*(tempFrame));

	interpolationKfi_->startInterpolation();
}


/*! Smoothly interpolates the Camera on a KeyFrameInterpolator path so that it goes to \p fr.

  \p fr is expressed in world coordinates. \p duration tunes the interpolation speed (default is
  1 second).

  See also interpolateToFitScene() and interpolateToZoomOnPixel(). */
void Camera::interpolateTo(const Frame& fr, qreal duration)
{
