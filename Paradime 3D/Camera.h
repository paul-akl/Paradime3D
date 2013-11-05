#ifndef CAMERA_CLASS
#define CAMERA_CLASS

#include "CommonClasses.h"
#include "Config.h"
//#include "Loader_Model.h"
#include "Math.h"

class CameraEntity
{

public:

	CameraEntity();
	CameraEntity(Math3d::Vec3f target_arg);
	
	void update(Math3d::Vec3f *position_arg, Math3d::Vec3f *target_arg, Math3d::Vec3f *upVector_arg);
	void update(Common::InputState *inputs_arg, double *deltaTime_arg);
	void update(Common::InputState *inputs_arg, Math3d::Vec3f target_arg);
	void updateViewMatrix();

	Math3d::Vec3f		position,
						target,
						upVector;

	Math3d::Mat4f		viewMatrix;

	bool				modified;

private:

};

#endif