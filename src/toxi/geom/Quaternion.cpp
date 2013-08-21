#include "Quaternion.h"


toxi::geom::Quaternion::Quaternion(void)
{
	identity();
}

toxi::geom::Quaternion::Quaternion( float w, float x, float y, float z )
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

toxi::geom::Quaternion::Quaternion( float w, Vec3D v )
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}


toxi::geom::Quaternion::~Quaternion(void)
{
}

toxi::geom::Quaternion toxi::geom::Quaternion::createFromAxisAngle( Vec3D axis, float angle )
{
	angle *= 0.5;
	float sin = toxi::math::MathUtils::sin( angle );
	float cos = toxi::math::MathUtils::cos( angle );
	Quaternion q = Quaternion( cos, axis.getNormalizedTo( sin ) );
	return q;
}

toxi::geom::Quaternion toxi::geom::Quaternion::createFromEuler( float pitch, float yaw, float roll )
{
	pitch *= 0.5;
	yaw *= 0.5;
	roll *= 0.5;
	float sinPitch = toxi::math::MathUtils::sin(pitch);
	float cosPitch = toxi::math::MathUtils::cos(pitch);
	float sinYaw = toxi::math::MathUtils::sin(yaw);
	float cosYaw = toxi::math::MathUtils::cos(yaw);
	float sinRoll = toxi::math::MathUtils::sin(roll);
	float cosRoll = toxi::math::MathUtils::cos(roll);
	float cosPitchCosYaw = cosPitch * cosYaw;
	float sinPitchSinYaw = sinPitch * sinYaw;

	Quaternion q = Quaternion();

	q.x = sinRoll * cosPitchCosYaw - cosRoll * sinPitchSinYaw;
	q.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	q.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	q.w = cosRoll * cosPitchCosYaw + sinRoll * sinPitchSinYaw;

	// alternative solution from:
	// http://is.gd/6HdEB
	//
	// double c1 = Math.cos(yaw/2);
	// double s1 = Math.sin(yaw/2);
	// double c2 = Math.cos(pitch/2);
	// double s2 = Math.sin(pitch/2);
	// double c3 = Math.cos(roll/2);
	// double s3 = Math.sin(roll/2);
	// double c1c2 = c1*c2;
	// double s1s2 = s1*s2;
	// w =c1c2*c3 - s1s2*s3;
	// x =c1c2*s3 + s1s2*c3;
	// y =s1*c2*c3 + c1*s2*s3;
	// z =c1*s2*c3 - s1*c2*s3;

	return q;
}

toxi::geom::Quaternion toxi::geom::Quaternion::createFromMatrix( Matrix4x4 m )
{
	double s = 0.0f;
	double q[4];
	// TODO arrays need to be malloc'ed?
	double trace = m.matrix[0][0] + m.matrix[1][1] + m.matrix[2][2];

	if (trace > 0.0f) {
		s = toxi::math::MathUtils::sqrt(trace + 1.0f);
		q[3] = s * 0.5f;
		s = 0.5f / s;
		q[0] = (m.matrix[1][2] - m.matrix[2][1]) * s;
		q[1] = (m.matrix[2][0] - m.matrix[0][2]) * s;
		q[2] = (m.matrix[0][1] - m.matrix[1][0]) * s;
	} else {
		int nxt[3]= { 1, 2, 0 };
		int i = 0, j = 0, k = 0;

		if (m.matrix[1][1] > m.matrix[0][0]) {
			i = 1;
		}

		if (m.matrix[2][2] > m.matrix[i][i]) {
			i = 2;
		}

		j = nxt[i];
		k = nxt[j];
		s = toxi::math::MathUtils::sqrt((m.matrix[i][i] - (m.matrix[j][j] + m.matrix[k][k])) + 1.0f);

		q[i] = s * 0.5f;
		s = 0.5f / s;
		q[3] = (m.matrix[j][k] - m.matrix[k][j]) * s;
		q[j] = (m.matrix[i][j] + m.matrix[j][i]) * s;
		q[k] = (m.matrix[i][k] + m.matrix[k][i]) * s;
	}

	return Quaternion((float) q[3], (float) q[0], (float) q[1],
		(float) q[2]);
}

toxi::geom::Quaternion toxi::geom::Quaternion::getAlignmentQuat( Vec3D dir, Vec3D forward )
{
	Vec3D target = dir.getNormalized();
	Vec3D axis = forward.cross(target);
	float length = axis.magnitude() + 0.0001f;
	float angle = (float) std::atan2(length, forward.dot(target));
	return createFromAxisAngle(axis, angle);
}

toxi::geom::Quaternion toxi::geom::Quaternion::add( Quaternion q )
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

