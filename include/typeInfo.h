#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <string>


template <typename t>
struct TypeInfo{
	static std::string name(){
		return typeid(t).name();
	}
	typedef t type;

};

template <>
struct TypeInfo < int > {
	static std::string name() { return "int"; }
	typedef int type;
};

template <>
struct TypeInfo < char > {
	static std::string name() { return "char"; }
	typedef char type;
};

template <>
struct TypeInfo < float32 > {
	static std::string name() { return "float"; }
	typedef float32 type;
};

template <>
struct TypeInfo < float64 > {
	static std::string name() { return "double"; }
	typedef float64 type;
};

template <>
struct TypeInfo < std::string > {
	static std::string name() { return "string"; }
	typedef std::string type;
};

template <typename t>
struct TypeInfo < const t > {
	static std::string name() {
		return "const " + TypeInfo<t>::name();
	}
	typedef typename TypeInfo<t>::type type;
};

template <typename t>
struct TypeInfo < t& > {
	static std::string name() {
		return "Ref " + TypeInfo<t>::name();
	}
	typedef typename TypeInfo<t>::type type;
};

template <typename t>
struct TypeInfo < const t& > {
	static std::string name() {
		return "const Ref " + TypeInfo<t>::name();
	}
	typedef typename TypeInfo<t>::type type;
};

template <typename t>
struct TypeInfo < t&& > {
	static std::string name() {
		return "Rvalue " + TypeInfo<t>::name();
	}
	typedef typename TypeInfo<t>::type type;
};

template <typename t>
struct TypeInfo < t* > {
	static std::string name() { 
		return TypeInfo<t>::name() + "Star";
	}
	typedef typename TypeInfo<t>::type type;
};

template <typename t>
struct TypeInfo < const t* > {
	static std::string name() {
		return "const " + TypeInfo<t>::name() + "Star";
	}
	typedef typename TypeInfo<t>::type type;
};

#endif //TYPE_INFO_H