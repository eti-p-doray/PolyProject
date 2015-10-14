#ifndef MATRIX_H
#define MATRIX_H

#include <QGenericMatrix>

template<typename T> class Quat;
template<typename T, int n, int m> class Matrix;
template<typename T, int n, int m> class TransposedMatrix;

template<typename T>
class Quat {
public:
    Quat() : data_{} {}
    Quat(const T (b)[4]) {*this = b;}
    Quat<T> & operator=(const T (b)[4]);
    
    T & operator() (const int i) {return data_[i];}
    const T & operator() (const int i) const {return data_[i];}
    
    Matrix<T, 4, 4> getMulPreMatrix();
    Matrix<T, 4, 4> getMulPostMatrix();
    Matrix<T, 3, 3> getMatrixConversionJac(int i);
    
    Quat<T> operator+(Quat<T> b);
    Quat<T> operator-(Quat<T> b);
    Quat<T> operator*(Quat<T> b);
    Quat<T> operator*(T b);
    T norm();
    
    Matrix<T, 3, 1> toVectX();
    Matrix<T, 3, 1> toVectY();
    Matrix<T, 3, 1> toVectZ();
    
private:
    T data_[4];
};

template<typename T, int n, int m>
class Matrix {
public:
    Matrix() : data_{} {}
    Matrix(const T (b)[n][m]) {*this = b;}
    explicit Matrix(const Quat<T> & quat);
    Matrix(const QGenericMatrix<n,m,float>& b);
    Matrix<T, n, m>& operator=(const T (b)[n][m]);
    Matrix<T, n, m>& operator=(const QGenericMatrix<n,m,float>& b);
    
    T & operator() (const int i, const int j) {return data_[i][j];}
    Matrix<T, 1, m> & operator() (const int i) {return *(Matrix<T, 1, m>*)data_[i];}
    
    Matrix<T, n, m>& operator+=(Matrix<T, n, m> b) {*this = *this + b;  return *this;}
    Matrix<T, n, m> operator+(Matrix<T, n, m> b);
    Matrix<T, n, m> operator-(Matrix<T, n, m> b);
    template<int p> Matrix<T,n,p> operator*(Matrix<T,m,p> & b);
    template<int p> Matrix<T,n,p> operator*(TransposedMatrix<T,m,p> & b);
    Matrix<T, n, m> operator*(T b);
    
    TransposedMatrix<T, m, n>& t() {return *(TransposedMatrix<T, m, n>*)this;}
    
private:
    T data_[n][m];
};

template<typename T, int n, int m>
class TransposedMatrix {
public:
    T & operator() (const int i, const int j) {return data[j][i];}
    
    template<int p> Matrix<T,n,p> operator*(Matrix<T,m,p> & b);
    
    Matrix<T, n, m> t() {return (Matrix<T, n, m>*)this;}
    
private:
    T data[m][n];
};

template <typename T>
T getAngle(Matrix<T, 3, 1> a, Matrix<T, 3, 1> b) {
    return acos(dot(a, b) / (norm(a) * norm(b)));
}

/*template <typename T>
T getAngle(Matrix<T, 3, 3> a, Matrix<T, 3, 3> b, Matrix<T, 3, 1> normal)
{
    float dotProduct[3];

    for (int i = 0; i < 3; i++) {
        dotProduct[i] = 0;
        for (int j = 0; j < 3; j++) {
            dotProduct[i] += a(j,i)*normal(j,0) + b(j,i)*normal(j,0);
        }
    }

    //calculer les deux vecteur normal au plan
    Matrix<T,3,1> planeNormal[2];
    if (dotProduct[0] < dotProduct[1] && dotProduct[0]  < dotProduct[2]) {
        planeNormal[0] = cross(normal, a) normal.cross(a.xBasis);
        planeNormal[1] = normal.cross(b.xBasis);
    }
    else if (dotProduct[1] < dotProduct[0] && dotProduct[1] < dotProduct[2]) {
        planeNormal[0] = normal.cross(a.yBasis);
        planeNormal[1] = normal.cross(b.yBasis);
    }
    else {
        planeNormal[0] = normal.cross(a.zBasis);
        planeNormal[1] = normal.cross(b.zBasis);
    }

    //calculer l'angle entre les deux vecteur
    float angle = getAngle(planeNormal[0], planeNormal[1]);

    //caclculer le signe de l'angle
    Matrix<T,3,1> normalTest = cross(planeNormal[0], planeNormal[1]);

    if (dot(normal, normalTest) >= 0) {
        return angle;
    }
    else {
        return -angle;
    }
}*/

