#ifndef MEOW_ENUM_HPP
#define MEOW_ENUM_HPP

#define ENUM_START(name)	\
struct name: implement fmt::Display {	\
	enum Type {		\
		__UNDEF,



#define ENUM_MED(name) \
	};	\
name(): type_(__UNDEF) {} \
name(Type type): type_(type) {} \
Type type_;	\
	bool operator==(const name& lhs) const {	\
		return type_ == lhs.type_;	\
	}	\
	bool operator!=(const name& lhs) const {	\
		return type_ != lhs.type_;	\
	}	\
std::string toString() override { \
	switch (type_) {	

#define ENUM_OUTPUT(name, str)	\
	case name: return str;

#define ENUM_DEFAULT_OUTPUT(name)	\
	case name: return #name;



#define ENUM_FUNC(name)		\
	default:	\
		panic("unimplemented");	\
		}	\
		}	

#define ENUM_FUNC_END(name) \
	};

#define ENUM_END(name) \
	ENUM_FUNC(name)	\
	ENUM_FUNC_END(name)
#endif