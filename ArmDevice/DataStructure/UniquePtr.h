//
//  sharedPtr.h
//  ArmDevice
//
//  Created by Etienne Pierre-Doray on 2015-02-11.
//
//

#ifndef ArmDevice_sharedPtr_h
#define ArmDevice_sharedPtr_h

template <class T>
class UniquePtr {
public:
    UniquePtr() {ptr_ = nullptr;}
    explicit UniquePtr(T* ptr) {ptr_ = ptr;}
    UniquePtr(UniquePtr<T>&& x) {ptr_ = x.ptr_; x.ptr_ = nullptr;}
    UniquePtr(UniquePtr&) = delete;
    
    ~UniquePtr() {delete ptr_;}
    
    UniquePtr<T>& operator=(T* ptr);
    UniquePtr<T>& operator=(UniquePtr<T>&& x);
    bool operator==(T* ptr) const {return ptr_ == ptr;}
    
    T* operator->() {return ptr_;}
    const T* operator->() const {return ptr_;}
    T* getPtr() {return ptr_;}
    
private:
    T* ptr_;
};

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(T* ptr) 
{
	if (ptr_ != ptr) {
		delete ptr_; 
		ptr_ = ptr; 
	}
	return *this;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& x)
{
	if (this != &x) {
		delete ptr_; 
		ptr_ = x.ptr_; 
		x.ptr_ = nullptr; 
	}
	return *this;
}

#endif