template <typename T>
T dot(Matrix<T, 3, 1> a, Matrix<T, 3, 1> b) {
    return (a.t() * b)(0,0);
}

template <typename T>
Matrix<T, 3, 1> cross(Matrix<T, 3, 1> a, Matrix<T, 3, 1> b) {
    Matrix<T, 3, 1> ans;
    ans(0,0) = a(1,0) * b(2,0) - a(2,0) * b(1,0);
    ans(1,0) = a(2,0) * b(0,0) - a(0,0) * b(2,0);
    ans(2,0) = a(0,0) * b(1,0) - a(1,0) * b(0,0);
    return ans;
}

template <typename T>
Matrix<T, 3, 1> cross(Matrix<T, 3, 1> a, TransposedMatrix<T, 3, 1> b) {
    Matrix<T, 3, 1> ans;
    ans(0,0) = a(1,0) * b(2,0) - a(2,0) * b(1,0);
    ans(1,0) = a(2,0) * b(0,0) - a(0,0) * b(2,0);
    ans(2,0) = a(0,0) * b(1,0) - a(1,0) * b(0,0);
    return ans;
}

template <typename T>
T norm(Matrix<T, 3, 1> a) {
    return sqrt(( a.t() * a )(0,0));
}


template <typename T>
Quat<T> & Quat<T>::operator=(const T (b)[4]) {
    for (int i=0; i<4; i++) {
        (*this)(i) = b[i];
    }
    return *this;
}

template <typename T>
Matrix<T, 4, 4> Quat<T>::getMulPreMatrix() {
    Matrix<T, 4, 4> mat;
    mat(0,0) = (*this)(0);    mat(0,1) = -(*this)(1);   mat(0,2) = -(*this)(2);   mat(0,3) = -(*this)(3);
    mat(1,0) = (*this)(1);    mat(1,1) =  (*this)(0);   mat(1,2) = -(*this)(3);   mat(1,3) =  (*this)(2);
    mat(2,0) = (*this)(2);    mat(2,1) =  (*this)(3);   mat(2,2) =  (*this)(0);   mat(2,3) = -(*this)(1);
    mat(3,0) = (*this)(3);    mat(3,1) = -(*this)(2);   mat(3,2) =  (*this)(1);   mat(3,3) =  (*this)(0);
    return mat;
}

template <typename T>
Matrix<T, 4, 4> Quat<T>::getMulPostMatrix() {
    Matrix<T, 4, 4> mat;
    mat(0,0) = (*this)(0);    mat(0,1) = -(*this)(1);   mat(0,2) = -(*this)(2);   mat(0,3) = -(*this)(3);
    mat(1,0) = (*this)(1);    mat(1,1) =  (*this)(0);   mat(1,2) =  (*this)(3);   mat(1,3) = -(*this)(2);
    mat(2,0) = (*this)(2);    mat(2,1) = -(*this)(3);   mat(2,2) =  (*this)(0);   mat(2,3) =  (*this)(1);
    mat(3,0) = (*this)(3);    mat(3,1) =  (*this)(2);   mat(3,2) = -(*this)(1);   mat(3,3) =  (*this)(0);
    return mat;
}

