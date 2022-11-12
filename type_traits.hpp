/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 16:40:59 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/12 19:50:00 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft
{
	template <class T, T v>
		struct integral_constant
		{
			static const T					value = v;
			typedef T						value_type;
			typedef integral_constant<T,v>	type;
			operator T() { return v; }
		};

	typedef integral_constant<bool, true>	true_type;
	typedef integral_constant<bool, false>	false_type;

	//enable_if	(https://cplusplus.com/reference/type_traits/enable_if/?kw=enable_if)
	template<bool Cond, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T type; };

	template<class T>
	struct is_integral : public false_type {};

	/**
	 * @brief	fundamental integral types
	 * @link	https://en.cppreference.com/w/cpp/language/types
	 * 			https://cplusplus.com/reference/type_traits/is_integral/
	 *
	 * bool
	 * char
	 * [x] char16_t(Since C++11)
	 * [x] char32_t(Since C++11)
	 * wchar_t : wide charter 표현을 위한 타입
	 * signed char
	 * short int
	 * int
	 * long int
	 * [x] long long int(Since C++11)
	 * unsigned char
	 * unsigned short int
	 * unsigned int
	 * unsigned long int
	 * [x] unsigned long long int(Since C++11)
	 */

	template<>
	struct is_integral<bool> : public true_type {};

	template<>
	struct is_integral<char> : public true_type {};

	template<>
	struct is_integral<wchar_t> : public true_type {};

	template<>
	struct is_integral<signed char> : public true_type {};

	template<>
	struct is_integral<short int> : public true_type {};
	template<>
	struct is_integral<int> : public true_type {};

	template<>
	struct is_integral<long int> : public true_type {};

	template<>
	struct is_integral<unsigned char> : public true_type {};

	template<>
	struct is_integral<unsigned short int> : public true_type {};

	template<>
	struct is_integral<unsigned int> : public true_type {};

	template<>
	struct is_integral<unsigned long int> : public true_type {};

}

#endif
