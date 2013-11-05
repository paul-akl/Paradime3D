#ifndef MATH_HEADER
#define MATH_HEADER

//#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#define M_E			2.71828182845904523536
#define M_LOG2E		1.44269504088896340736
#define M_LOG10E	0.434294481903251827651
#define M_LN2		0.693147180559945309417
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.785398163397448309616
#define M_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.707106781186547524401

namespace Math3d
{
	struct Vec2i
	{
	    int x, y;

	    Vec2i()
	    {
			x = 0.0f;
			y = 0.0f;
	    }
	    Vec2i(int x_arg, int y_arg)
	    {
	        x = x_arg;
	        y = y_arg;
	    }
	};
	struct Vec2f
	{
	    float x, y;
	
	    Vec2f()
	    {
			x = 0.0f;
			y = 0.0f;
	    }
	    Vec2f(float x_arg, float y_arg)
	    {
	        x = x_arg;
	        y = y_arg;
	    }
	};
	struct Vec3f
	{
	    float x, y, z;
	
	    Vec3f()
	    {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
	    }
	    Vec3f(float x_arg, float y_arg, float z_arg)
	    {
	        x = x_arg;
	        y = y_arg;
	        z = z_arg;
	    }
	    Vec3f(Vec2f v_arg, float z_arg)
	    {
			x = v_arg.x;
			y = v_arg.y;
	        z = z_arg;
	    }

		const inline Vec3f& operator+=(const Vec3f& v_arg)
		{
			x += v_arg.x;
			y += v_arg.y;
			z += v_arg.z;

			return *this;
		}
		const inline Vec3f& operator-=(const Vec3f& v_arg)
		{
			x -= v_arg.x;
			y -= v_arg.y;
			z -= v_arg.z;

			return *this;
		}
		const inline Vec3f& operator*=(const Vec3f& v_arg)
		{
			x *= v_arg.x;
			y *= v_arg.y;
			z *= v_arg.z;

			return *this;
		}
		const inline Vec3f& operator*=(const float f_arg)
		{
			x *= f_arg;
			y *= f_arg;
			z *= f_arg;

			return *this;
		}
		const inline Vec3f& operator/=(const Vec3f& v_arg)
		{
			x /= v_arg.x;
			y /= v_arg.y;
			z /= v_arg.z;

			return *this;
		}
		const inline Vec3f& operator/=(const float f_arg)
		{
			x /= f_arg;
			y /= f_arg;
			z /= f_arg;

			return *this;
		}
		inline bool operator==(Vec3f& v_arg)
		{
			return (x == v_arg.x && y == v_arg.y && z == v_arg.z);
		}
		inline bool operator!=(Vec3f& v_arg)
		{
			return (x != v_arg.x || y != v_arg.y || z != v_arg.z);
		}
		
		const inline float squareLength() const
		{
			return (x * x + y * y + z * z);
		}
		const inline float length() const
		{
			return sqrtf(squareLength());
		}
		const inline float dot(const Vec3f& v_arg) const
		{
			return (x * v_arg.x + y * v_arg.y + z * v_arg.z);
		}
		const inline Vec3f cross(const Vec3f& v_arg)
		{
			*this = Vec3f(y * v_arg.z - z * v_arg.y, z * v_arg.x - x * v_arg.z, x * v_arg.y - y * v_arg.x);
			return *this;
		}
		const inline Vec3f target(float verticalAngle_arg, float horizontalAngle_arg)
		{
			*this = Vec3f(	cosf(verticalAngle_arg) * sinf(horizontalAngle_arg),
							sinf(verticalAngle_arg),
							cosf(verticalAngle_arg) * cosf(horizontalAngle_arg));
			return *this;
		}
		const inline Vec3f horizontal(float horizontalAngle_arg)
		{
			*this = Vec3f(	sinf(horizontalAngle_arg - M_PI / 2.0f),
							0,
							cosf(horizontalAngle_arg - M_PI / 2.0f));
			return *this;
		}
	
		void inline normalize()
		{
			const float length = this->length();

			x /= length;
			y /= length;
			z /= length;
		}
	    void rotate(float angle_arg, const Vec3f& axis_arg);
	};
	struct Vec4f
	{
	    float x, y, z, w;
	