template <typename T>
Matrix<T, 3, 3> Quat<T>::getMatrixConversionJac(int i) {
    Matrix<T, 3, 3> mat;
    switch (i) {
        case 0:
            mat(0,0) = (*this)(0);    mat(0,1) = -(*this)(3);   mat(0,2) =  (*this)(2);
            mat(1,0) = (*this)(3);    mat(1,1) =  (*this)(0);   mat(1,2) = -(*this)(1);
            mat(2,0) =-(*this)(2);    mat(2,1) =  (*this)(1);   mat(2,2) =  (*this)(0);
            break;
            
        case 1:
            mat(0,0) = (*this)(1);    mat(0,1) =  (*this)(2);   mat(0,2) =  (*this)(3);
            mat(1,0) = (*this)(2);    mat(1,1) = -(*this)(1);   mat(1,2) = -(*this)(0);
            mat(2,0) = (*this)(3);    mat(2,1) =  (*this)(0);   mat(2,2) = -(*this)(1);
            break;
            
        case 2:
            mat(0,0) =-(*this)(2);    mat(0,1) =  (*this)(1);   mat(0,2) =  (*this)(0);
            mat(1,0) = (*this)(1);    mat(1,1) =  (*this)(2);   mat(1,2) =  (*this)(3);
            mat(2,0) =-(*this)(0);    mat(2,1) =  (*this)(3);   mat(2,2) = -(*this)(2);
            break;
            
        case 3:
            mat(0,0) =-(*this)(3);    mat(0,1) = -(*this)(0);   mat(0,2) =  (*this)(1);
            mat(1,0) = (*this)(0);    mat(1,1) = -(*this)(3);   mat(1,2) =  (*this)(2);
            mat(2,0) = (*this)(1);    mat(2,1) =  (*this)(2);   mat(2,2) =  (*this)(3);
            break;
            
        default:
            break;
    }
    mat  = mat * 2.0;
    return mat;
}

template <typename T>
Quat<T> Quat<T>::operator+(const Quat<T> b) {
    Quat<T> ans;
    for (int i = 0; i < 4; i++) {
        ans(i) = data_[i] + b(i);
    }
    return ans;
}

template <typename T>
Quat<T> Quat<T>::operator-(const Quat<T> b)  {
    Quat<T> ans;
    for (int i = 0; i < 4; i++) {
        ans(i) = data_[i] - b(i);
    }
    return ans;
}

template <typename T>
Quat<T> Quat<T>::operator*(const Quat<T> b) {
    Quat<T> ans;
    ans(0) = (*this)(0)*b(0) - (*this)(1)*b(1) - (*this)(2)*b(2) - (*this)(3)*b(3);
    ans(1) = (*this)(0)*b(1) + (*this)(1)*b(0) + (*this)(2)*b(3) - (*this)(3)*b(2);
    ans(2) = (*this)(0)*b(2) - (*this)(1)*b(3) + (*this)(2)*b(0) + (*this)(3)*b(1);
    ans(3) = (*this)(0)*b(3) + (*this)(1)*b(2) - (*this)(2)*b(1) + (*this)(3)*b(0);
    
    return ans;
}

template <typename T>
Quat<T> Quat<T>::operator*(const T b) {
    Quat<T> ans;
    for (int i = 0; i < 4; i++) {
        ans(i) = (*this)(i) * b;
    }
    return ans;
}

template <typename T>
T Quat<T>::norm() {
    return pow(data_[0], 2) + pow(data_[1], 2) + pow(data_[2], 2) + pow(data_[3], 2);
}

template <typename T>
Matrix<T, 3, 1> Quat<T>::toVectX() {
    Matrix<T, 3, 1> x;
    x(0) = pow((*this)(0),2) + pow((*this)(1),2) - pow((*this)(2),2) - pow((*this)(3),2);
    x(1) = 2*((*this)(1)*(*this)(2)-(*this)(0)*(*this)(3));
    x(2) = 2*((*this)(1)*(*this)(3)+(*this)(0)*(*this)(1));
}

template <typename T>
Matrix<T, 3, 1> Quat<T>::toVectY() {
    Matrix<T, 3, 1> y;
    y(0,0) = 2*((*this)(1)*(*this)(2)+(*this)(0)*(*this)(3));
    y(1,0) = pow((*this)(0),2)-pow((*this)(1),2)+pow((*this)(2),2)-pow((*this)(3),2);
    y(2,0) = 2*((*this)(2)*(*this)(3)-(*this)(0)*(*this)(1));
    return y;
}
template <typename T>
Matrix<T, 3, 1> Quat<T>::toVectZ() {
    Matrix<T, 3, 1> z;
    z[0] = 2*((*this)(1)*(*this)(3)-(*this)(0)*(*this)(2));
    z[1] = 2*((*this)(2)*(*this)(3)+(*this)(0)*(*this)(1));
    z[2] = pow((*this)(0),2)-pow((*this)(1),2)-pow((*this)(2),2)+pow((*this)(3),2);
    return z;
}


