/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 16:40:59 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/11 21:28:45 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_HPP
# define UTIL_HPP

namespace ft
{
	//enable_if	(https://cplusplus.com/reference/type_traits/enable_if/?kw=enable_if)
	template<bool Cond, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T type; };
}

#endif
