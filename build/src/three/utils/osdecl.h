#ifndef THREE_OSDECL_H
#define THREE_OSDECL_H
#ifdef _WIN32
#	ifdef COMPILE_THREE_DLL
#		define DLX __declspec(dllexport)
#	else
#		ifdef USE_THREE_DLL	
#			define DLX __declspec(dllimport)
#		else
#			define DLX
#		endif
#	endif
#else
#	define DLX
#endif

#include <cstddef>
#include <type_traits>
#include <typeinfo>


/*
template<typename Type, typename DataType>
inline bool is(DataType& data) {
	if (&data == NULL) return false;
	return typeid(data) == typeid(Type);
}

template<typename Base,typename T>
inline bool instanceof(const T) {
	return std::is_base_of<Base, T>::value;
}

template <typename T>
inline void unused(T) {}
*/
template<typename Base,typename T>
inline bool instanceOf(T* ptr) {
	return dynamic_cast<Base*>(ptr) != nullptr;
}
/*
template<typename Base,typename T>
inline bool isInstanceOf(const T& ptr) {
	try {
		unused(dynamic_cast<const Base&>(ptr));
		return true;
	}
	catch (std::bad_cast e) {
		return false;
	}
}
*/
#endif