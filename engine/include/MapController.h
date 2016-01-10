#ifndef MAP_CONTROLLER_H_
#define MAP_CONTROLLER_H_

namespace yam2d
{
	class Layer;
	class GameObject;
	class Camera;

	void renderCamera(Camera* camera, Layer* layer);

	void updateLayer(Layer* layer, float deltaTime);

	void renderLayerObject(GameObject* gameObject, Layer* layer);

}


#endif

