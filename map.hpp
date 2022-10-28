/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 15:25:09 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/27 20:42:26 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include <memory>
# include <stdexcept>
# include <cstddef>
//# include <iostream>
# include "utility.hpp" //for ft::pair
# include "algorithm.hpp"

//# include ""
namespace ft
{
	// AVL 트리는 삭제/삽입하는 곳을 기준으로 위로 올라가면서 회전한다!
	// 1. tree를 Map에 어떻게 붙여야 할까?
	// 2. less가 뭐용? : Key의 쌍을 개를 비교하는데 사용함, 과제에서 요구하지 않기 때문에 직접 만들 필요 없음.
	template <class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T>>>
	class map
	{
		public:
			typedef Key																key_type;
			typedef T																mapped_type;
			typedef pair<const Key, T>												value_type;
			typedef Compare															key_compare;
			typedef Alloc															allocator_type;
			typedef typename Alloc::template rebind<node>::other 					alloc_node;
			typedef typename Alloc::reference										reference;
			typedef typename Alloc::const_reference									const_reference;
			// 여기 수정해야함
			typedef ft::bidirectional_iterator<value_type, Compare, node>			iterator;
			typedef ft::bidirectional_iterator<const value_type, Compare, node>		const_iterator;
			typedef size_t															size_type;
			typedef ptrdiff_t														difference_type;
			typedef typename Alloc::pointer											pointer;
			typedef typename Alloc::const_pointer									const_pointer;
			typedef ft::reverse_iterator<iterator>									reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>							const_reverse_iterartor;

		private:
			/**
			 * @brief
			 * 			  parent
			 * 			   /
			 * 			 node
			 * 			/    \
			 * 		  left  right
			 */
			struct node
			{
				ft::pair<const Key, T>	value;
				size_t					height;
				node*					parent;
				node*					left;
				node*					right;
			};
		private:
			node*			_root;
			node*			_new_node;
			node*			_last_node;
			size_type		_size;
			allocator_type	_alloc_pair;
			key_compare		_comp;
			alloc_node		_alloc_node;	//rebind

		/**
		 * ==================================================================
		 * @brief
		 * AVL Tree
		 *
		 * ================================================================== */

			node* create_node(const value_type& value)
			{
				struct node* node = _alloc.allocate(1);
				_alloc_pair.construct(&node->value, value);
				node->height = 1;
				node->parent = NULL;
				node->left = NULL;
				node->right = NULL;
				return ( node );
			}

			node* insert_node(node* node, const value_type& value)
			{
				if (node == _last_node)
				{
					_new_node = create_node(value);
					_new_node->right = _last_node;
					_last_node->parent = _new_node;
					return ( _new_node );
				}
				if (node == NULL)
				{
					_new_node = create_node(value);
					return _new_node;
				}
				if (node->value.first > value.first)
				{
					node->left = insert_node(node->left, value);
					node->left->parent = node;
				}
				else if (node->value.first < value.first)
				{
					node->right = insert_node(node->right, value);
					node->right->parent = node;
				}
				node->height = get_max_height(node) + 1;
				return ( balance_tree(node) );
			};

			node* delete_node(node* node, const key_type& key)
			{
				if (node == NULL)
					return (node);
				if (node->value.first > key)
				{
					node->left = delete_node(node->left, key);
					if (node->left)
						node->left->parent = node;
				}
				else if (node->value.first < key)
				{
					node->right = delete_node(node->right, key);
					if (node->right)
						node->right->parent = node;
				}
				else //삭제할 노드 발견하면 (node->key == key)
				{
				//=============================================//
					//자식이 1개 or 0개
					if( (node->left == NULL) || (node->right == NULL) )
					{
						struct node* tmp = node->left ? node->right : node->left;

						//자식 노드가 없을 경우
						if (tmp == NULL)
						{
							tmp = node;
							node = NULL;
						}
						//자식 노드가 1개만 있는 경우
						else
						{
							tmp->parent = node;
							*node = *tmp;
							//parent need
						}
						dealloc_node(tmp);
					}
					else
					{
						//!!!!HOW TO CHANGE PARENT?
						// 부모를 어떻게 넣어줘야 할지 모르겠다.
						// 1. 서브트리가 길다면?

						// node with two children: Get the inorder
						// successor (smallest in the right subtree)
						struct Node* tmp = find_node_min(node->right);

						// Copy the inorder successor's
						// data to this node
						node->key = tmp->key;

						// Delete the inorder successor
						node->right = delete_node(node->right, tmp->key);
					}
				}
				//=============================================//
				node->height = get_max_height(node) + 1;
				return ( balance_tree(node) );
			};