	    Vec4f()
	    {   
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
	    }
	    Vec4f(float x_arg, float y_arg, float z_arg, float w_arg)
	    {
	        x = x_arg;
	        y = y_arg;
	        z = z_arg;
	        w = w_arg;
	    }
	    Vec4f(Vec3f vector_arg, float w_arg)
	    {
	        x = vector_arg.x;
	        y = vector_arg.y;
	        z = vector_arg.z;
	        w = w_arg;
	    } 
	    Vec4f(Vec2f vector_arg, float z_arg, float w_arg)
	    {
	        x = vector_arg.x;
	        y = vector_arg.y;
	        z = z_arg;
	        w = w_arg;
	    } 
	};

	const inline Vec3f operator+(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return Vec3f(l_arg.x + r_arg.x, l_arg.y + r_arg.y, l_arg.z + r_arg.z);
	}
	const inline Vec3f operator-(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return Vec3f(l_arg.x - r_arg.x, l_arg.y - r_arg.y, l_arg.z - r_arg.z);
	}
	const inline Vec3f operator*(const float f_arg, const Vec3f& v_arg)
	{
		return Vec3f(v_arg.x * f_arg, v_arg.y * f_arg, v_arg.z * f_arg);
	}
	const inline Vec3f operator*(const Vec3f& v_arg, const float f_arg)
	{
		return Vec3f(v_arg.x * f_arg, v_arg.y * f_arg, v_arg.z * f_arg);
	}
	const inline float operator*(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return (l_arg.x * r_arg.x, l_arg.y * r_arg.y, l_arg.z * r_arg.z);
	}
	const inline Vec3f operator/(const float f_arg, const Vec3f& v_arg)
	{
		return (v_arg * (1 / f_arg));
	}
	const inline Vec3f operator/(const Vec3f& v_arg, const float f_arg)
	{
		return v_arg * (1 / f_arg);
	}
	const inline Vec3f operator/(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return Vec3f(l_arg.x / r_arg.x, l_arg.y / r_arg.y, l_arg.z / r_arg.z);
	}
	const inline Vec3f operator^(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return Vec3f(l_arg.y * r_arg.z - l_arg.z * r_arg.y, 
					 l_arg.z * r_arg.x - l_arg.x * r_arg.z, 
					 l_arg.x * r_arg.y - l_arg.y * r_arg.x);
	}
	const inline Vec3f operator-(const Vec3f& v_arg)
	{
		return Vec3f(-v_arg.x, -v_arg.y, -v_arg.z);
	}
	const inline Vec3f normalize(const Vec3f& v_arg)
	{
		const float length = v_arg.length();
		return Vec3f(v_arg.x / length, v_arg.y / length, v_arg.z / length);
	}
	const inline Vec3f cross(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return Vec3f(l_arg.y * r_arg.z - l_arg.z * r_arg.y, 
					 l_arg.z * r_arg.x - l_arg.x * r_arg.z, 
					 l_arg.x * r_arg.y - l_arg.y * r_arg.x);
	}
	const inline float dot(const Vec3f& l_arg, const Vec3f& r_arg)
	{
		return (l_arg.x * r_arg.x + l_arg.y * r_arg.y + l_arg.z * r_arg.z);
	}

	struct Quaternion
	{
		float x, y, z, w;

		Quaternion()
		{

		}
		Quaternion(float x_arg, float y_arg, float z_arg, float w_arg)
		{
			x = x_arg;
			y = y_arg;
			z = z_arg;
			w = w_arg;
		}

		inline void normalize()
		{
			float Length = sqrtf(x * x + y * y + z * z + w * w);

			x /= Length;
			y /= Length;
			z /= Length;
			w /= Length;
		}
		inline Quaternion conjugate()
		{
			return Quaternion(-x, -y, -z, w);
		}

		const inline Quaternion operator*(const Quaternion& q_arg)
		{
			return Quaternion(	(x * q_arg.w) + (w * q_arg.x) + (y * q_arg.z) - (z * q_arg.y),
								(y * q_arg.w) + (w * q_arg.y) + (z * q_arg.x) - (x * q_arg.z),
								(z * q_arg.w) + (w * q_arg.z) + (x * q_arg.y) - (y * q_arg.x),
								(w * q_arg.w) - (x * q_arg.x) - (y * q_arg.y) - (z * q_arg.z));
		}
		const inline Quaternion operator*(const Vec3f& v_arg)
		{
			return Quaternion(	 (w * v_arg.x) + (y * v_arg.z) - (z * v_arg.y),
								 (w * v_arg.y) + (z * v_arg.x) - (x * v_arg.z),
								 (w * v_arg.z) + (x * v_arg.y) - (y * v_arg.x),
								-(x * v_arg.x) - (y * v_arg.y) - (z * v_arg.z));
		}
	};
	