toxi::geom::Quaternion toxi::geom::Quaternion::addSelf( Quaternion q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

float toxi::geom::Quaternion::dot( Quaternion q )
{
	return (x * q.x) + (y * q.y) + (z * q.z) + (w * q.w);
}

toxi::geom::Quaternion toxi::geom::Quaternion::getConjugate()
{
	Quaternion q = Quaternion();
	q.x = -x;
	q.y = -y;
	q.z = -z;
	q.w = w;
	return q;
}

toxi::geom::Matrix4x4 toxi::geom::Quaternion::getMatrix()
{
	return toMatrix4x4();
}

toxi::geom::Quaternion toxi::geom::Quaternion::getNormalized()
{
	return Quaternion( *this ).normalize();
}

toxi::geom::Quaternion toxi::geom::Quaternion::identity()
{
	w = 1.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::interpolateTo( Quaternion target, float t )
{
	return Quaternion( *this ).interpolateToSelf( target, t );
}

toxi::geom::Quaternion toxi::geom::Quaternion::interpolateTo( Quaternion target, float t, InterpolateStrategy * is )
{
	return Quaternion( *this ).interpolateToSelf( target, is->interpolate(0, 1, t) );
}

toxi::geom::Quaternion toxi::geom::Quaternion::interpolateToSelf( Quaternion target, double t )
{
	double scale;
	double invscale;
	float dot = this->dot( target );
	double theta = std::acos(dot);
	double sintheta = toxi::math::MathUtils::sin(theta);
	if (sintheta > 0.001f) {
		scale = toxi::math::MathUtils::sin(theta * (1.0 - t)) / sintheta;
		invscale = toxi::math::MathUtils::sin(theta * t) / sintheta;
	} else {
		scale = 1 - t;
		invscale = t;
	}
	if (dot < 0) {
		w = (float) (scale * w - invscale * target.w);
		x = (float) (scale * x - invscale * target.x);
		y = (float) (scale * y - invscale * target.y);
		z = (float) (scale * z - invscale * target.z);
	} else {
		w = (float) (scale * w + invscale * target.w);
		x = (float) (scale * x + invscale * target.x);
		y = (float) (scale * y + invscale * target.y);
		z = (float) (scale * z + invscale * target.z);
	}
	return normalize();
}

toxi::geom::Quaternion toxi::geom::Quaternion::interpolateToSelf( Quaternion target, double t, toxi::geom::InterpolateStrategy * is )
{
	float st = is->interpolate(0, 1, t);
	return interpolateToSelf(target, st);
}

float toxi::geom::Quaternion::magnitude()
{
	return (float) toxi::math::MathUtils::sqrt(x * x + y * y + z * z + w * w);
}

toxi::geom::Quaternion toxi::geom::Quaternion::multiply( Quaternion q2 )
{
	Quaternion res = Quaternion();
	res.w = w * q2.w - x * q2.x - y * q2.y - z * q2.z;
	res.x = w * q2.x + x * q2.w + y * q2.z - z * q2.y;
	res.y = w * q2.y + y * q2.w + z * q2.x - x * q2.z;
	res.z = w * q2.z + z * q2.w + x * q2.y - y * q2.x;

	return res;
}

toxi::geom::Quaternion toxi::geom::Quaternion::normalize()
{
	double mag = std::sqrt(x * x + y * y + z * z + w * w);
	if (mag > toxi::math::MathUtils::EPS) {
		mag = 1.0 / mag;
		x *= mag;
		y *= mag;
		z *= mag;
		w *= mag;
	}
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::scale( float t )
{
	return Quaternion(x * t, y * t, z * t, w * t);
}

toxi::geom::Quaternion toxi::geom::Quaternion::scaleSelf( float t )
{
	x *= t;
	y *= t;
	z *= t;
	w *= t;
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::set( float w, float x, float y, float z )
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::set( float w, Vec3D v )
{
	this->w = w;
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::set( Quaternion q )
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
	return *this;
}

toxi::geom::Quaternion toxi::geom::Quaternion::sub( Quaternion q )
{
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

toxi::geom::Quaternion toxi::geom::Quaternion::subSelf( Quaternion q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

float * toxi::geom::Quaternion::toArray()
{
	float arr[4] = {w, x, y, z};
	return arr;
}

float * toxi::geom::Quaternion::toAxisAngle()
{
	float res[4];
	float sa = (float) std::sqrt(1.0f - w * w);
	if (sa < toxi::math::MathUtils::EPS) {
		sa = 1.0f;
	} else {
		sa = 1.0f / sa;
	}
	res[0] = (float) std::acos(w) * 2.0f;
	res[1] = x * sa;
	res[2] = y * sa;
	res[3] = z * sa;
	return res;
}

toxi::geom::Matrix4x4 toxi::geom::Quaternion::toMatrix4x4()
{
	// Converts this quaternion to a rotation matrix.
	//
	// | 1 - 2(y^2 + z^2) 2(xy + wz) 2(xz - wy) 0 |
	// | 2(xy - wz) 1 - 2(x^2 + z^2) 2(yz + wx) 0 |
	// | 2(xz + wy) 2(yz - wx) 1 - 2(x^2 + y^2) 0 |
	// | 0 0 0 1 |

	float x2 = x + x;
	float y2 = y + y;
	float z2 = z + z;
	float xx = x * x2;
	float xy = x * y2;
	float xz = x * z2;
	float yy = y * y2;
	float yz = y * z2;
	float zz = z * z2;
	float wx = w * x2;
	float wy = w * y2;
	float wz = w * z2;

	return Matrix4x4(1 - (yy + zz), xy - wz, xz + wy, 0, xy + wz,
		1 - (xx + zz), yz - wx, 0, xz - wy, yz + wx, 1 - (xx + yy), 0,
		0, 0, 0, 1);
}

std::string toxi::geom::Quaternion::toString()
{
	std::stringstream ss;
	ss << "{axis: [" << x << ", " << y << ", " << z << "], w: " << w << "}";
	return ss.str();
}
