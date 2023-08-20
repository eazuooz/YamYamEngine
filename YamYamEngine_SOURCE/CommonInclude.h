#pragma once

#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <type_traits>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CAST_UINT(x) (static_cast<UINT>(x))

// Enable enum flags:
// https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html

template<typename E>
struct enable_bitmask_operators 
{
	static constexpr bool enable = false;
};

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator|(E lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>
		( 
			static_cast<underlying>(lhs) | static_cast<underlying>(rhs) 
		);
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type operator|=(E& lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>
		( 
			static_cast<underlying>(lhs) | static_cast<underlying>(rhs) 
		);

	return lhs;
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator&(E lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>
		( 
			static_cast<underlying>(lhs) & static_cast<underlying>(rhs) 
		);
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type operator&=(E& lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>
		(
			static_cast<underlying>(lhs) & static_cast<underlying>(rhs)
		);

	return lhs;
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator~(E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	rhs = static_cast<E>
		(
			~static_cast<underlying>(rhs)
		);
	
	return rhs;
}

template<typename E>
constexpr bool has_flag(E lhs, E rhs)
{
	return (lhs & rhs) == rhs;
}

//#include "bitmask_operators.hpp"
//
//enum class A {
//	x = 1, y = 2
//};
//
//enum class B :unsigned long {
//	x = 0x80000000, y = 0x40000000
//};
//
//template<>
//struct enable_bitmask_operators<A> {
//	static const bool enable = true;
//};
//
//template<>
//struct enable_bitmask_operators<B> {
//	static const bool enable = true;
//};
//
//enum class C { x, y };
//
//
//int main() {
//	A a1 = A::x | A::y;
//	A a2 = a1 & A::y;
//	a2 ^= A::x;
//	A a3 = ~a1;
//
//	B b1 = B::x | B::y;
//	B b2 = b1 & B::y;
//	b2 ^= B::x;
//	B b3 = ~b1;
//
//	// C c1=C::x | C::y;
//	// C c2=c1&C::y;
//	// c2^=C::x;
//	// C c3=~c1;
//}