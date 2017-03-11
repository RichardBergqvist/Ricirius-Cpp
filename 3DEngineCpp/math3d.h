#ifndef MATH3D_H_INCLUDED
#define MATH3D_H_INCLUDED

#include <math.h>
#define MATH_PI 3.1415926535897932384626433832795
#define toRadians(x) (float)(((x) * MATH_PI / 180.0f))
#define toDegrees(x) (float)(((x) * 180.0f / MATH_PI))

template<typename T>
inline T clamp(const T &a, const T &min, const T &max) {
	if (a < min) return min;
	else if (a > max) return max; 
	else return a;
}

template<typename T, unsigned int D>
class Vector {
public:
	Vector() { }

	inline T dot(const Vector<T, D>& r) const {
		T result = T(0);
		for(unsigned int i = 0; i < D; i++)
			result += (*this)[i] * r[i];
			
		return result;
	}
	
	inline T max() const {
		T max = (*this)[0];
		
		for(int i = 0; i < D; i++)
			if((*this)[i] > max)
				max = (*this)[i];
		
		return max;
	}
	
	inline T LengthSq() const { return this->dot(*this); }
	inline T Length() const { return sqrt(LengthSq()); }
	inline Vector<T,D> normalized() const { return *this/Length(); }
	inline Vector<T,D> lerp(const Vector<T,D>& r, T lerpFactor) const { return (r - *this) * lerpFactor + *this; }

	inline Vector<T, D> operator+(const Vector<T,D>& r) const {
		Vector<T, D> result;
		for(unsigned int i = 0; i < D; i++)
			result[i] = values[i] + r[i];
		
		return result;
	}
	
	inline Vector<T, D> operator-(const Vector<T,D>& r) const {
		Vector<T, D> result;
		for(unsigned int i = 0; i < D; i++)
			result[i] = values[i] - r[i];
		
		return result;
	}
	
	inline Vector<T, D> operator*(const T& r) const {
		Vector<T, D> result;
		for(unsigned int i = 0; i < D; i++)
			result[i] = values[i] * r;
		
		return result;
	}
	
	inline Vector<T, D> operator/(const T& r) const {
		Vector<T, D> result;
		for(unsigned int i = 0; i < D; i++)
			result[i] = values[i] / r;
		
		return result;
	}
	
	inline Vector<T, D>& operator+=(const Vector<T,D>& r)  {
		for(unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] + r[i];
		
		return *this;
	}
	
	inline Vector<T, D>& operator-=(const Vector<T,D>& r)  {
		for(unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] - r[i];
		
		return *this;
	}
	
	inline Vector<T, D>& operator*=(const T& r) {
		for(unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] * r;
		
		return *this;
	}
	
	inline Vector<T, D>& operator/=(const T& r)  {
		for(unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] / r;
		
		return *this;
	}
	
	T& operator [] (unsigned int i) { return values[i]; }
	T operator [] (unsigned int i) const { return values[i]; }
	
	inline bool operator==(const Vector<T,D>& r) const {
		for(unsigned int i = 0; i < D; i++)
			if((*this)[i] != r[i])
				return false;
		return true;
	}
	
	inline bool operator!=(const Vector<T,D>& r) const { return !operator==(r); }
protected:
private:
	T values[D];
};

template<typename T>
class Vector2 : public Vector<T, 2> {
public:
	Vector2() { }
	
	Vector2(const Vector<T, 2>& r) {
		(*this)[0] = r[0];
		(*this)[1] = r[1];
	}
	
	Vector2(T x, T y) {
		(*this)[0] = x;
		(*this)[1] = y;
	}
	
	T cross(const Vector2<T>& r) const {
		return getX() * r.getY() - getY() * r.getX();
	}

	inline void setX(const T& x) { (*this)[0] = x; }
	inline void setY(const T& y) { (*this)[1] = y; }
	
	inline T getX() const { return (*this)[0]; }
	inline T getY() const { return (*this)[1]; }
	
	inline void set(const T& x, const T& y) { setX(x); setY(y); }
protected:
private:
};

template<typename T>
class Vector3 : public Vector<T, 3> {
public:
	Vector3() { }
	
	Vector3(const Vector<T, 3>& r) {
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}
	
