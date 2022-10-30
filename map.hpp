/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jisokang <jisokang@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 15:25:09 by jisokang          #+#    #+#             */
/*   Updated: 2022/10/30 19:58:02 by jisokang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include <iostream>
# include <memory>
# include <stdexcept>
# include <cstddef> //for ptrdiff_t size_t
//# include <iostream>
# include "utility.hpp" //for ft::pair
# include "algorithm.hpp"
# include "iterator.hpp"
# include "bidirectional_iterator.hpp"

//# include ""
namespace ft
{
	// AVL 트리는 삭제/삽입하는 곳을 기준으로 위로 올라가면서 회전한다!
	// 1. tree를 Map에 어떻게 붙여야 할까?
	// 2. less가 뭐용? : Key의 쌍을 개를 비교하는데 사용함, 과제에서 요구하지 않기 때문에 직접 만들 필요 없음.
	template <class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
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
				int						height;
				node*					parent;
				node*					left;
				node*					right;
			};

		public:
			typedef Key																key_type;
			typedef T																mapped_type;
			typedef pair<const Key, T>												value_type;
			typedef Compare															key_compare;
			typedef Alloc															allocator_type;
			typedef typename Alloc::template rebind<node>::other 					alloc_node;
			typedef typename Alloc::reference										reference;
			typedef typename Alloc::const_reference									const_reference;
			//typedef value_type&										reference;
			//typedef const value_type&									const_reference;
			typedef ft::bidirectional_iterator<value_type, Compare, node>			iterator;
			typedef ft::bidirectional_iterator<const value_type, Compare, node>		const_iterator;
			typedef size_t															size_type;
			typedef ptrdiff_t														difference_type;
			typedef typename Alloc::pointer											pointer;
			typedef typename Alloc::const_pointer									const_pointer;
			typedef ft::reverse_iterator<iterator>									reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>							const_reverse_iterator;


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
				struct node* node = _alloc_node.allocate(1);
				_alloc_pair.construct(&node->value, value);
				node->height = 1;
				node->parent = NULL;
				node->left = NULL;
				node->right = NULL;
				return ( node );
			}

			//void insertNode(node* node, const value_type& value)
            //{
            //    _root = insert_node(node, value);
            //}

			node* insert_node(node* node, const value_type& value)
			{
				if (node == _last_node)
				{
					//std::cout <<"Node == _last_node\n";
					_new_node = create_node(value);
					_new_node->right = _last_node;
					_last_node->parent = _new_node;
					//std::cout <<"Node == _last_node done\n";
					return ( _new_node );
				}
				if (node == NULL)
				{
					//std::cout <<"Node == NULL\n";
					_new_node = create_node(value);
					return _new_node;
				}
				if (node->value.first > value.first)
				{
					//std::cout <<"LEFT\n";
					node->left = insert_node(node->left, value);
					node->left->parent = node;
				}
				else if (node->value.first < value.first)
				{
					node->right = insert_node(node->right, value);
					node->right->parent = node;
				}
				node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
				return ( balance_tree(node) );
			};

			node* delete_node(node* node, const key_type& key)
			{
				if (!node)
					return NULL;
				if (node->value.first > key) // 왼쪽 자식으로 이동
				{
					node->left = delete_node(node->left, key);
					if (node->left)
						node->left->parent = node;
				}
				else if (node->value.first < key) // 오른쪽 자식으로 이동
				{
					node->right = delete_node(node->right, key);
					if (node->right)
						node->right->parent = node;
				}
                else  // 삭제할 노드 발견
                {
                    if (node->left && (!node->right || node->right == _last_node))
					{
						struct node* _target = node;
						node = _target->left;
						node->parent = _target->parent;
						if (_target->right == _last_node)
						{
							node->right = _last_node;
							_last_node->parent = node;
						}
						dealloc_node(_target);
					}
                    else if (!node->left && node->right)
					{
						struct node* _target = node;
						node = _target->right;
						node->parent = _target->parent;
						dealloc_node(_target);
					}
					else if (node->left && node->right)
					{
						struct node* _target = node;    //  1  1
						node = find_node_min(_target->right); //중위 후속자를 찾아서 n이 참조하게 함
						node->parent= _target->parent;
						node->right = del_min(_target->right);
						if (node->right)
							node->right->parent = node;
						node->left = _target->left;
						node->left->parent = node;
						dealloc_node(_target);
					}
					else
					{
						dealloc_node(node);
						return nullptr;
					}
                }
                node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
                return balance_tree(node);
            };

			node* del_min(node* node)
            {
                if (!node->left)
                    return node->right;
                node->left = del_min(node->left);
                node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
                return balance_tree(node);
            };
/*===============================================================  */
			//node* delete_node(node* node, const key_type& key)
			//{
			//	if (node == NULL)
			//		return (node);
			//	if (node->value.first > key)
			//	{
			//		node->left = delete_node(node->left, key);
			//		if (node->left)
			//			node->left->parent = node;
			//	}
			//	else if (node->value.first < key)
			//	{
			//		node->right = delete_node(node->right, key);
			//		if (node->right)
			//			node->right->parent = node;
			//	}
			//	else //삭제할 노드 발견하면 (node->key == key)
			//	{
			//	//=============================================//
			//		//자식이 1개 or 0개
			//		if( (node->left == NULL) || (node->right == NULL) )
			//		{
			//			struct node* tmp = node->left ? node->right : node->left;

			//			//자식 노드가 없을 경우
			//			if (tmp == NULL)
			//			{
			//				tmp = node;
			//				node = NULL;
			//			}
			//			//자식 노드가 1개만 있는 경우
			//			else
			//			{
			//				tmp->parent = node;
			//				*node = *tmp;
			//				//parent need
			//			}
			//			dealloc_node(tmp);
			//		}
			//		else
			//		{
			//			//!!!!HOW TO CHANGE PARENT?
			//			// 부모를 어떻게 넣어줘야 할지 모르겠다.
			//			// 1. 서브트리가 길다면?

			//			// node with two children: Get the inorder
			//			// successor (smallest in the right subtree)
			//			//struct node* tmp = find_node_min(node->right);

			//			// Copy the inorder successor's
			//			// data to this node
			//			//node->value.first = tmp->value.first;
			//			//node->key = tmp->key;

			//			// Delete the inorder successor
			//			//node->right = delete_node(node->right, tmp->key);
			//			//node->right = delete_node(node->right, tmp->value.first);
			//		}
			//	}
			//	//=============================================//
			//	node->height = get_max_height(node) + 1;
			//	return ( balance_tree(node) );
			//};
