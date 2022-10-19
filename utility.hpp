/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:19:58 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/19 14:26:17 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

namespace ft
{
//ISO+IEC p.358

template <class T1, class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		T1 first;
		T2 second;

		pair();
		pair(const T1& x, const T2& y);
		template<class U, class V> pair(const pair<U, V> &p);
	};

pair();
}

#endif
