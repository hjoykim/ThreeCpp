#include "TrackballControls.h"
#include <three/math/Quaternion.h>
#include <iostream>
namespace three {
    namespace control {
        Vector2 TrackballControls::getMouseOnScreen(float pageX, float pageY)
        {
            //Vector4
            //X->left, Y->top,Z->width, W->height
            return Vector2(
                (pageX - screen.x) / screen.z,
                (pageY - screen.y) / screen.w);


        }
        Vector3 TrackballControls::getMouseProjectionOnBall(float pageX, float pageY)
        {
            // TODO: 여기에 return 문을 삽입합니다.
            Vector3 mouseOnBall = Vector3(
                (pageX - screen.z * 0.5f - screen.x) / (screen.z * 0.5f),
                (screen.w * 0.5f + screen.y - pageY) / (screen.w * 0.5f),
                0.0f
            );

            float length = mouseOnBall.length();

            if (noRoll)
            {
                if (length < SQRT1_2)
                    mouseOnBall.z = (float)sqrt(1.0f - length * length);
                else
                    mouseOnBall.z = 0.5f / length;
            }
            else if (length > 1.0f)
                mouseOnBall.normalize();
            else
                mouseOnBall.z = (float)sqrt(1.0f - length * length);

            Vector3 camPos = camera->position;
            eye = camPos - target;
            Vector3 upClone;
            upClone.copy(camera->up);

            Vector3 projection;
            upClone.normalize();

            projection = upClone.multiplyScalar(mouseOnBall.y);

            Vector3 cross;
            cross.copy(camera->up);
            cross.cross(eye);
            cross.normalize();
            cross.multiplyScalar(mouseOnBall.x);

            projection = projection.add(cross);

            //  projection.add(_eye.normalize().scale(mouseOnBall.z));
            Vector3 eyeClone;
            eyeClone.copy(eye);
            eyeClone.normalize();

            projection.add(eyeClone.multiplyScalar(mouseOnBall.z));

            return projection;
        }
        void TrackballControls::rotateCamera()
        {
            Vector3 axis;
            Quaternion quaternion;

            float angle = (float)acos(rotateStart.dot(rotateEnd) / rotateStart.length() / rotateEnd.length());

            if (angle > 0)
            {
                axis.crossVectors(rotateStart, rotateEnd).normalize();

                angle *= rotateSpeed;

                quaternion.setFromAxisAngle(axis, -angle);

                eye.applyQuaternion(quaternion);
                camera->up.applyQuaternion(quaternion);

                rotateEnd.applyQuaternion(quaternion);

                if (staticMoving)
                {
                    rotateStart.copy(rotateEnd);
                }
                else
                {
                    quaternion.setFromAxisAngle(axis, angle * (dynamicDampingFactor - 1.0f));
                    rotateStart.applyQuaternion(quaternion);

                }

            }
        }
        void TrackballControls::zoomCamera()
        {
            if (state == STATE::TOUCH_ZOOM_PAN)
            {
                float factor = touchZoomDistanceStart / touchZoomDistanceEnd;
                touchZoomDistanceStart = touchZoomDistanceEnd;
                eye = eye * factor;
            }
            else
            {
                float factor = (float)(1.0f + (zoomEnd.y - zoomStart.y) * zoomSpeed);
                if (factor != 1.0f && factor > 0.0f)
                {
                    eye.multiplyScalar(factor);
                }
                if (staticMoving)
                {
                    zoomStart.set(zoomEnd.x, zoomEnd.y);
                }
                else
                {
                    zoomStart.y += (zoomEnd.y - zoomStart.y) * dynamicDampingFactor;
                }

            }
        }
        void TrackballControls::panCamera()
        {
            Vector2 mouseChange;
            Vector3 objectUp;
            Vector3 pan;

            mouseChange.copy(panEnd);
            mouseChange.sub(panStart);

            if (mouseChange.lengthSq() > 0)
            {
                mouseChange.multiplyScalar(eye.length() * panSpeed);
                pan.copy(eye);
                pan.cross(camera->up);
                pan.setLength(mouseChange.x);
                pan.add(objectUp.copy(camera->up).setLength(mouseChange.y));

                camera->position.add(pan);
                target.add(pan);

                /*
                pan.Normalize();
                pan = Vector3.Multiply(pan, mouseChange.X);

                Vector3 upClone = new Vector3(this.camera.Up);
                upClone.Normalize();
                upClone = Vector3.Multiply(upClone, mouseChange.Y);
                pan += upClone;

                this.camera.Position = Vector3.Add(this.camera.Position, pan);
                this.target = Vector3.Add(target, pan);
                */
                if (staticMoving)
                {
                    panStart.copy(panEnd);
                }
                else
                {
                    //mouseChange = panEnd - panStart;
                    //mouseChange = Vector2.Multiply(mouseChange, DynamicDampingFactor);
                    //panStart += mouseChange;
                    panStart.add(mouseChange.subVectors(panEnd, panStart).multiplyScalar(dynamicDampingFactor));
                }

            }
        }
        void TrackballControls::checkDistance()
        {
            if (!noZoom || !noPan)
            {

                if (eye.lengthSq() > maxDistance * maxDistance)
                {
                    eye.normalize();
                    eye.multiplyScalar(maxDistance);

                    camera->position = target + eye;
                }

                if (eye.lengthSq() < minDistance * minDistance)
                {
                    eye.normalize();

                    eye.multiplyScalar(minDistance);

                    camera->position = target + eye;
                }

            }
        }
        void TrackballControls::update()
        {
            eye.subVectors(camera->position, target);
            if (!noRotate)
            {
                rotateCamera();
            }

            if (!noZoom)
            {
                zoomCamera();
            }

            if (!noPan)
            {
                panCamera();
            }

            // object.position =  target + _eye;
            camera->position = target + eye;

            checkDistance();

            // object.lookAt( target );
            camera->lookAt(target);



            // distanceToSquared
            if ((lastPosition - camera->position).lengthSq() > 0.0f)
            {
                //
                //   dispatchEvent( changeEvent );

                lastPosition.copy(camera->position);

            }
        }
        void TrackballControls::sizeChanged(const Vector4& screen)
        {
            //Vector4
            //X->left, Y->top,Z->width, W->height
            this->screen.copy(screen);
            camera->aspect = (float)(screen.z / screen.w);
            camera->updateProjectionMatrix();
        }
        void TrackballControls::reset()
        {
            state = STATE::NONE;
            keyState = STATE::NONE;

            target.copy(target0);
            camera->position.copy(position0);
            camera->up.copy(up0);
            camera->zoom = zoom0;

            camera->updateProjectionMatrix();

            eye.subVectors(camera->position, target);

            camera->lookAt(target);

            //_this.dispatchEvent(changeEvent);

            lastPosition.copy(camera->position);
            lastZoom = camera->zoom;
        }
        void TrackballControls::keydown(byte keyCode)
        {
            if (enabled == false) return;

            //window.removeEventListener('keydown', keydown);

            if (keyState != STATE::NONE) {

                return;

            }
            else if (keyCode == keys[(byte)STATE::ROTATE] && !noRotate) {

                keyState = STATE::ROTATE;

            }
            else if (keyCode == keys[(byte)STATE::ZOOM] && !noZoom) {

                keyState = STATE::ZOOM;

            }
            else if (keyCode == keys[(byte)STATE::PAN] && !noPan) {

                keyState = STATE::PAN;

            }
        }
        void TrackballControls::keyup()
        {
            if (enabled == false) return;

            keyState = STATE::NONE;

            //window.addEventListener('keydown', keydown, false);
        }
        void TrackballControls::mouseDown(unsigned button, float x, float y)
        {
            if (!enabled) return;


            if (state == STATE::NONE)
            {
                switch (button)
                {
                case 0: //MouseButtons.Left
                    state = STATE::ROTATE;
                    break;
                case 1://MouseButtons.Middle:
                    state = STATE::ZOOM;
                    break;
                case 2: //MouseButtons.Right:
                    state = STATE::PAN;
                    break;
                }
            }

            if (state == STATE::ROTATE && !noRotate)
            {

                rotateStart = getMouseProjectionOnBall(x, y);
                rotateEnd = rotateStart;

            }
            else if (state == STATE::ZOOM && !noZoom)
            {

                zoomStart = getMouseOnScreen(x, y);
                zoomEnd = zoomStart;


            }
            else if (state == STATE::PAN && !noPan)
            {

                panStart = getMouseOnScreen(x, y);
                panEnd = panStart;

            }
        }

        // if sender was unfocused, then you must focus that
        // if(sender.focused==false) sender.focus()
        void TrackballControls::mouseMove(float x, float y)
        {
            if (!enabled) return;

            if (state == STATE::ROTATE && !noRotate)
            {

                rotateEnd = getMouseProjectionOnBall(x, y);

            }
            else if (state == STATE::ZOOM && !noZoom)
            {

                zoomEnd = getMouseOnScreen(x, y);

            }
            else if (state == STATE::PAN && !noPan)
            {

                panEnd = getMouseOnScreen(x, y);

            }
        }
        void TrackballControls::mouseUp()
        {
            if (!enabled) return;
            state = STATE::NONE;
        }
        void TrackballControls::mouseWheel(float delta)
        {
            if (!enabled) return;

            float _delta = delta / 40;

            zoomStart.y += _delta * 0.01f;

        }
        void TrackballControls::touchstart()
        {
            //TODO
        }
        void TrackballControls::touchmove()
        {
            //TODO
        }
        void TrackballControls::touchend()
        {
            //TODO
        }
    }
}
