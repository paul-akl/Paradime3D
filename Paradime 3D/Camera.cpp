#include "Camera.h"

CameraEntity::CameraEntity()
{
	modified = true;
	upVector = Math3d::Vec3f(0, 1, 0);
}
CameraEntity::CameraEntity(Math3d::Vec3f target_arg)
{

}

void CameraEntity::update(Math3d::Vec3f *position_arg, Math3d::Vec3f *target_arg, Math3d::Vec3f *upVector_arg)
{
	position = *position_arg;
	target = *target_arg;
	upVector = *upVector_arg;
}
void CameraEntity::update(Common::InputState *inputs_arg, double *deltaTime_arg)
{
	/*
	updateMovement(inputs_arg, deltaTime_arg);
	updateAngles(inputs_arg);
		
	const Math3d::Vec3f verticalAxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Math3d::Vec3f View(1.0f, 0.0f, 0.0f);
	View.Rotate(horizontalAngle, verticalAxis);
	View.normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Math3d::Vec3f horizontalAxis = verticalAxis.cross(View);
	horizontalAxis.normalize();
	View.Rotate(verticalAngle, horizontalAxis);
       
	targetVector = View;
	targetVector.normalize();

	upVector = targetVector.cross(horizontalAxis);
	upVector.normalize();*/
}
void CameraEntity::update(Common::InputState *inputs_arg, Math3d::Vec3f target_arg)
{/*
	updateAngles(inputs_arg);
	
	Math3d::Vec3f verticalAxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	Math3d::Vec3f view(1.0f, 0.0f, 0.0f);
	view.rotate(horizontalAngle, verticalAxis);
	view.normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Math3d::Vec3f horizontalAxis = verticalAxis.cross(view);
	horizontalAxis.normalize();
	view.rotate(verticalAngle, horizontalAxis);
       
	//targetVector = view;
	//targetVector.normalize();

	upVector = target.cross(horizontalAxis);
	upVector.normalize();*/
}
void CameraEntity::updateViewMatrix()
{
	viewMatrix.initCamera(position, target, upVector);
}