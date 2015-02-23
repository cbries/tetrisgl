/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __CVEC_H__
#define __CVEC_H__

#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>

/**
 * @brief Calculates vectors
 */
template<class T>
class cVec
{
private:
	T _x, _y, _z;
	T _tmpField[3];

public:
	cVec() {
		_x = _y = _z = 0.0f;
		_tmpField[0] = _tmpField[1] = _tmpField[2] = 0.0f;
	}
	cVec( T x, T y, T z ) {
		_x = x;
		_y = y;
		_z = z;
		_tmpField[0] = x;
		_tmpField[1] = y;
		_tmpField[2] = z;
	}
	cVec( const cVec & v ) {
		_x = v._x;
		_y = v._y;
		_z = v._z;
		_tmpField[0] = _x;
		_tmpField[1] = _y;
		_tmpField[2] = _z;
	}

	inline T betrag() {
		return sqrtf( _x*_x + _y*_y + _z*_z );
	}

	inline void setX( T x ) { _x = x; _tmpField[0] = x; }
	inline void setY( T y ) { _y = y; _tmpField[1] = y; }
	inline void setZ( T z ) { _z = z; _tmpField[2] = z; }
	inline void set( T x, T y, T z ) {
		setX(x);
		setY(y);
		setZ(z);
	}

	inline T * xyz() {
		return _tmpField;
	}

	inline T & x() { return _x; }
	inline T & y() { return _y; }
	inline T & z() { return _z; }

	cVec<T> const & operator=( const cVec<T> & obj );
	cVec<T> operator+( const cVec<T> & v );
	cVec<T> operator+( const T v );
	cVec<T> operator-( const cVec<T> & v );
	cVec<T> operator-( const T v );
	cVec<T> operator*( const cVec<T> & v );
	cVec<T> operator*( const T scalar );
	bool operator==( const cVec<T> & v );
	template<class U> friend std::ostream & operator <<( std::ostream & out, cVec<U> & v );
};

template<class U>
std::ostream & operator<<( std::ostream & out, cVec<U> & v )
{
	out << std::setprecision(4);
	out << "vec("	<< std::setw(4) << v.x() << " "
							<< std::setw(4) << v.y() << " "
							<< std::setw(4) << v.z() << ")";
	return out;
}

template<class T> cVec<T> const &
cVec<T>::operator=( const cVec<T> & obj )
{
	_x = obj._x;
	_y = obj._y;
	_z = obj._z;
	_tmpField[0] = obj._x;
	_tmpField[1] = obj._y;
	_tmpField[2] = obj._z;
}

template<class T> cVec<T>
cVec<T>::operator+( const cVec<T> & v )
{
	cVec<T> vTmp( _x + v._x, _y + v._y, _z + v._z );
	return vTmp;
}

template<class T> cVec<T>
cVec<T>::operator+( const T v )
{
	cVec<T> vTmp( _x + v, _y + v, _z + v );
	return vTmp;
}

template<class T> cVec<T>
cVec<T>::operator-( const cVec<T> & v )
{
	cVec<T> vTmp( _x - v._x, _y - v._y, _z - v._z );
	return vTmp;
}

template<class T> cVec<T>
cVec<T>::operator-( const T v )
{
	cVec<T> vTmp( _x - v, _y - v, _z - v );
	return vTmp;
}

template<class T> cVec<T>
cVec<T>::operator*( const cVec<T> & v )
{
	cVec<T> vTmp( _x * v._x, _y * v._y, _z * v._z );
	return vTmp;
}

template<class T> cVec<T>
cVec<T>::operator*( const T scalar )
{
	cVec<T> vTmp( _x * scalar, _y * scalar, _z * scalar );
	return vTmp;
}

template<class T> bool
cVec<T>::operator==( const cVec<T> & v )
{
	if( _x == v._x && _y == v._y && _z == v._z )
		return true;
	return false;
}

/**
 * @brief Holds a vector list of cVec.
 * cVec Objekten.
 */
template<class T>
class cVecList : public std::vector<cVec<T> > {
};

#endif
