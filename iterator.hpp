/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 20:35:21 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/14 18:52:37 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <cstddef>				//FOR ptrdiff_t (https://en.cppreference.com/w/cpp/types/ptrdiff_t)
# include "iterator_traits.hpp"

namespace ft
{
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

template <class Iterator>
	class reverse_iterator : public
		iterator<typename iterator_traits<Iterator>::iterator_category,
				 typename iterator_traits<Iterator>::value_type,
				 typename iterator_traits<Iterator>::difference_type,
				 typename iterator_traits<Iterator>::pointer,
				 typename iterator_traits<Iterator>::reference> {
	protected:
		Iterator current;
	public:
		typedef Iterator											iterator_type;
		typedef typename iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename iterator_traits<Iterator>::reference		reference;
		typedef typename iterator_traits<Iterator>::pointer			pointer;

		reverse_iterator();
		explicit reverse_iterator(Iterator x);
		template <class U> reverse_iterator(const reverse_iterator<U>& u);

		Iterator base() const	{ return(current); };
		reference	operator*() const{
			Iterator tmp = current;
			return *--tmp;
		};
		pointer		operator->() const{
			return &(operator*());
		};

		reverse_iterator&	operator++(){
			--current;
			return (*this);
		};
		reverse_iterator	operator++(int){
			reverse_iterator tmp = *this;
			--current;
			return (tmp);
		};
		reverse_iterator&	operator--(){
			++current;
			return (*this);
		};
		reverse_iterator	operator--(int){
			reverse_iterator tmp = *this;
			++current;
			return (tmp);
		};

		reverse_iterator	operator+ (difference_type n) const{
			return (reverse_iterator(current - n));
		};
		reverse_iterator&	operator+=(difference_type n){
			current -= n;
			return (*this);
		};
		reverse_iterator	operator- (difference_type n) const{
			return (reverse_iterator(current+n));
		};
		reverse_iterator&	operator-=(difference_type n){
			current += n;
			return (*this);
		};
		reference operator[](difference_type n) const{
			return (current[-n-1]);	//왜에?
		};
	};

	template <class Iterator>
		bool operator==(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current == y.current);
			};
	template <class Iterator>
		bool operator<(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current > y.current);
			};
	template <class Iterator>
		bool operator!=(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current != y.current);
			};
	template <class Iterator>
		bool operator>(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current < y.current);
			};
	template <class Iterator>
		bool operator>=(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current <= y.current);
			};
	template <class Iterator>
		bool operator<=(
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (x.current >= y.current);
			};
	template <class Iterator>
		typename reverse_iterator<Iterator>::difference_type operator-(	//왜 difference_type으로 가져올까?
			const reverse_iterator<Iterator>& x,
			const reverse_iterator<Iterator>& y)
			{
				return (y.current - x.current);
			};
	template <class Iterator>
		typename reverse_iterator<Iterator>::difference_type operator+(	//왜 -n 으로 인자를 받을까 y가 아니고?
			typename reverse_iterator<Iterator>::difference_type n,
			const reverse_iterator<Iterator>& x)
			{
				return (x.current - n);
			};

	////tag는 머시당가?
	//struct input_iterator_tag {};
	//struct output_iterator_tag {};
	//struct forward_iterator_tag: public input_iterator_tag {};
	//struct bidirectional_iterator_tag: public forward_iterator_tag {};
	//struct random_access_iterator_tag: public bidirectional_iterator_tag {};

	//24.3.4 iterator operations:
	template <class InputIterator, class Distance>
		void advance(InputIterator& i, Distance n){

		};
	template <class InputIterator>
		typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last){

		};


}

#endif
