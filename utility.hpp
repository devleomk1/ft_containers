/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:19:58 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/03 23:16:01 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

namespace ft
{
//ISO+IEC p.358
/**
 * @brief
 *
 * @tparam T1
 * @tparam T2
 * @link (https://cplusplus.com/reference/utility/pair/?kw=pair)
 */
template <class T1, class T2>
	struct pair {
		public:
			typedef T1 first_type;
			typedef T2 second_type;

			first_type first;
			second_type second;

			pair() : first(), second() {};
			pair(const T1& x, const T2& y) : first(x), second(y) {};
			template<class U, class V>
				pair(const pair<U, V> &p) : first(p.first), second(p.second) {};

	};

	/**
	 * @brief
	 * - relational operators
	 * 		operation	|	equivalent operation
	 *		x!=y		|	!(x==y)
	 *		x>y			|	y<x
	 *		x<=y		|	!(y<x)
	 *		x>=y		|	!(x<y)
	 */
	template <class T1, class T2>
		bool operator== (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( x.first == y.first && x.second == y.second );
		};
	template <class T1, class T2>
		bool operator!= (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( !(x == y) );
		};
	template <class T1, class T2>
		bool operator<  (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( x.first < y.first || (!(y.first < x.first) && x.second < y.second) );
		};
	template <class T1, class T2>
		bool operator<= (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( !(y < x) );
		};
	template <class T1, class T2>
		bool operator>  (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( y < x );
		};
	template <class T1, class T2>
		bool operator>= (const pair<T1,T2>& x, const pair<T1,T2>& y){
			return ( !(x < y) );
		};

	template <class T1, class T2>
		//이거 Head에는 &있는데 왜 안붙일까? -> 복사 생성자가 생성 안되니까 속도 차이가 발생해 개꿀
		pair<T1,T2> make_pair(T1 x, T2 y){
			return ( pair<T1,T2>(x, y) );
		};

}

#endif
