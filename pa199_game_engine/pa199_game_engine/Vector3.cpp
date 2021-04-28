#include "Vector3.h"
#include "Cylindrical3.h"
#include <math.h>



Vector3::Vector3()
{
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized()
{
	float mag = magnitude();
	if(mag == 0)
		return Vector3(0,0,0);
	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::operator*(const float& scalar)
{
	return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator+(const Vector3& other)
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other)
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::cross(const Vector3 other)
{
	float c_x = y*other.z - z*other.y;
	float c_y = z*other.x - x*other.z;
	float c_z = x*other.y - y*other.x;

	return Vector3(c_x, c_y, c_z);
}

float Vector3::dot(const Vector3 other)
{
	return x*other.x + y*other.y + z*other.z;
}

Cylindrical3 Vector3::toCylindrical()
{
	float r = sqrt((x * x) + (y * y));
	float angle = atan(y / x);
	Cylindrical3 res = Cylindrical3(r, angle, z);
	return res;
	return Cylindrical3();
}

Vector3 Vector3::opposite()
{
	return (*this) * -1;
}

bool Vector3::operator==(const Vector3& other)
{
	if (x == other.x && y == other.y && z == other.z) {
		return true;
	}
	return false;
}
