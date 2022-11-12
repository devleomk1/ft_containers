/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 00:33:53 by jisokang          #+#    #+#             */
/*   Updated: 2022/11/12 21:00:18 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"
//ISO/IEC 14882:1998(E) © ISO/IEC 23.2.3.3 Template class stack 23 Containers library
 namespace ft {
	template <class T, class Container = ft::vector<T> >
	class stack {
	public:
		typedef typename Container::value_type	value_type;
		typedef typename Container::size_type	size_type;
		typedef			 Container				container_type;
	protected:
		Container c;
	public:
		explicit stack(const Container& c = Container())
		: c(c) {};
		bool      empty() const { return c.empty(); }
		size_type size()  const { return c.size(); }
		value_type&       top() { return c.back(); }
		const value_type& top() const { return c.back(); }
		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_back(); }

	template <class _T, class _Container>
	friend bool operator==(const stack<_T, _Container>& x, const stack<_T, _Container>& y);

	template <class _T, class _Container>
	friend bool operator< (const stack<_T, _Container>& x, const stack<_T, _Container>& y);
};

	template <class T, class Container>
	bool operator==(const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( x.c == y.c );
	};

	template <class T, class Container>
	bool operator< (const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( x.c < y.c );
	};
	template <class T, class Container>
	bool operator!=(const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( !(x == y) );
	};
	template <class T, class Container>
	bool operator> (const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( y < x );
	};
	template <class T, class Container>
	bool operator>=(const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( !(x < y) );
	};
	template <class T, class Container>
	bool operator<=(const stack<T, Container>& x,
					const stack<T, Container>& y){
		return ( !(y < x) );
	};
}

#endif
