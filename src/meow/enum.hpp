#ifndef MEOW_ENUM_HPP
#define MEOW_ENUM_HPP

#define ENUM_START(name)	\
struct name: implement fmt::Display {	\
	enum Type {		\
		UNDEF,



#define ENUM_MED(name) \
	};	\
name(): type_(UNDEF) {} \
name(Type type): type_(type) {} \
std::string toString() override { \
	switch (type_) {	

#define ENUM_OUTPUT(name, str)	\
	case name: return str;

#define ENUM_DEFAULT_OUTPUT(name)	\
	case name: return #name;

#define ENUM_END(name) \
	default:	\
		panic("unimplemented");	\
		}	\
	}	\
	Type type_;	\
	};

#endif