#include <cmath>

class Vector3
{
public:
	double x;
	double y;
	double z;

	Vector3(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	};

	double GetLength() const {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	void Normalize() {
		auto length = GetLength();
		x = x / length;
		y = y / length;
		z = z / length;
	}

	static Vector3 Normalized(Vector3 v) {
		auto vector = Vector3(v.x, v.y, v.z);
		auto length = vector.GetLength();
		vector.x = vector.x / length;
		vector.y = vector.y / length;
		vector.z = vector.z / length;
		return vector;
	}
};