			size_t get_height(node* node)
			{
				if (node == NULL)
					return ( 0 );
				return ( node->height );
			};

			size_t get_max_height(node* node)
			{
				return( std::max(get_height(node->left), get_height(node->right)); );
			};

			int get_balance_factor(node* node)
			{
				if (node == NULL)
					return ( 0 );
				return ( get_height(node->left) - get_height(node->right) );
			};

			void update_height(node* nod, node* tmp)
			{
				nod->height = get_max_height(nod) + 1;
				tmp->height = get_max_height(tmp) + 1;
			};

			/**
			 * @brief
			 * 				node
			 * 				/
			 * 			  tmp			=>			tmp
			 * 			 /   \					   /   \
			 * 	tmp->left	(tmp->right)	tmp->left	node
			 * 										   /
			 * 									 (tmp->right)
			 *
			 * @param node
			 * @return node*
			 */
			node* rotate_right(node* node)
			{
				struct node* tmp = node->left;
				tmp->parent = node->parent;
				node->left = tmp->right;
				if (tmp->right) //TMP의 오른쪽 노드가 있으면
				{
					tmp->right->parent = node;
				}
				tmp->right = node;
				nodes->parent = tmp;
				update_height(node, tmp);
				return (tmp);
			};

			/**
			 * @brief
			 * 		  node
			 * 			  \
			 * 			  tmp			=>			tmp
			 * 			 /   \					   /   \
			 * 	(tmp->left)	tmp->right			node	tmp->right
			 * 										\
			 * 									 (tmp->left)
			 *
			 * @param node
			 * @return node*
			 */
			node* rotate_left(node* node)
			{
				struct node* tmp = node->right;
				tmp->parent = node->parent;
				node->right = tmp->left;
				if (tmp->left)
				{
					tmp->left->parent = node;
				}
				node->parent = tmp;
				update_height(node, tmp);
				return (tmp);
			};

			node* find_node(node* node, const key_type& key)
			{
				if (node == NULL)
					return ( NULL );
				else if (node->value.first == key)
					return ( NULL );
				else if (node->value.first > key && node->left )
					return ( find_node(node->left, key) );
				else if (node->value.first < key && node->right )
					return ( find_node(node->right, key) );
				return ( NULL );
			};

			void balance_tree(node* node)
			{
				if (get_balance_factor(node) > 1 )			//LL case
				{
					if (get_balance_factor(node->left) < 0)	//LR case
						node->left = rotate_left(node->left);
					node = rotate_right(node);
				}
				else if (get_balance_factor(node < -1))		//RR case
				{
					if (get_balance_factor(node->right) > 0) //RL case
						node->right = rotate_right(node->right);
					node = rotate_left(node);
				}
				else /* -1, 0, 1 */
				{}
				return node;
			};

			node* find_node_min(node* node)
			{
				if (node->left == NULL)
					return ( node );
				return ( find_node_min(node->left) );
			};

			node* find_node_max(node* node)
			{
				if (node->right == NULL)
					return ( node );
				return ( find_node_min(node->right) );
			};

			delete_min()
			{

			};	//이거 이름 이상하다 -> 이거 필요함???

			void dealloc_node(node* node)
			{
				_alloc_pair.destroy(&node->value);
				_alloc_node.deallocate(node, 1);
			};

			//void print_int(node* node)
			//{
			//	std::cout << "[" << << "]";
			//};
			//print_string();
		public:

			/**
			 * @brief 이거 왜 없음????????
			 * ==================================================================================================
			 */
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
			/* ==================================================================================================*/

			explicit map(const key_compare& comp = key_compare(),
						 const allocator_type& alloc = allocator_type())
						 : _tree(comp, a)
						 {};

