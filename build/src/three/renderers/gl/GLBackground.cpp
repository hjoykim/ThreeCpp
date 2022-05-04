#include "GLBackground.h"
#include <three/scenes/Scene.h>
#include <three/textures/CubeTexture.h>
#include <three/cameras/Camera.h>
#include <three/geometries/BoxGeometry.h>
#include <three/geometries/PlaneGeometry.h>
#include <three/materials/ShaderMaterial.h>
#include <three/renderers/shaders/ShaderLib.h>
#include <three/textures/Texture.h>
#include <three/renderers/GLRenderer.h>
#include <three/renderers/gl/GLRenderTarget.h>
#include <three/renderers/GLCubeRenderTarget.h>
#include <memory>
namespace three {
    namespace gl {
        void GLBackground::setClear(const Color& color, float alpha)
        {
            state->colorBuffer.setClear(color.r, color.g, color.b, alpha, premultipliedAlpha);
        }
        Color& GLBackground::getClearColor()
        {
            return clearColor;
        }
        void GLBackground::setClearColor(const Color& color, const float alpha)
        {
            clearColor.copy(color);
            clearAlpha = alpha;

            setClear(clearColor, clearAlpha);

        }
        float GLBackground::getClearAlpha()
        {
            return clearAlpha;
        }
        void GLBackground::setClearAlpha(float alpha)
        {
            clearAlpha = alpha;
            setClear(clearColor, clearAlpha);
        }
        void GLBackground::render(three::GLRenderer& renderer, GLRenderList& renderList,Scene& scene, Camera& camera, bool forceClear)
        {
          
           
            if (!scene.hasBackground()) {
                setClear(clearColor, clearAlpha);
            }
            else if (scene.isColor) {
                Color color = scene.getBackgroundColor();
                setClear(color, 1);
                forceClear = true;
            }

            if (renderer.autoClear || forceClear)
                renderer.clear(renderer.autoClearColor, renderer.autoClearDepth, renderer.autoClearStencil);

            if (scene.isCubeTexture || scene.isGLCubeRenderTarget || (scene.getBackgroundCubeTexture()!=nullptr && scene.getBackgroundCubeTexture()->mapping == TextureMapping::CubeReflectionMapping) || (scene.getBackgroundTexture()!=nullptr && scene.getBackgroundTexture()->mapping == TextureMapping::CubeReflectionMapping)) {
                if (boxMesh == nullptr) {
                    boxMesh = Mesh::create(std::make_shared<BoxBufferGeometry>(1, 1, 1), std::make_shared<ShaderMaterial>());
                    ShaderMaterial::ptr shaderMaterial = std::dynamic_pointer_cast<ShaderMaterial>(boxMesh->material);
                    shaderMaterial->type = "BackgroundCubeMaterial";
                    shaderMaterial->uniforms = std::make_shared<UniformValues>(getShader("cube").uniforms);
                    shaderMaterial->vertexShader = getShader("cube").vertexShader;
                    shaderMaterial->fragmentShader = getShader("cube").fragmentShader;
                    shaderMaterial->side = Side::BackSide;
                    shaderMaterial->depthTest = false;
                    shaderMaterial->depthWrite = false;
                    shaderMaterial->fog = false;

                    BoxBufferGeometry::ptr geometry = std::dynamic_pointer_cast<BoxBufferGeometry>(boxMesh->geometry);
                    geometry->deleteAttribute(AttributeName::normal);
                    geometry->deleteAttribute(AttributeName::uv);
                    //geometry->normal.reset();// geometry->deleteAttribute<float>("normal");
                    //geometry->uv.reset();// geometry->deleteAttribute<float>("uv");

                    //boxMesh->onBeforeRender.connect(*this, &GLBackground::beforeRender);
                    boxMesh->onBeforeRender.connect(*this,&GLBackground::beforeRender);

                    shaderMaterial->envMap = (*shaderMaterial->uniforms)["envMap"].get<Texture::ptr>();//->get<Texture::ptr>("envMap");
                    objects->update(*boxMesh);
                }
                Texture::ptr emptyTexture;
                Texture::ptr texture = scene.isGLCubeRenderTarget ? scene.getBackgroundCubeRenderTarget()->texture : emptyTexture;

                ShaderMaterial::ptr material = std::dynamic_pointer_cast<ShaderMaterial>(boxMesh->material);
                material->uniforms->set("envMap", texture);

                if (scene.isCubeTexture)
                    material->uniforms->set("flipEnvMap", -1.0f);
                else
                    material->uniforms->set("flipEnvMap", 1.0f);

                if ((scene.isColor && !((Color*)currentBackground)->equals(scene.getBackgroundColor()))
                    || (texture!=nullptr && texture->version != currentBackgroundVersion)
                    || (currentTonemapping != renderer.toneMapping)) {
                    material->needsUpdate = true;
                   
                   currentBackground = (void *)&scene.getBackgroundColor();
                   
                   if (texture!=nullptr)
                        currentBackgroundVersion = texture->version;

                    currentTonemapping = renderer.toneMapping;
                }
                renderList.unshift(boxMesh, std::dynamic_pointer_cast<BufferGeometry>(boxMesh->geometry), boxMesh->material, 0, 0, nullptr);
            }
            else if (scene.isTexture) { //scene background is texture
                if (planeMesh == nullptr) {
                    PlaneBufferGeometry::ptr geometry = std::make_shared<PlaneBufferGeometry>(2, 2);
                    planeMesh = std::make_shared<Mesh>(geometry, std::make_shared<ShaderMaterial>());
                    ShaderMaterial::ptr shaderMaterial = std::dynamic_pointer_cast<ShaderMaterial>(planeMesh->material);
                    shaderMaterial->type = "BackgroundMaterial";
                    shaderMaterial->uniforms = std::make_shared<UniformValues>(getShader("background").uniforms);
                    shaderMaterial->vertexShader = getShader("background").vertexShader;
                    shaderMaterial->fragmentShader = getShader("background").fragmentShader;
                    shaderMaterial->side = Side::FrontSide;
                    shaderMaterial->depthTest = false;
                    shaderMaterial->depthWrite = false;
                    shaderMaterial->fog = false;

                    geometry->deleteAttribute(AttributeName::normal);
                    //geometry->normal.reset();// geometry->deleteAttribute<float>("normal");

                    shaderMaterial->map = (*shaderMaterial->uniforms)["t2D"].get<Texture::ptr>();
                    objects->update(*planeMesh);
                }
                ShaderMaterial::ptr material = std::dynamic_pointer_cast<ShaderMaterial>(planeMesh->material);
                material->uniforms->set("t2D", scene.getBackgroundTexture()); //-->shared ptr
                
                if (scene.getBackgroundTexture()->matrixAutoUpdate) {
                    scene.getBackgroundTexture()->updateMatrix();
                }
                material->uniforms->set("uvTransform", scene.getBackgroundTexture()->matrix);
                if ((Texture*)currentBackground != scene.getBackgroundTexture().get() ||
                    currentBackgroundVersion != scene.getBackgroundTexture()->version ||
                    currentTonemapping != renderer.toneMapping) {

                    material->needsUpdate = true;

                    currentBackground = (void *)&scene.getBackgroundTexture();
                    currentBackgroundVersion = scene.getBackgroundTexture()->version;
                    currentTonemapping = renderer.toneMapping;
                }

                renderList.unshift(planeMesh, std::dynamic_pointer_cast<BufferGeometry>(planeMesh->geometry), planeMesh->material, 0, 0, nullptr);
            }          
           
        }
        void GLBackground::beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material, const std::shared_ptr<GLRenderTarget>& renderTarget, DrawRange* group)
        {
           object->matrixWorld.copyPosition(camera->matrixWorld);
        }
    }
}
