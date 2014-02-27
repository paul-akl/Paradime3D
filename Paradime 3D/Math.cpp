#include "Math.h"

namespace Math3d
{
	void Vec3f::rotate(float angle_arg, const Vec3f& axis_arg)
	{
		const float sinHalfAngle = sinf(toRadian(angle_arg/2));
		
		Quaternion rotationQuat(axis_arg.x * sinHalfAngle, 
								axis_arg.y * sinHalfAngle, 
								axis_arg.z * sinHalfAngle, 
								cosf(toRadian(angle_arg/2)));

		Quaternion conjugateQuat = rotationQuat.conjugate();
		//ConjugateQ.normalize();
		Quaternion ret = rotationQuat * (*this) * conjugateQuat;

		x = ret.x;
		y = ret.y;
		z = ret.z;
	}

	float toRadian(float f_arg)
	{
		return (f_arg * (float)PI / 180.0f);
	}
	float toDegree(float f_arg)
	{
		return (f_arg * 180.0f / (float)PI);
	}
	float getMax(float x_arg, float y_arg)
	{
		return x_arg > y_arg ? x_arg : y_arg;
	}

	const Mat4f Mat4f::operator*=(const Mat4f& mat4f_arg)
	{
		m[0] =	m[0] * mat4f_arg.m[0]  + m[4] * mat4f_arg.m[1]  + m[8]  * mat4f_arg.m[2]  + m[12] * mat4f_arg.m[3];
		m[1] =	m[1] * mat4f_arg.m[0]  + m[5] * mat4f_arg.m[1]  + m[9]  * mat4f_arg.m[2]  + m[13] * mat4f_arg.m[3];
		m[2] =	m[2] * mat4f_arg.m[0]  + m[6] * mat4f_arg.m[1]  + m[10] * mat4f_arg.m[2]  + m[14] * mat4f_arg.m[3];
		m[3] =	m[3] * mat4f_arg.m[0]  + m[7] * mat4f_arg.m[1]  + m[11] * mat4f_arg.m[2]  + m[15] * mat4f_arg.m[3];
		m[4] =	m[0] * mat4f_arg.m[4]  + m[4] * mat4f_arg.m[5]  + m[8]  * mat4f_arg.m[6]  + m[12] * mat4f_arg.m[7];
		m[5] =	m[1] * mat4f_arg.m[4]  + m[5] * mat4f_arg.m[5]  + m[9]  * mat4f_arg.m[6]  + m[13] * mat4f_arg.m[7];
		m[6] =	m[2] * mat4f_arg.m[4]  + m[6] * mat4f_arg.m[5]  + m[10] * mat4f_arg.m[6]  + m[14] * mat4f_arg.m[7];
		m[7] =	m[3] * mat4f_arg.m[4]  + m[7] * mat4f_arg.m[5]  + m[11] * mat4f_arg.m[6]  + m[15] * mat4f_arg.m[7];
		m[8] =	m[0] * mat4f_arg.m[8]  + m[4] * mat4f_arg.m[9]  + m[8]  * mat4f_arg.m[10] + m[12] * mat4f_arg.m[11];
		m[9] =	m[1] * mat4f_arg.m[8]  + m[5] * mat4f_arg.m[9]  + m[9]  * mat4f_arg.m[10] + m[13] * mat4f_arg.m[11];
		m[10] = m[2] * mat4f_arg.m[8]  + m[6] * mat4f_arg.m[9]  + m[10] * mat4f_arg.m[10] + m[14] * mat4f_arg.m[11];
		m[11] = m[3] * mat4f_arg.m[8]  + m[7] * mat4f_arg.m[9]  + m[11] * mat4f_arg.m[10] + m[15] * mat4f_arg.m[11];
		m[12] = m[0] * mat4f_arg.m[12] + m[4] * mat4f_arg.m[13] + m[8]  * mat4f_arg.m[14] + m[12] * mat4f_arg.m[15];
		m[13] = m[1] * mat4f_arg.m[12] + m[5] * mat4f_arg.m[13] + m[9]  * mat4f_arg.m[14] + m[13] * mat4f_arg.m[15];
		m[14] = m[2] * mat4f_arg.m[12] + m[6] * mat4f_arg.m[13] + m[10] * mat4f_arg.m[14] + m[14] * mat4f_arg.m[15];
		m[15] = m[3] * mat4f_arg.m[12] + m[7] * mat4f_arg.m[13] + m[11] * mat4f_arg.m[14] + m[15] * mat4f_arg.m[15];

		return *this;
	}
	void Mat4f::rotate(const Vec3f& vec3f_arg)
	{
		Mat4f rotX, rotY, rotZ;
		
		const float x = toRadian(vec3f_arg.x);
		const float y = toRadian(vec3f_arg.y);
		const float z = toRadian(vec3f_arg.z);
		
		rotX.m[0] = 1.0f; rotX.m[4] = 0.0f   ; rotX.m[8] = 0.0f		; //rotX.m[12] = 0.0f;
		rotX.m[1] = 0.0f; rotX.m[5] = cosf(x); rotX.m[9] = -sinf(x)	;// rotX.m[13] = 0.0f;
		rotX.m[2] = 0.0f; rotX.m[6] = sinf(x); rotX.m[10] = cosf(x) ; //rotX.m[14] = 0.0f;
		//rotX.m[3] = 0.0f; rotX.m[7] = 0.0f   ; rotX.m[11] = 0.0f    ; rotX.m[15] = 1.0f;

		rotY.m[0] = cosf(y); rotY.m[4] = 0.0f; rotY.m[8] = -sinf(y)	; //rotY.m[12] = 0.0f;
		rotY.m[1] = 0.0f   ; rotY.m[5] = 1.0f; rotY.m[9] = 0.0f		; //rotY.m[13] = 0.0f;
		rotY.m[2] = sinf(y); rotY.m[6] = 0.0f; rotY.m[10] = cosf(y) ; //rotY.m[14] = 0.0f;
		//rotY.m[3] = 0.0f   ; rotY.m[7] = 0.0f; rotY.m[11] = 0.0f    ; rotY.m[15] = 1.0f;

		rotZ.m[0] = cosf(z); rotZ.m[4] = -sinf(z); rotZ.m[8] = 0.0f; //rotZ.m[12] = 0.0f;
		rotZ.m[1] = sinf(z); rotZ.m[5] = cosf(z) ; rotZ.m[9] = 0.0f; //rotZ.m[13] = 0.0f;
		rotZ.m[2] = 0.0f   ; rotZ.m[6] = 0.0f    ; rotZ.m[10] = 1.0f; //rotZ.m[14] = 0.0f;
		//rotZ.m[3] = 0.0f   ; rotZ.m[7] = 0.0f    ; rotZ.m[11] = 0.0f; rotZ.m[15] = 1.0f;

		*this = (*this * rotY * rotX * rotZ);
	}
	void Mat4f::perspective(float FOV_arg, int screenWidth_arg, int screenHeight_arg, float zNear_arg, float zFar_arg)
	{
		float	radFOV = toRadian(FOV_arg),
				height = cosf(0.5f * radFOV) / sinf(0.5f * radFOV),
				width = height * screenHeight_arg / screenWidth_arg,
				zRange = zFar_arg - zNear_arg;

		m[0] = width;					 m[4] = 0.0f;			m[8] = 0.0f;								m[12] = 0.0f;
		m[1] = 0.0f;					 m[5] = height;			m[9] = 0.0f;								m[13] = 0.0f;
		m[2] = 0.0f;					 m[6] = 0.0f;			m[10] = -(zNear_arg + zFar_arg) / zRange;	m[14] = -(2.0f * zFar_arg * zNear_arg) / zRange;
		m[3] = 0.0f;					 m[7] = 0.0f;			m[11] = -1.0f;								m[15] = 0.0f;
	}
	void Mat4f::perspective(float FOV_arg, float aspectRatio_arg, float zNear_arg, float zFar_arg)
	{
		float	range = tanf(toRadian(FOV_arg / 2.0f)) * zNear_arg,
				left = -range * aspectRatio_arg,
				right = range * aspectRatio_arg,
				bottom = -range,
				top = range,
				zRange = zFar_arg - zNear_arg;
		
		m[0] = (2.0f * zNear_arg) / (right - left);	 m[4] = 0.0f;									m[8] = 0.0f;								m[12] = 0.0f;
		m[1] = 0.0f;								 m[5] = (2.0f * zNear_arg) / (top - bottom);	m[9] = 0.0f;								m[13] = 0.0f;
		m[2] = 0.0f;								 m[6] = 0.0f;									m[10] = -(zFar_arg + zNear_arg) / zRange;	m[14] = -(2.0f * zFar_arg * zNear_arg) / zRange;
		m[3] = 0.0f;								 m[7] = 0.0f;									m[11] = -1.0f;								m[15] = 0.0f;
	}
	void Mat4f::initCamera(const Vec3f& position_arg, const Vec3f& target_arg, const Vec3f& up_arg)
	{
		Vec3f	N = normalize(target_arg - position_arg),
				U = normalize(up_arg),
				V = normalize(cross(N, U));

		U = cross(V, N);

		m[0] = V.x;					m[4] = V.y;					m[8]  = V.z;				m[12] = -dot(V, position_arg);
		m[1] = U.x;					m[5] = U.y;					m[9]  = U.z;				m[13] = -dot(U, position_arg);
		m[2] = -N.x;				m[6] = -N.y;				m[10] = -N.z;				m[14] = dot(N, position_arg);
		//m[3] = 0.0f;				m[7] = 0.0f;				m[11] = 0.0f;				m[15] = 1.0f;
	}
	void Mat4f::ortho(float left_arg, float right_arg, float down_arg, float up_arg, float zNear_arg, float zFar_arg)
	{
		m[0] = 2 / (right_arg - left_arg);							m[4] = 0.0f;										m[8] = 0.0f;												m[12] = 0.0f;
		m[1] = 0.0f;												m[5] = 2 / (up_arg - down_arg);						m[9] = 0.0f;												m[13] = 0.0f;
		m[2] = 0.0f;												m[6] = 0.0f;										m[10] = -2 / (zFar_arg - zNear_arg);						m[14] = 0.0f;
		m[3] = -(right_arg + left_arg)  / (right_arg - left_arg);	m[7] = -(up_arg + down_arg)  / (up_arg - down_arg);	m[11] = -(zFar_arg + zNear_arg)  / (zFar_arg - zNear_arg);	m[15] = 1.0f;
	}
}