	Vector3(T x, T y, T z) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}
	
	inline Vector3<T> cross(const Vector3<T>& r) const {
		T x = (*this)[1] * r[2] - (*this)[2] * r[1];
		T y = (*this)[2] * r[0] - (*this)[0] * r[2];
		T z = (*this)[0] * r[1] - (*this)[1] * r[0];

		return Vector3<T>(x, y, z);
	}
	
	inline Vector3<T> rotate(T angle, const Vector3<T>& axis) const {
		const T sinAngle = sin(-angle);
		const T cosAngle = cos(-angle);
		
		return this->cross(axis * sinAngle) +        
			(*this * cosAngle) +               
		    axis * this->dot(axis * (1 - cosAngle));
	}
	
	inline Vector2<T> getXY() const { return Vector2<T>(getX(), getY()); }
	inline Vector2<T> getYZ() const { return Vector2<T>(getY(), getZ()); }
	inline Vector2<T> getZX() const { return Vector2<T>(getZ(), getX()); }
	
	inline Vector2<T> getYX() const { return Vector2<T>(getY(), getX()); }
	inline Vector2<T> getZY() const { return Vector2<T>(getZ(), getY()); }
	inline Vector2<T> getXZ() const { return Vector2<T>(getX(), getZ()); }
	
	inline void setX(const T& x) { (*this)[0] = x; }
	inline void setY(const T& y) { (*this)[1] = y; }
	inline void setZ(const T& z) { (*this)[2] = z; }

	inline T getX() const { return (*this)[0]; }
	inline T getY() const { return (*this)[1]; }
	inline T getZ() const { return (*this)[2]; }
	
	inline void set(const T& x, const T& y, const T& z) { setX(x); setY(y); setZ(z); }
protected:
private:
};

template<typename T>
class Vector4 : public Vector<T, 4> {
public:
	Vector4() { }
	
	Vector4(const Vector<T, 4>& r) {
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
		(*this)[3] = r[3];
	}
	
	Vector4(T x, T y, T z, T w) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}
	inline void setX(const T& x) { (*this)[0] = x; }
	inline void setY(const T& y) { (*this)[1] = y; }
	inline void setZ(const T& z) { (*this)[2] = z; }
	inline void setW(const T& w) { (*this)[3] = w; }
	
	inline T getX() const { return (*this)[0]; }
	inline T getY() const { return (*this)[1]; }
	inline T getZ() const { return (*this)[2]; }
	inline T getW() const { return (*this)[3]; }
	
	inline void set(const T& x, const T& y, const T& z, const T& w) { setX(x); setY(y); setZ(z); setW(w); }
protected:
private:
};

template<typename T, unsigned int D>
class Matrix {
public:
	inline Matrix<T, D> initIdentity() {
		for(unsigned int i = 0; i < D; i++) {
			for(unsigned int j = 0; j < D; j++) {
				if(i == j)
					m[i][j] = T(1);
				else
					m[i][j] = T(0);
			}
		}
		
		return *this;
	}
	
	inline Matrix<T, D> initScale(const Vector<T,D - 1>& r) {
		for(unsigned int i = 0; i < D; i++) {
			for(unsigned int j = 0; j < D; j++) {
				if(i == j && i != D - 1)
					m[i][j] = r[i];
				else
					m[i][j] = T(0);
			}
		}
		
		m[D - 1][D - 1] = T(1);
		
		return *this;
	}
	
	inline Matrix<T, D> initTranslation(const Vector<T,D - 1>& r) {
		for(unsigned int i = 0; i < D; i++) {
			for(unsigned int j = 0; j < D; j++) {
				if(i == D - 1 && j != D - 1)
					m[i][j] = r[j];
				else if(i == j)
					m[i][j] = T(1);
				else
					m[i][j] = T(0);
			}
		}
		
		m[D - 1][D - 1] = T(1);
		
		return *this;
	}

	inline Matrix<T, D> transpose() const {
		Matrix<T, D> t;
		for (int j = 0; j < D; j++) {
			for (int i = 0; i < D; i++) {
				t[i][j] = m[j][i];
			}
		}
		return t;
	}

