#pragma once

#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4x4.h"
#include"Transform.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace MyMath {

	Matrix4x4 MakeIdentity4x4();
	Matrix4x4 MakeScaleMatrix(Vector3 scale);
	Matrix4x4 MakeRotateZMatrix(float radian);
	Matrix4x4 MakeTranslateMatrix(Vector3 translate);


#pragma region Affine

	Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
#pragma endregion

#pragma region 逆数
	Matrix4x4 Inverse(const Matrix4x4& m);
#pragma endregion

	Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
}