/*===============================================================  */


			int get_height(node* node)
			{
				if (node == NULL)
					return ( 0 );
				return ( node->height );
			};

			int get_max_height(node* node)
			{
				return( std::max(get_height(node->left), get_height(node->right)) );
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
				node->parent = tmp;
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
				tmp->left = node;
				node->parent = tmp;
				//node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
				//tmp->height = std::max(get_height(tmp->left), get_height(tmp->right)) + 1;
				update_height(node, tmp);
				return (tmp);
			};

			//node* find_node(node* node, const key_type& key) const
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

			node* find_node(node* node, const key_type& key) const
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

			node* balance_tree(node* node)
			{
				if (get_balance_factor(node) > 1 )			//LL case
				{
					if (get_balance_factor(node->left) < 0)	//LR case
						node->left = rotate_left(node->left);
					node = rotate_right(node);
				}
				else if (get_balance_factor(node) < -1)		//RR case
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

			//delete_min()
			//{

			//};	//이거 이름 이상하다 -> 이거 필요함???

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
			 * @brief 이거 왜 없음???????? -> 아 있네...
			 * ==================================================================================================
			 */
			class value_compare
			{
				friend class map;
				public:
					typedef bool 		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;
					bool operator()(const value_type& lhs, const value_type& rhs) const { return _comp(lhs.first, rhs.first); }
				protected:
					key_compare _comp;
					value_compare(key_compare comp) : _comp(comp) {}
			};
			//class value_compare : public std::binary_function<value_type, value_type, bool> {
			//	friend class map; //부모/자식 관계가 아닌 외부 클래스를 private까지 접근 가능하도록 할 수 있음 개꿀
			//	protected:
			//		Compare comp;
			//		value_compare(Compare c) : comp(c) {}
			//	public:
			//		bool operator()(const value_type& x, const value_type& y) const {
			//			return comp(x.first, y.first);
			//		}
			//};
			/* ==================================================================================================*/

			explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _size(0), _alloc_pair(alloc), _comp(comp)
			{
				_last_node = create_node(value_type());
				_last_node->height = 0;
				_root = _last_node;
			};

			template <class InputIterator>
			map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _size(0), _alloc_pair(alloc), _comp(comp)
			{
				_last_node = create_node(value_type());
				_last_node->height = 0;
				_root = _last_node;
				while (first != last)
				{
					insert(*first);
					first++;
				}
			};
			//map(const map<Key,T,Compare,Allocator>& x); --> 이거 수문이 지우고 아래로 변환함?
			map(const map& x)
			: _size(0), _alloc_pair(x._alloc_pair), _comp(x._comp), _alloc_node(x._alloc_node)
			{
				_last_node = create_node(value_type());
				_last_node->height = 0;
				_root = _last_node;
				const_iterator i = x.begin();
				while (i != x.end())
				{
					insert(*i);
					i++;
				}
				//for (const_iterator i = x.begin(); i != x.end(); ++i)
				//	insert(*i);

			};

			~map() {};	//없어도됨?

			map<Key,T,Compare,Alloc>&
				operator=(const map<Key,T,Compare,Alloc>& x)
				{
					if (&x == this)
						return (*this);
					clear();
					insert(x.begin(), x.end());
					return (*this);
				};

			iterator					begin(){
				node* node = _root;
				while (node && node->left)
				{
					node = node->left;
				}
				return ( iterator(node, _last_node, _comp) );
			};
			const_iterator				begin() const{
				node* node = _root;
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
				node* node = find_node(_root, x);
				if (node)
					return (node->value.second);
				value_type value = ft::make_pair<key_type, mapped_type>(x, mapped_type());
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
			pair<iterator, bool>	insert(const value_type& x)
			{
				node* node = find_node(_root, x.first);
				if (node)
					return (ft::pair<iterator, bool>(iterator(node, _last_node, _comp), false));
				_root = insert_node(_root, x);

				//insertNode(_root, x);
				_size++;
				std::cout << "insert A done\n";
				return (ft::pair<iterator, bool>(iterator(node, _last_node, _comp), true));

			};
			iterator				insert(iterator position, const value_type& x)
			{
				(void)position;
				node* node = find_node(_root, x.first);
				if (node)
					return ( iterator(node, _last_node, _comp) );
				_root = insert_node(_root, x);
				//insertNode(_root, x);
				_size++;
				return ( iterator(node, _last_node, _comp) );
			};
			template <class InputIterator>
				void insert(InputIterator first, InputIterator last)
				{
					std::cout <<"inset C\n";
					while (first != last)
					{
						std::cout<< "-size: " << size()<<"\n";
						insert(*first);
						first++;
					}
				};

			void				erase(iterator position)
			{
				_root = delete_node(_root, position.getNode()->value.first);
				_size--;
			};

			size_type			erase(const key_type& x)
			{
				node* node = find_node(_root, x);
				if (node == NULL)
					return (0);
				_root = delete_node(_root, x);
				_size--;
				return (1);

			};
			void				erase(iterator first, iterator last)
			{
				while (first != last)
				{
					iterator tmp(first);
					first++;
					erase(tmp);
				}
			};
			void				swap(map<Key,T,Compare,Alloc>& x)
			{
				std::swap(this->_root, x._root);
				std::swap(this->_alloc_node, x._alloc_node);
				std::swap(this->_alloc_pair, x._alloc_pair);
				std::swap(this->_last_node, x._last_node);
				std::swap(this->_new_node, x._new_node);
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
				return (_comp);
			};
			value_compare		value_comp() const
			{
				return (value_compare(_comp));
			};

		/* Map operations: */
			//typedef ft::bidirectional_iterator<ft::pair<const Key, T>, Compare, ft::map<Key, T, Compare, Alloc>::node> ft::map<Key, T, Compare, Alloc>::iterator

			iterator			find(const key_type& x)
			{
				node* tmp = find_node(_root, x);
				if (tmp)
					return iterator(tmp, _last_node, _comp);
				return end();
			};
			const_iterator		find(const key_type& x) const
			{
				node* tmp = find_node(_root, x);
				if (tmp)
					return const_iterator(tmp, _last_node, _comp);
				return end();
			};
			size_type			count(const key_type& x) const
			{
				//if (find(x) != this->end())
				//	return (1);
				//return (0);
				node* tmp = find_node(_root, x);
				if (tmp)
					return (1);
				return (0);
			};

			//bound는 뭐하는 함수일까? ->
			iterator			lower_bound(const key_type& x)			//이상 -> 일단 이거는 한번 찾아보자
			{
				iterator it = begin();
				for (; it != end(); ++it)
					if (!_comp(it->first, x))
						break;
				return it;
			};
			const_iterator		lower_bound(const key_type& x) const
			{
				const_iterator it = begin();
				for (; it != end(); ++it)
					if (!_comp(it->first, x))
						break;
				return it;
			};
			iterator			upper_bound(const key_type& x)
			{
				iterator it = begin();
				for (; it != end(); ++it)
					if (_comp(x, it->first))
						break;
				return it;
			};			//초과
			const_iterator		upper_bound(const key_type& x) const
			{
				const_iterator it = begin();
				for (; it != end(); ++it)
					if (_comp(x, it->first))
						break;
				return it;
			};

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

	template <class Key, class T, class Compare, class Alloc>
		bool operator==(const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); };

	template <class Key, class T, class Compare, class Alloc>
		bool operator< (const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); };
	template <class Key, class T, class Compare, class Alloc>
		bool operator!=(const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return !(x == y); };
	template <class Key, class T, class Compare, class Alloc>
		bool operator> (const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return y < x; };
	template <class Key, class T, class Compare, class Alloc>
		bool operator>=(const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return !(x < y); };
	template <class Key, class T, class Compare, class Alloc>
		bool operator<=(const map<Key,T,Compare,Alloc>& x,
						const map<Key,T,Compare,Alloc>& y)
						{ return !(y < x); };

	/* Specialized algorithms: */
	template <class Key, class T, class Compare, class Alloc>
		void swap(map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y){
			x.swap(y);
		};

}

#endif
