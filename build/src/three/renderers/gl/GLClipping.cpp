#include "GLClipping.h"
namespace three {
	namespace gl {
        std::vector<float> GLClipping::projectPlanes(std::vector<Plane>& planes, const Camera::ptr& camera, int dstOffset, bool skipTransform)
		{
            auto nPlanes = planes.size();
            std::vector<float>& dstArray = uniform;

            if (nPlanes != 0)
            {
                if (skipTransform != true || dstArray.size() > 0)
                {
                    auto flatSize = dstOffset + nPlanes * 4;
                    auto viewMatrix = camera->matrixWorldInverse;

                    viewNormalMatrix.getNormalMatrix(viewMatrix);

                    if (dstArray.size()>0 || dstArray.size() < flatSize)
                    {
                        
                        dstArray.resize(flatSize);
                    }

                    
                    for (int i = 0, i4 = dstOffset; i != nPlanes; ++i, i4 += 4)
                    {
                        plane.copy(planes[i]);
                        plane.applyMatrix4(viewMatrix, &viewNormalMatrix);

                        plane.normal.toArray(&dstArray[0], i4);
                        dstArray[i4 + 3] = plane.constant;
                    }
                }
               
                uniform = dstArray;
                needUpdate = true;
            }
            
            numPlanes = nPlanes;
            numIntersection = 0;

            return dstArray;

            
		}
		void GLClipping::resetGlobalState()
		{
            if (uniform != globalState) {
                uniform = globalState;
                needUpdate = numGlobalPlanes > 0;
            }
            numPlanes = numGlobalPlanes;
            numIntersection = 0;
		}
        
        bool GLClipping::init(std::vector<Plane>& planes, bool enableLocalClipping, const Camera::ptr& camera)
		{
			bool enabled = planes.size() > 0 || enableLocalClipping || numGlobalPlanes != 0 || localClippingEnabled;

			localClippingEnabled = enableLocalClipping;
			globalState = projectPlanes(planes, camera, 0);
            numGlobalPlanes = planes.size();
            return enabled;
		}
		void three::gl::GLClipping::beginShadows()
		{
			renderingShadows = true;
            std::vector<Plane> emptyPlanes;
			projectPlanes(emptyPlanes);
		}
		void GLClipping::endShadows()
		{
			renderingShadows = false;
			resetGlobalState();
		}
		void GLClipping::setState(std::vector<Plane>& planes, bool clipIntersection, bool clipShadows, const Camera::ptr& camera, MaterialProperties& cache, bool fromCache)
		{
            if (!localClippingEnabled || planes.size() == 0 || renderingShadows && !clipShadows)
            {
                if (renderingShadows)
                {
                    std::vector<Plane> emptyPlanes;
                    projectPlanes(emptyPlanes);
                }
                else
                {
                    resetGlobalState();
                }
            }
            else
            {
                auto nGlobal = renderingShadows ? 0 : numGlobalPlanes;
                auto lGlobal = nGlobal * 4;

                uniform = cache.clippingState;

                
                projectPlanes(planes, camera, lGlobal, fromCache);

                cache.clippingState.assign(globalState.begin(), globalState.end());
                

                numIntersection = clipIntersection ? numPlanes : 0;
                numPlanes += nGlobal;
            }
		}
	}
}
