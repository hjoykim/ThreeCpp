#include "PerspectiveCamera.h"
#include <three/math/MathUtils.h>
using namespace three::math;
namespace three {
	/**
	 * Sets the FOV by focal length in respect to the current .filmGauge.
	 *
	 * The default film gauge is 35, so that the focal length can be specified for
	 * a 35mm (full frame) camera.
	 *
	 * Values for focal length and film gauge must have the same unit.
	 */
	void three::PerspectiveCamera::setFocalLength(float focalLength)
	{
		// see http://www.bobatkins.com/photography/technical/field_of_view.html
		float vExtentSlope = 0.5f * getFilmHeight() / focalLength;

		fov = RAD2DEG * 2 * atan(vExtentSlope);
		updateProjectionMatrix();
	}

	/**
	 * Calculates the focal length from the current .fov and .filmGauge.
	 */
	float PerspectiveCamera::getFocalLength()
	{
		float vExtentSlope = tan(DEG2RAD * 0.5f * fov);

		return 0.5f * getFilmHeight() / vExtentSlope;
	}

	float PerspectiveCamera::getEffectiveFOV()
	{
		return RAD2DEG * 2 * atan(tan(DEG2RAD * 0.5f * fov) / zoom);
	}

	float PerspectiveCamera::getFilmWidth()
	{
		// film not completely covered in portrait format (aspect < 1)
		return filmGauge * std::min(aspect, 1.0f);
	}

	float PerspectiveCamera::getFilmHeight()
	{
		// film not completely covered in landscape format (aspect > 1)
		return filmGauge / std::max(aspect, 1.0f);
	}
	/**
	 * Sets an offset in a larger frustum. This is useful for multi-window or
	 * multi-monitor/multi-machine setups.
	 *
	 * For example, if you have 3x2 monitors and each monitor is 1920x1080 and
	 * the monitors are in grid like this
	 *
	 *   +---+---+---+
	 *   | A | B | C |
	 *   +---+---+---+
	 *   | D | E | F |
	 *   +---+---+---+
	 *
	 * then for each monitor you would call it like this
	 *
	 *   var w = 1920;
	 *   var h = 1080;
	 *   var fullWidth = w * 3;
	 *   var fullHeight = h * 2;
	 *
	 *   --A--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 0, h * 0, w, h );
	 *   --B--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 1, h * 0, w, h );
	 *   --C--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 2, h * 0, w, h );
	 *   --D--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 0, h * 1, w, h );
	 *   --E--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 1, h * 1, w, h );
	 *   --F--
	 *   camera.setViewOffset( fullWidth, fullHeight, w * 2, h * 1, w, h );
	 *
	 *   Note there is no reason monitors have to be the same size or in a grid.
	 */
	void PerspectiveCamera::setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height)
	{
		aspect = fullWidth / fullHeight;		

		view.enabled = true;
		view.fullWidth = fullWidth;
		view.fullHeight = fullHeight;
		view.offsetX = x;
		view.offsetY = y;
		view.width = width;
		view.height = height;

		updateProjectionMatrix();
	}

	void PerspectiveCamera::clearViewOffset()
	{
		//if (view != = null) {

			view.enabled = false;

		//}

		updateProjectionMatrix();

	}

	void PerspectiveCamera::updateProjectionMatrix()
	{
		Camera::updateProjectionMatrix();
		//float near = this.near,
		float top = _near * tan(DEG2RAD * 0.5f * fov) / zoom;
		float height = 2 * top;
		float width = aspect * height;
		float left = -0.5f * width;

		if (view.enabled) {

			float fullWidth = view.fullWidth;
			float fullHeight = view.fullHeight;

			left += view.offsetX * width / fullWidth;
			top -= view.offsetY * height / fullHeight;
			width *= view.width / fullWidth;
			height *= view.height / fullHeight;

		}

		float skew = filmOffset;
		if (skew != 0) left += _near * skew / getFilmWidth();

		projectionMatrix.makePerspective(left, left + width, top, top - height, _near, _far);

		projectionMatrixInverse.getInverse(projectionMatrix);
	}

}