			template <class InputIterator>
				map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
				: _tree(comp, a)
				{
					_tree.insert_unique(first, last);	///이건 왜 하는거지? -> 그냥 insert라면 안되나?
				};
			//map(const map<Key,T,Compare,Allocator>& x); --> 이거 수문이 지우고 아래로 변환함?
			map(const map& x)
			: _tree(x._tree)
			{};

			~map() {};

			map<Key,T,Compare,Allocator>&
				operator=(const map<Key,T,Compare,Allocator>& x)
				{
					_tree = x._tree;
					return (*this);
				};

			iterator					begin(){
				struct node* node = _root;
				while (node && node->left)
				{
					node = node->left;
				}
				return ( iterator(node, _last_node, _comp) );
			};
			const_iterator				begin() const{
				struct node* node = _root;
				while (node && node->left)
				{
					node = node->left;
				}
				return ( const_iterator(node, _last_node, _comp) );
			};
			iterator					end(){
				return ( iterator(_last_node, _last_node, _comp) );
			};
			const_iterator				end() const{
				return ( const_iterator(_last_node, _last_node, _comp) );
			};
			reverse_iterator			rbegin(){
				return ( reverse_iterator(end()) );
			};
			const_reverse_iterator		rbegin() const{
				return ( const_reverse_iterator(end()) );
			};
			reverse_iterator			rend(){
				return ( reverse_iterator(begin()) );
			};
			const_reverse_iterator		rend() const{
				return ( const_reverse_iterator(begin()) );
			};

			/**
			 * @brief Capacity
			 *
			 */
			bool			empty() const{
				return ( _size == 0 );
			};
			size_type		size() const{
				return ( _size );
			};
			size_type		max_size() const{
				return ( _alloc_node.max_size() );
			};

			/**
			 * @brief element access
			 *
			 * @param x
			 * @return T&
			 */
			T& operator[](const key_type& x)
			{
				struct node* node = find_node(_root, key);
				if (node)
					return (node->value.second);
				value_type value = ft::make_pair<key_type, mapped_type>(key, mapped_type());
				_root = insert_node(_root, value);
				_size++;
				return (_new_node->value.second);
			};

			/* NO at() in C++98 */

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
			void				swap(map<Key,T,Compare,Allocator>& x)
			{
				std::swap(this->_root, x._root);
				std::swap(this->_size, x._size);
				std::swap(this->_comp, x._comp);
			};
			void	clear()
			{
				erase(begin(), end());
			};

		/* Observers */
			key_compare			key_comp() const
			{
				return (_tree._key_compare);
			};
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

			ft::pair<iterator, iterator>
				equal_range(const key_type& x)
				{
					return ( ft::pair<iterator, iterator>(lower_bound(x), upper_bound(x)) );
				};
			ft::pair<const_iterator, const_iterator>
				equal_range(const key_type& x) const
				{
					return ( ft::pair<const_iterator, const_iterator>(lower_bound(x), upper_bound(x)) );
				};
	};

	/**
	 * @brief Non-member function overloads
	 * - relational operators
	 * 		operation	|	equivalent operation
	 *		a!=b		|	!(a==b)
	 *		a>b			|	b<a
	 *		a<=b		|	!(b<a)
	 *		a>=b		|	!(a<b)
	 * - swap
	 */

	template <class Key, class T, class Compare, class Allocator>
		bool operator==(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); };

	template <class Key, class T, class Compare, class Allocator>
		bool operator< (const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); };
	template <class Key, class T, class Compare, class Allocator>
		bool operator!=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return !(x == y); };
	template <class Key, class T, class Compare, class Allocator>
		bool operator> (const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return y < x; };
	template <class Key, class T, class Compare, class Allocator>
		bool operator>=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return !(x < y); };
	template <class Key, class T, class Compare, class Allocator>
		bool operator<=(const map<Key,T,Compare,Allocator>& x,
						const map<Key,T,Compare,Allocator>& y)
						{ return !(y < x); };

	/* Specialized algorithms: */
	template <class Key, class T, class Compare, class Allocator>
		void swap(map<Key,T,Compare,Allocator>& x, map<Key,T,Compare,Allocator>& y){
			x.swap(y);
		};

}

#endif