template <typename T, int n, int m>
Matrix<T, n, m>::Matrix(const Quat<T> & quat) {
    T q[4][4];
    for (int i=0; i<4; i++) {
        q[i][i] = quat(i) * quat(i);
        for (int j=i+1; j<=4; j++) {
            q[i][j] = quat(i) * quat(j);
        }
    }
    
    (*this)(0,0) = q[0][0]+q[1][1]-q[2][2]-q[3][3];
    (*this)(0,1) = 2*(q[1][2]-q[0][3]);
    (*this)(0,2) = 2*(q[1][3]+q[0][2]);
    
    (*this)(1,0) = 2*(q[1][2]+q[0][3]);
    (*this)(1,1) = q[0][0]-q[1][1]+q[2][2]-q[3][3];
    (*this)(1,2) = 2*(q[2][3]-q[0][1]);
    
    (*this)(2,0) = 2*(q[1][3]-q[0][2]);
    (*this)(2,1) = 2*(q[2][3]+q[0][1]);
    (*this)(2,2) = q[0][0]-q[1][1]-q[2][2]+q[3][3];
}

template <typename T, int n, int m>
Matrix<T, n, m>::Matrix(const QGenericMatrix<n,m,float>& b)
{
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            (*this)(i,j) = b(i,j);
        }
    }
}

template <typename T, int n, int m>
Matrix<T, n, m>& Matrix<T, n, m>::operator=(const QGenericMatrix<n,m,float>& b)
{
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            (*this)(i,j) = b(i,j);
        }
    }
    return *this;
}

template <typename T, int n, int m>
Matrix<T, n, m> & Matrix<T, n, m>::operator=(const T (b)[n][m]) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            (*this)(i,j) = b[i][j];
        }
    }
    return *this;
}

template <typename T, int n, int m>
Matrix<T, n, m> Matrix<T, n, m>::operator+(Matrix<T, n, m> b) {
    Matrix<T, n, m> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            ans(i,j) = (*this)(i,j)+b(i,j);
        }
    }
    return ans;
}

template <typename T, int n, int m>
Matrix<T, n, m> Matrix<T, n, m>::operator-(Matrix<T, n, m> b) {
    Matrix<T, n, m> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            ans(i,j) = (*this)(i,j)-b(i,j);
        }
    }
    return ans;
}

template <typename T, int n, int m> template <int p>
Matrix<T,n,p> Matrix<T, n, m>::operator*(Matrix<T,m,p> & b) {
    Matrix<T, n, p> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<p; j++) {
            ans(i,j) = (*this)(i,0) * b(0,j);
            for (int k=1; k<m; k++) {
                ans(i,j) += (*this)(i,k)*b(k,j);
            }
        }
    }
    return ans;
}

template <typename T, int n, int m> template <int p>
Matrix<T,n,p> Matrix<T, n, m>::operator*(TransposedMatrix<T,m,p> & b) {
    Matrix<T, n, p> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<p; j++) {
            ans(i,j) = (*this)(i,0) * b(0,j);
            for (int k=1; k<m; k++) {
                ans(i,j) += (*this)(i,k)*b(k,j);
            }
        }
    }
    return ans;
}

template <typename T, int n, int m>
Matrix<T, n, m> Matrix<T,n,m>::operator*(T b) {
    Matrix<T, n, m> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            ans(i,j) = (*this)(i,j) * b;
        }
    }
    return ans;
}

template <typename T, int n, int m> template <int p>
Matrix<T,n,p> TransposedMatrix<T, n, m>::operator*(Matrix<T,m,p> & b) {
    Matrix<T, n, p> ans;
    for (int i=0; i<n; i++) {
        for (int j=0; j<p; j++) {
            ans(i,j) = (*this)(i,0) * b(0,j);
            for (int k=1; k<m; k++) {
                ans(i,j) += (*this)(i,k)*b(k,j);
            }
        }
    }
    return ans;
}

#endif