	const inline Quaternion operator*(const Quaternion& l_arg, const Quaternion& r_arg)
	{
		return Quaternion(	(l_arg.x * r_arg.w) + (l_arg.w * r_arg.x) + (l_arg.y * r_arg.z) - (l_arg.z * r_arg.y),
							(l_arg.y * r_arg.w) + (l_arg.w * r_arg.y) + (l_arg.z * r_arg.x) - (l_arg.x * r_arg.z),
							(l_arg.z * r_arg.w) + (l_arg.w * r_arg.z) + (l_arg.x * r_arg.y) - (l_arg.y * r_arg.x),
							(l_arg.w * r_arg.w) - (l_arg.x * r_arg.x) - (l_arg.y * r_arg.y) - (l_arg.z * r_arg.z));
	}
	const inline Quaternion operator*(const Quaternion& q_arg, const Vec3f& v_arg)
	{
		return Quaternion(	 (q_arg.w * v_arg.x) + (q_arg.y * v_arg.z) - (q_arg.z * v_arg.y),
							 (q_arg.w * v_arg.y) + (q_arg.z * v_arg.x) - (q_arg.x * v_arg.z),
							 (q_arg.w * v_arg.z) + (q_arg.x * v_arg.y) - (q_arg.y * v_arg.x),
							-(q_arg.x * v_arg.x) - (q_arg.y * v_arg.y) - (q_arg.z * v_arg.z));
	}

	float toRadian(float f_arg);
	float toDegree(float f_arg);
	float getMax(float x_arg, float y_arg);

	class Mat4f
	{
	public:

		float m[16];

		Mat4f()
		{
			m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f;	m[12] = 0.0f;
			m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f;	m[13] = 0.0f;
			m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f;	m[14] = 0.0f;
			m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f;	m[15] = 1.0f;
		}
		Mat4f(float m_arg[16])
		{
			m[0] = m_arg[0]; m[4] = m_arg[4]; m[8]  = m_arg[8];	 m[12] = m_arg[12];
			m[1] = m_arg[1]; m[5] = m_arg[5]; m[9]  = m_arg[9];	 m[13] = m_arg[13];
			m[2] = m_arg[2]; m[6] = m_arg[6]; m[10] = m_arg[10]; m[14] = m_arg[14];
			m[3] = m_arg[3]; m[7] = m_arg[7]; m[11] = m_arg[11]; m[15] = m_arg[15];
		}
		Mat4f(	float m01, float m02, float m03, float m04, 
				float m05, float m06, float m07, float m08, 
				float m09, float m10, float m11, float m12, 
				float m13, float m14, float m15, float m16)
		{
			m[0] = m01; m[4] = m05; m[8]  = m09; m[12] = m13;
			m[1] = m02; m[5] = m06; m[9]  = m10; m[13] = m14;
			m[2] = m03; m[6] = m07; m[10] = m11; m[14] = m15;
			m[3] = m04; m[7] = m08; m[11] = m12; m[15] = m16;
		}
		Mat4f(Vec3f& rotX_arg, Vec3f& rotY_arg, Vec3f& rotZ_arg, Vec3f& position_arg)
		{
			m[0] = rotX_arg.x; m[4] = rotX_arg.y; m[8]  = rotX_arg.z; m[12] = position_arg.x;
			m[1] = rotY_arg.x; m[5] = rotY_arg.y; m[9]  = rotY_arg.z; m[13] = position_arg.y;
			m[2] = rotZ_arg.x; m[6] = rotZ_arg.y; m[10] = rotZ_arg.z; m[14] = position_arg.z;
		}

		inline void Mat4f::identity()
		{
			m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f;	m[12] = 0.0f;
			m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f;	m[13] = 0.0f;
			m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f;	m[14] = 0.0f;
			m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f;	m[15] = 1.0f;
		}
		inline void Mat4f::transform(const Vec3f& vec3f_arg)
		{
			m[12] = vec3f_arg.x;
			m[13] = vec3f_arg.y;
			m[14] = vec3f_arg.z;
		}
		inline void Mat4f::scale(float scale_arg)
		{
			m[0]  *= scale_arg;
			m[1]  *= scale_arg;
			m[2]  *= scale_arg;
			m[3]  *= scale_arg;
			m[4]  *= scale_arg;
			m[5]  *= scale_arg;
			m[6]  *= scale_arg;
			m[7]  *= scale_arg;
			m[8]  *= scale_arg;
			m[9]  *= scale_arg;
			m[10] *= scale_arg;
		}
		inline void Mat4f::scale(Vec3f& v_arg)
		{
			m[0]  *= v_arg.x;
			m[1]  *= v_arg.x;
			m[2]  *= v_arg.x;
			m[3]  *= v_arg.x;
			m[4]  *= v_arg.y;
			m[5]  *= v_arg.y;
			m[6]  *= v_arg.y;
			m[7]  *= v_arg.y;
			m[8]  *= v_arg.z;
			m[9]  *= v_arg.z;
			m[10] *= v_arg.z;
		}