	inline Matrix<T, D> inverse() const {
		int i, j, k;
		Matrix<T, D> s;
		Matrix<T, D> t(*this);

		for (i = 0; i < D - 1 ; i++) {
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < D; j++) {
				T tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				return Matrix<T, D>();
			}

			if (pivot != i) {
				for (j = 0; j < D; j++) {
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < D; j++) {
				T f = t[j][i] / t[i][i];

				for (k = 0; k < D; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		for (i = D - 1; i >= 0; --i) {
			T f;

			if ((f = t[i][i]) == 0) {
				return Matrix<T, D>();
			}

			for (j = 0; j < D; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < D; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	inline Matrix<T,D> operator*(const Matrix<T,D>& r) const {
		Matrix<T,D> ret;
		for (unsigned int i = 0 ; i < D ; i++)  {
			for (unsigned int j = 0 ; j < D; j++)  {
				ret.m[i][j] = T(0);
				for(unsigned int k = 0; k < D; k++)
					ret.m[i][j] += m[k][j] * r.m[i][k];
			}
		}
		return ret;
	}
	
	inline Vector<T,D> transform(const Vector<T,D>& r) const {
		Vector<T,D> ret;
		
		for(unsigned int i = 0; i < D; i++) {
			ret[i] = 0;
			for(unsigned int j = 0; j < D; j++)
				ret[i] += m[j][i] * r[j];
		}
		
		return ret;
	}
	
	inline Vector<T,D-1> transform(const Vector<T,D-1>& r) const {
		Vector<T,D> r2;
		
		for(int i = 0; i < D-1; i++)
			r2[i] = r[i];
			
		r2[D-1] = T(1);
		
		Vector<T,D> ret2 = transform(r2);
		Vector<T,D-1> ret;
		
		for(int i = 0; i < D-1; i++)
			ret[i] = ret2[i];
			
		return ret;
	}
	
	inline void set(unsigned int x, unsigned int y, T val) { m[x][y] = val; }
	
	inline const T* operator[](int index) const { return m[index]; }
	inline T* operator[](int index) { return m[index]; }
protected:
private:
	T m[D][D];
};

template<typename T>
class Matrix4 : public Matrix<T, 4> {
public:
	Matrix4() { }

	template<unsigned int D>
	Matrix4(const Matrix<T, D>& r) {
		if(D < 4) {
			this->initIdentity();
		
			for(unsigned int i = 0; i < D; i++)
				for(unsigned int j = 0; j < D; j++)
					(*this)[i][j] = r[i][j];
		} else {
			for(unsigned int i = 0; i < 4; i++)
				for(unsigned int j = 0; j < 4; j++)
					(*this)[i][j] = r[i][j];
		}
	}

	inline Matrix4<T> initRotationEuler(T rotateX, T rotateY, T rotateZ) {
		Matrix4<T> rx, ry, rz;

		const T x = rotateX;
		const T y = rotateY;
		const T z = rotateZ;

		rx[0][0] = T(1);   rx[1][0] = T(0)  ;  rx[2][0] = T(0)   ; rx[3][0] = T(0);
		rx[0][1] = T(0);   rx[1][1] = cos(x);  rx[2][1] = -sin(x); rx[3][1] = T(0);
		rx[0][2] = T(0);   rx[1][2] = sin(x);  rx[2][2] = cos(x) ; rx[3][2] = T(0);
		rx[0][3] = T(0);   rx[1][3] = T(0)  ;  rx[2][3] = T(0)   ; rx[3][3] = T(1);
		
		ry[0][0] = cos(y); ry[1][0] = T(0);    ry[2][0] = -sin(y); ry[3][0] = T(0);
		ry[0][1] = T(0)  ; ry[1][1] = T(1);    ry[2][1] = T(0)   ; ry[3][1] = T(0);
		ry[0][2] = sin(y); ry[1][2] = T(0);    ry[2][2] = cos(y) ; ry[3][2] = T(0);
		ry[0][3] = T(0)  ; ry[1][3] = T(0);    ry[2][3] = T(0)   ; ry[3][3] = T(1);
		
		rz[0][0] = cos(z); rz[1][0] = -sin(z); rz[2][0] = T(0);    rz[3][0] = T(0);
		rz[0][1] = sin(z); rz[1][1] = cos(z) ; rz[2][1] = T(0);    rz[3][1] = T(0);
		rz[0][2] = T(0)  ; rz[1][2] = T(0)   ; rz[2][2] = T(1);    rz[3][2] = T(0);
		rz[0][3] = T(0)  ; rz[1][3] = T(0)   ; rz[2][3] = T(0);    rz[3][3] = T(1);

		*this = rz * ry * rx;
		
		return *this;
	}
	
	inline Matrix4<T> initRotationFromVectors(const Vector3<T>& n, const Vector3<T>& v, const Vector3<T>& u) {
		(*this)[0][0] = u.getX();   (*this)[1][0] = u.getY();   (*this)[2][0] = u.getZ();   (*this)[3][0] = T(0);
		(*this)[0][1] = v.getX();   (*this)[1][1] = v.getY();   (*this)[2][1] = v.getZ();   (*this)[3][1] = T(0);
		(*this)[0][2] = n.getX();   (*this)[1][2] = n.getY();   (*this)[2][2] = n.getZ();   (*this)[3][2] = T(0);
		(*this)[0][3] = T(0);       (*this)[1][3] = T(0);       (*this)[2][3] = T(0);       (*this)[3][3] = T(1);  
		
		return *this;
	}
	
	inline Matrix4<T> initRotationFromDirection(const Vector3<T>& forward, const Vector3<T>& up) {
		Vector3<T> n = forward.normalized();
		Vector3<T> u = Vector3<T>(up.normalized()).cross(n);
		Vector3<T> v = n.cross(u);
		
		return initRotationFromVectors(n,v,u);
	}
	
	inline Matrix4<T> initPerspective(T fov, T aspectRatio, T zNear, T zFar) {
		const T zRange     = zNear - zFar;
		const T tanHalfFOV = tanf(fov / T(2));

		(*this)[0][0] = T(1)/(tanHalfFOV * aspectRatio); (*this)[1][0] = T(0);   (*this)[2][0] = T(0);            (*this)[3][0] = T(0);
		(*this)[0][1] = T(0);                   (*this)[1][1] = T(1)/tanHalfFOV; (*this)[2][1] = T(0);            (*this)[3][1] = T(0);
		(*this)[0][2] = T(0);                   (*this)[1][2] = T(0);            (*this)[2][2] = (-zNear - zFar)/zRange ; (*this)[3][2] = T(2)*zFar*zNear/zRange;
		(*this)[0][3] = T(0);                   (*this)[1][3] = T(0);            (*this)[2][3] = T(1);            (*this)[3][3] = T(0); 
		
		return *this;
	}
	
	inline Matrix4<T> initOrthographic(T left, T right, T bottom, T top, T near, T far) {
		const T width = (right - left);
		const T height = (top - bottom);
		const T depth = (far - near);

		(*this)[0][0] = T(2)/width; (*this)[1][0] = T(0);        (*this)[2][0] = T(0);        (*this)[3][0] = -(right + left)/width;
		(*this)[0][1] = T(0);       (*this)[1][1] = T(2)/height; (*this)[2][1] = T(0);        (*this)[3][1] = -(top + bottom)/height;
		(*this)[0][2] = T(0);       (*this)[1][2] = T(0);        (*this)[2][2] = T(-2)/depth; (*this)[3][2] = -(far + near)/depth;
		(*this)[0][3] = T(0);       (*this)[1][3] = T(0);        (*this)[2][3] = T(0);        (*this)[3][3] = T(1); 
		
		return *this;
	}
protected:
private:
};

template<typename T>
class Matrix3 : public Matrix<T, 3> {
public:
	Matrix3() { }
	
	template<unsigned int D>
	Matrix3(const Matrix<T, D>& r) {
		if(D < 3) {
			this->InitIdentity();
		
			for(unsigned int i = 0; i < D; i++)
				for(unsigned int j = 0; j < D; j++)
					(*this)[i][j] = r[i][j];
		} else {
			for(unsigned int i = 0; i < 3; i++)
				for(unsigned int j = 0; j < 3; j++)
					(*this)[i][j] = r[i][j];
		}
	}
};

class Quaternion;

class Vector3f : public Vector3<float> {
public:
	Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}
	
	Vector3f(const Vector3<float>& r) {
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}

	inline float Length() const { return sqrtf(getX() * getX() + getY() * getY() + getZ() * getZ()); }
	inline float dot(const Vector3f& v) const { return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ(); }

	inline Vector3f cross(const Vector3f& v) const {
		const float _x = getY() * v.getZ() - getZ() * v.getY();
		const float _y = getZ() * v.getX() - getX() * v.getZ();
		const float _z = getX() * v.getY() - getY() * v.getX();

		return Vector3f(_x, _y, _z);
	}

	inline Vector3f rotate(float angle, const Vector3f& axis) const {
		const float sin = sinf(-angle);
		const float cos = cosf(-angle);

		return this->cross(axis * sin) +        
			(*this * cos) +                  
		    axis * this->dot(axis * (1 - cos));
	}

	Vector3f rotate(const Quaternion& rotation) const;

	inline Vector3f normalized() const {
		const float length = Length();

		return Vector3f(getX() / length, getY() / length, getZ() / length);
	}

	inline Vector3f operator+(const Vector3f& r) const { return Vector3f(getX() + r.getX(), getY() + r.getY(), getZ() + r.getZ()); }
	inline Vector3f operator-(const Vector3f& r) const { return Vector3f(getX() - r.getX(), getY() - r.getY(), getZ() - r.getZ()); }
	inline Vector3f operator*(float f) const { return Vector3f(getX() * f, getY() * f, getZ() * f); }
	inline Vector3f operator/(float f) const { return Vector3f(getX() / f, getY() / f, getZ() / f); }

	inline bool operator==(const Vector3f& r) const { return getX() == r.getX() && getY() == r.getY() && getZ() == r.getZ(); }
	inline bool operator!=(const Vector3f& r) const { return !operator==(r); }

	inline Vector3f& operator+=(const Vector3f& r) {
		(*this)[0] += r.getX();
		(*this)[1] += r.getY();
		(*this)[2] += r.getZ();

		return *this;
	}

    inline Vector3f& operator-=(const Vector3f& r) {
		(*this)[0] -= r.getX();
		(*this)[1] -= r.getY();
		(*this)[2] -= r.getZ();

		return *this;
    }
    
    inline Vector3f& operator*=(float f) {
		(*this)[0] *= f;
		(*this)[1] *= f;
		(*this)[2] *= f;

		return *this;
    }
    
	inline Vector3f& operator/=(float f) {
		(*this)[0] /= f;
		(*this)[1] /= f;
		(*this)[2] /= f;

		return *this;
	}

	inline void setX(float x) { (*this)[0] = x; }
	inline void setY(float y) { (*this)[1] = y; }
	inline void setZ(float z) { (*this)[2] = z; }

	inline float getX() const { return (*this)[0]; }
	inline float getY() const { return (*this)[1]; }
	inline float getZ() const { return (*this)[2]; }

	inline void set(float x, float y, float z) { (*this)[0] = x; (*this)[1] = y; (*this)[2] = z; }
private:
	
};

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

typedef Vector2<float> Vector2f;
typedef Vector4<float> Vector4f;

typedef Vector2<double> Vector2d;
typedef Vector3<double> Vector3d;
typedef Vector4<double> Vector4d;

typedef Matrix<int, 2> Matrix2i;
typedef Matrix3<int> Matrix3i;
typedef Matrix4<int> Matrix4i;

typedef Matrix<float, 2> Matrix2f;
typedef Matrix3<float> Matrix3f;
typedef Matrix4<float> Matrix4f;

typedef Matrix<double, 2> Matrix2d;
typedef Matrix3<double> Matrix3d;
typedef Matrix4<double> Matrix4d;

class Quaternion : public Vector4<float> {
public:
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}
	
	Quaternion(const Vector4<float>& r) {
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
		(*this)[3] = r[3];
	}
	
	Quaternion(const Vector3f& axis, float angle) {
		float sinHalfAngle = sinf(angle/2);
		float cosHalfAngle = cosf(angle/2);
		
		(*this)[0] = axis.getX() * sinHalfAngle;
		(*this)[1] = axis.getY() * sinHalfAngle;
		(*this)[2] = axis.getZ() * sinHalfAngle;
		(*this)[3] = cosHalfAngle;
	}
	
	Quaternion(const Matrix4f& m) {
		float trace = m[0][0] + m[1][1] + m[2][2];
		
		if(trace > 0) {
			float s = 0.5f / sqrtf(trace + 1.0f);
			(*this)[3] = 0.25f / s;
			(*this)[0] = (m[1][2] - m[2][1]) * s;
			(*this)[1] = (m[2][0] - m[0][2]) * s;
			(*this)[2] = (m[0][1] - m[1][0]) * s;
		} else if(m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
			(*this)[3] = (m[1][2] - m[2][1]) / s;
			(*this)[0] = 0.25f * s;
			(*this)[1] = (m[1][0] + m[0][1]) / s;
			(*this)[2] = (m[2][0] + m[0][2]) / s;
		} else if(m[1][1] > m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
			(*this)[3] = (m[2][0] - m[0][2]) / s;
			(*this)[0] = (m[1][0] + m[0][1]) / s;
			(*this)[1] = 0.25f * s;
			(*this)[2] = (m[2][1] + m[1][2]) / s;
		} else {
			float s = 2.0f * sqrtf(1.0f + m[2][2] - m[1][1] - m[0][0]);
			(*this)[3] = (m[0][1] - m[1][0]) / s;
			(*this)[0] = (m[2][0] + m[0][2]) / s;
			(*this)[1] = (m[1][2] + m[2][1]) / s;
			(*this)[2] = 0.25f * s;
		}
		
		float length = Length();
		(*this)[3] = (*this)[3] / length;
		(*this)[0] = (*this)[0] / length;
		(*this)[1] = (*this)[1] / length;
		(*this)[2] = (*this)[2] / length;
	}
	
	inline Quaternion nLerp(const Quaternion& r, float lerpFactor, bool shortestPath) const {
		Quaternion correctedDest;
		
		if(shortestPath && this->dot(r) < 0)
			correctedDest = r * -1;
		else
			correctedDest = r;
	
		return Quaternion(lerp(correctedDest, lerpFactor).normalized());
	}
	
	inline Quaternion sLerp(const Quaternion& r, float lerpFactor, bool shortestPath) const {
		static const float EPSILON = 1e3;
	
		float cos = this->dot(r);
		Quaternion correctedDest;
		
		if(shortestPath && cos < 0) {
			cos *= -1;
			correctedDest = r * -1;
		} else
			correctedDest = r;
			
		if(fabs(cos) > (1 - EPSILON))
			return nLerp(correctedDest, lerpFactor, false);
		
		float sin = (float)sqrtf(1.0f - cos * cos);
		float angle = atan2(sin, cos);
		float invSin = 1.0f/sin;
		
		float srcFactor = sinf((1.0f - lerpFactor) * angle) * invSin;
		float destFactor = sinf((lerpFactor) * angle) * invSin;
		
		return Quaternion((*this) * srcFactor + correctedDest * destFactor);
	}
	
	inline Matrix4f toRotationMatrix() const {
		Vector3f forward = Vector3f(2.0f * (getX() * getZ() - getW() * getY()), 2.0f * (getY() * getZ() + getW() * getX()), 1.0f - 2.0f * (getX() * getX() + getY() * getY()));
		Vector3f up = Vector3f(2.0f * (getX()*getY() + getW()*getZ()), 1.0f - 2.0f * (getX()*getX() + getZ()*getZ()), 2.0f * (getY()*getZ() - getW()*getX()));
		Vector3f right = Vector3f(1.0f - 2.0f * (getY()*getY() + getZ()*getZ()), 2.0f * (getX()*getY() - getW()*getZ()), 2.0f * (getX()*getZ() + getW()*getY()));
	
		return Matrix4f().initRotationFromVectors(forward,up,right);
	}
	
	inline Vector3f getForward() const { return Vector3f(0,0,1).rotate(*this); }
	
	inline Vector3f getBack() const { return Vector3f(0, 0, -1).rotate(*this); }
	
	inline Vector3f getUp() const { return Vector3f(0,1,0).rotate(*this); }
	
	inline Vector3f getDown() const { return Vector3f(0,-1,0).rotate(*this); }
	
	inline Vector3f getRight() const { return Vector3f(1,0,0).rotate(*this); }
	
	inline Vector3f getLeft() const { return Vector3f(-1,0,0).rotate(*this); }

	inline Quaternion conjugate() const { return Quaternion(-getX(), -getY(), -getZ(), getW()); }

	inline Quaternion operator*(const Quaternion& r) const {
		const float _w = (getW() * r.getW()) - (getX() * r.getX()) - (getY() * r.getY()) - (getZ() * r.getZ());
		const float _x = (getX() * r.getW()) + (getW() * r.getX()) + (getY() * r.getZ()) - (getZ() * r.getY());
		const float _y = (getY() * r.getW()) + (getW() * r.getY()) + (getZ() * r.getX()) - (getX() * r.getZ());
		const float _z = (getZ() * r.getW()) + (getW() * r.getZ()) + (getX() * r.getY()) - (getY() * r.getX());

		return Quaternion(_x, _y, _z, _w);
	}
	
	inline Quaternion operator*(const Vector3<float>& v) const {
		const float _w = -(getX() * v.getX()) - (getY() * v.getY()) - (getZ() * v.getZ());
		const float _x = (getW() * v.getX()) + (getY() * v.getZ()) - (getZ() * v.getY());
		const float _y = (getW() * v.getY()) + (getZ() * v.getX()) - (getX() * v.getZ());
		const float _z = (getW() * v.getZ()) + (getX() * v.getY()) - (getY() * v.getX());

		return Quaternion(_x, _y, _z, _w);
	}
};

#endif // MATH3D_H_INCLUDED