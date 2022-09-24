#include "Upch.h"
#include "Camera.h"

namespace U
{
	Camera::Camera(const glm::mat4& projectionMatrix)
		:m_ProjectionMatrix(projectionMatrix)
	{
	}
}