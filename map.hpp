/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 15:25:09 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/20 23:35:02 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include <memory>
# include <stdexcept>
# include "utility.hpp" //for ft::pair
namespace ft
{
	// 1. tree를 Map에 어떻게 붙여야 할까?
	// 2. less가 뭐용? : Key의 쌍을 개를 비교하는데 사용함, 과제에서 요구하지 않기 때문에 직접 만들 필요 없음.
	template <class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T>>>
	class map
	{
		private:
			struct node
			{
				ft::pair<const Key, T>	value;
				size_t					height;
				node*					parent;
				node*					left;
				node*					right;
			};

		public:
			typedef Key										key_type;
			typedef T										mapped_type;
			typedef pair<const Key, T>						value_type;
			typedef Compare									key_compare;
			typedef Allocator								allocator_type;
			typedef typename Allocator::reference			reference;
			typedef typename Allocator::const_reference		const_reference;
			// 여기 수정해야함
			typedef implementation defined					iterator;
			typedef implementation defined					const_iterator;
			typedef implementation defined					size_type;
			typedef implementation defined					difference_type;
			typedef typename Allocator::pointer				pointer;
			typedef typename Allocator::const_pointer		const_pointer;
			typedef std::reverse_iterator<iterator>			reverse_iterator;
			typedef std::reverse_iterator<const_iterator>	const_reverse_iterartor;

			class value_compare : public binary_function<value_type, value_type, bool> {
				friend class map; //부모/자식 관계가 아닌 외부 클래스를 private까지 접근 가능하도록 할 수 있음 개꿀
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {}
				public:
					bool operator()(const value_type& x, const value_type& y) const {
						return comp(x.first, y.first);
					}
			};

			explicit map(const Compare& comp = Compare(),
						 const Allocator& = Allocator());
			template <class InputIterator>
				map(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Allocator& = Allocator()){

				};
			map(const map<Key,T,Compare,Allocator>& x);
			~map();
			map<Key,T,Compare,Allocator>&
				operator=(const map<Key,T,Compare,Allocator>& x);

			iterator					begin();
			const_iterator				begin() const;
			iterator					end();
			const_iterator				end() const;
			reverse_iterator			rbegin();
			const_reverse_iterator		rbegin() const;
			reverse_iterator			rend();
			const_reverse_iterator		rend() const;

			/**
			 * @brief Capacity
			 *
			 */
			bool			empty() const;
			size_type		size() const;
			size_type		max_size() const;

			/**
			 * @brief element access
			 *
			 * @param x
			 * @return T&
			 */
			T& operator[](const key_type& x);

		/* Modifers: */

			/**
			 * @brief
			 *
			 * @param x
			 * @return pair<iterator, bool>
			 */
			pair<iterator, bool>	insert(const value_type& x);
			iterator				insert(iterator position, const value_type& x);
			template <class InputIterator>
				void insert(InputIterator first, InputIterator last);

			void				erase(iterator position);
			size_type			erase(const key_type& x);
			void				erase(iterator first, iterator last);
			void				swap(map<Key,T,Compare,Allocator>&);
			void				clear();

		/* Observers */
			key_compare			key_comp() const;
			value_compare		value_comp() const;

		/* Map operations: */
			iterator			find(const key_type& x);
			const_iterator		find(const key_type& x) const;
			size_type			count(const key_type& x) const;

			//bound는 뭐하는 함수일까? ->
			iterator			lower_bound(const key_type& x);			//이상 -> 일단 이거는 한번 찾아보자
			const_iterator		lower_bound(const key_type& x) const;
			iterator			upper_bound(const key_type& x);			//초과
			const_iterator		upper_bound(const key_type& x) const;

			pair<iterator, iterator>
				equal_range(const key_type& x);
			pair<const_iterator, const_iterator>
				equal_range(const key_type& x) const;
	};

	template <class Key, class T, class Compare, class Allocator>
		bool operator==(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);

	template <class Key, class T, class Compare, class Allocator>
		bool operator< (const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
		bool operator!=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
		bool operator> (const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
		bool operator>=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
		bool operator<=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y);

	/* Specialized algorithms: */
	template <class Key, class T, class Compare, class Allocator>
		void swap(map<Key,T,Compare,Allocator>& x,
					map<Key,T,Compare,Allocator>& y);

}

#endif