		void rotate(const Vec3f& vec3f_arg);
		void perspective(float FOV_arg, int screenWidth_arg, int screenHeight_arg, float zNear, float zFar);
		void perspective(float FOV_arg, float aspectRatio_arg, float zNear_arg, float zFar_arg);
		void initCamera(const Vec3f& position_arg, const Vec3f& target_arg, const Vec3f& up_arg);
		
		const Mat4f operator*=(const Mat4f& mat4f_arg);
	};

	const inline Mat4f operator*(const Mat4f& l_arg, const Mat4f r_arg)
	{
		// Multiplication can be done more stylish with a nested loop, but this should be slightly faster
		return Mat4f(	l_arg.m[0] * r_arg.m[0]  + l_arg.m[4] * r_arg.m[1]  + l_arg.m[8]  * r_arg.m[2]  + l_arg.m[12] * r_arg.m[3],
						l_arg.m[1] * r_arg.m[0]  + l_arg.m[5] * r_arg.m[1]  + l_arg.m[9]  * r_arg.m[2]  + l_arg.m[13] * r_arg.m[3],
						l_arg.m[2] * r_arg.m[0]  + l_arg.m[6] * r_arg.m[1]  + l_arg.m[10] * r_arg.m[2]  + l_arg.m[14] * r_arg.m[3],
						l_arg.m[3] * r_arg.m[0]  + l_arg.m[7] * r_arg.m[1]  + l_arg.m[11] * r_arg.m[2]  + l_arg.m[15] * r_arg.m[3],
						l_arg.m[0] * r_arg.m[4]  + l_arg.m[4] * r_arg.m[5]  + l_arg.m[8]  * r_arg.m[6]  + l_arg.m[12] * r_arg.m[7],
						l_arg.m[1] * r_arg.m[4]  + l_arg.m[5] * r_arg.m[5]  + l_arg.m[9]  * r_arg.m[6]  + l_arg.m[13] * r_arg.m[7],
						l_arg.m[2] * r_arg.m[4]  + l_arg.m[6] * r_arg.m[5]  + l_arg.m[10] * r_arg.m[6]  + l_arg.m[14] * r_arg.m[7],
						l_arg.m[3] * r_arg.m[4]  + l_arg.m[7] * r_arg.m[5]  + l_arg.m[11] * r_arg.m[6]  + l_arg.m[15] * r_arg.m[7],
						l_arg.m[0] * r_arg.m[8]  + l_arg.m[4] * r_arg.m[9]  + l_arg.m[8]  * r_arg.m[10] + l_arg.m[12] * r_arg.m[11],
						l_arg.m[1] * r_arg.m[8]  + l_arg.m[5] * r_arg.m[9]  + l_arg.m[9]  * r_arg.m[10] + l_arg.m[13] * r_arg.m[11],
						l_arg.m[2] * r_arg.m[8]  + l_arg.m[6] * r_arg.m[9]  + l_arg.m[10] * r_arg.m[10] + l_arg.m[14] * r_arg.m[11],
						l_arg.m[3] * r_arg.m[8]	 + l_arg.m[7] * r_arg.m[9]  + l_arg.m[11] * r_arg.m[10] + l_arg.m[15] * r_arg.m[11],
						l_arg.m[0] * r_arg.m[12] + l_arg.m[4] * r_arg.m[13] + l_arg.m[8]  * r_arg.m[14] + l_arg.m[12] * r_arg.m[15],
						l_arg.m[1] * r_arg.m[12] + l_arg.m[5] * r_arg.m[13] + l_arg.m[9]  * r_arg.m[14] + l_arg.m[13] * r_arg.m[15],
						l_arg.m[2] * r_arg.m[12] + l_arg.m[6] * r_arg.m[13] + l_arg.m[10] * r_arg.m[14] + l_arg.m[14] * r_arg.m[15],
						l_arg.m[3] * r_arg.m[12] + l_arg.m[7] * r_arg.m[13] + l_arg.m[11] * r_arg.m[14] + l_arg.m[15] * r_arg.m[15]);
	}
}

#endif