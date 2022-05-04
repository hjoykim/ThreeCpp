#ifndef TRACKBALL_CONTROLS_THREE_H
#define TRACKBALL_CONTROLS_THREE_H
#include <three/cameras/Camera.h>
#include <three/math/Vector3.h>
#include <three/math/Vector2.h>
#include <three/math/Vector4.h>
#include <limits>
#include <three/Types.h>
namespace three {
    namespace control {
        enum class STATE : int {
            NONE = -1,
            ROTATE = 0,
            ZOOM = 1,
            PAN = 2,
            TOUCH_ROTATE = 3,
            TOUCH_ZOOM_PAN = 4
        };


        class TrackballControls {
        private:
            //{ 65 /*A*/, 83 /*S*/, 68 /*D*/ };

            byte keys[3] = { 65 , 83, 68 };
            // internal variables
            Vector3 target;

            Vector3 lastPosition;

            float lastZoom = 1;

            STATE state = STATE::NONE;

            STATE keyState = STATE::NONE;

            Vector3 eye;

            Vector3 rotateStart;

            Vector3 rotateEnd;

            Vector2 zoomStart;

            Vector2 zoomEnd;

            float touchZoomDistanceStart = 0.0f;

            float touchZoomDistanceEnd = 0.0f;

            Vector2 panStart;

            Vector2 panEnd;

            Vector4 screen;

            Vector3 target0;

            Vector3 position0;

            Vector3 up0;

            float zoom0;
        public:

            using ptr = std::shared_ptr<TrackballControls>;

            bool enabled = true;

            float rotateSpeed = 1.0f;
            float zoomSpeed = 1.2f;
            float panSpeed = 0.3f;

            bool noRotate = false;
            bool noZoom = false;
            bool noPan = false;
            bool noRoll = false;

            bool staticMoving = false;
            float dynamicDampingFactor = 0.2f;

            float minDistance = 0.0f;
            float maxDistance = std::numeric_limits<float>::infinity();



            Camera::ptr camera;

            TrackballControls() {
                target.set(0, 0, 0);

                lastPosition.set(0, 0, 0);

                eye.set(0, 0, 0);

                rotateStart.set(0, 0, 0);

                rotateEnd.set(0, 0, 0);

                zoomStart.set(0, 0);

                zoomEnd.set(0, 0);

                panStart.set(0, 0);

                panEnd.set(0, 0);

                screen.set(0, 0, 0, 0);

                target0.set(0, 0, 0);

                position0.set(0, 0, 0);

                up0.set(0, 1, 0);

                zoom0 = 1;
            }
            TrackballControls(const Camera::ptr& camera, const Vector4& screen) : TrackballControls() {
                this->camera = camera;
                this->screen = screen;
                target0.copy(target);
                position0.copy(camera->position);
                up0.copy(camera->up);
                zoom0 = camera->zoom;
            }

            static ptr create() {
                return ptr(new TrackballControls());
            }
            static ptr create(const Camera::ptr& camera, const Vector4& screen) {
                return ptr(new TrackballControls(camera, screen));
            }

            void initTrackballControls(const Camera::ptr& camera, const Vector4& screen) {
                
                target.set(0, 0, 0);
                lastPosition.set(0, 0, 0);
                eye.set(0, 0, 0);
                rotateStart.set(0, 0, 0);
                rotateEnd.set(0, 0, 0);
                zoomStart.set(0, 0);
                zoomEnd.set(0, 0);
                panStart.set(0, 0);
                panEnd.set(0, 0);

                this->camera = camera;
                this->screen = screen;
                target0.copy(target);
                position0.copy(camera->position);
                up0.copy(camera->up);
                zoom0 = camera->zoom;
                
            }
            Vector2 getMouseOnScreen(float pageX, float pageY);

            Vector3 getMouseProjectionOnBall(float pageX, float pageY);

            void rotateCamera();

            void zoomCamera();

            void panCamera();

            void checkDistance();

            void update();

            void sizeChanged(const Vector4& screen);

            void reset();

            void keydown(byte keyCode);

            void keyup();

            void mouseDown(unsigned button, float x, float y);

            void mouseMove(float x, float y);

            void mouseUp();

            void mouseWheel(float delta);

            void touchstart();

            void touchmove();

            void touchend();

            void setState(int mouseState) {
                this->state = (STATE)mouseState;
            }
        };
    }
}
#endif