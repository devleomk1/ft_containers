/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 16:40:59 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/23 14:34:59 by jisokang         ###   ########.fr       */
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

	template<>											//이거 템플릿에 안 넣어주는 이유는?
	struct is_integral<bool> : public true_type {};		//여기 들어가는 type들의 기준은 무엇인가? 왜 이것들만 들어감?

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
