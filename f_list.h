//
// Created by andreasjhon on 2/12/25.
//

#ifndef F_LIST_H
#define F_LIST_H

#include <memory>
#include <initializer_list>
#include <utility>
#include <algorithm>

/*
    CUSTOM FORWARD_LIST IMPLEMENTATION (SINGLY LINKED LIST) USING SHARED_POINTER TO SUPPORT COPY SEMANTICS
    FEEL FREE TO CONTRIBUTE TO MAKE THE IMPLEMENTATION BETTER
    EXPECTED TO BE USED WITH > C++11 (Orginally tested with C++17 g++ compiler)
    NOTE : CONTAINER WON'T THROW ANY EXCEPTIONS IF YOU TRY TO ACCESS INVALIDATED POINTER (EITHER BY DEFERENCING OR TRYING TO USE OPERATOR-> ON INVALIDATED ITERATORS).
    - IF YOU ENCOUNTER SEGFAULT, YOU MAY BE TRYING TO ACCESS INVALIDATED POINTER / NULL POINTERS.
    NO SUPPORT FOR CONTEXPR FOR NOW
*/ 

namespace andr {
    template<typename T>
    class f_list {
    private:
        //base node structure for singly linked list
        struct Base_node {
            T m_data;
            std::shared_ptr<Base_node> next;

            template<typename... Args>
            explicit
            Base_node(Args&&... args) noexcept
                : m_data(std::forward<Args>(args)...) {
            }
        };

        using Node_ptr = Base_node*;

        std::shared_ptr<Base_node> head;
        size_t size;
        static constexpr Node_ptr END = nullptr;

        template<typename... Args>
        std::shared_ptr<Base_node> _create_node(Args&&... _data) noexcept {
            return std::make_shared<Base_node>(std::forward<T>(_data)...);
        }

        template<typename... Args>
        void
        _create_at_head(Args&&... args) noexcept {
            head = std::make_shared<Base_node>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void
        _create_at(Node_ptr pos, Args&&... args) noexcept {
            pos->next = std::make_shared<Base_node>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void
        _create_at_last(Args&&... args) noexcept {
            Node_ptr _curr = _last_ptr();
            _curr->next = std::make_shared<Base_node>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void
        _create_at_front(Args&&... args) noexcept {
            std::shared_ptr<Base_node> _new = std::make_shared<Base_node>(std::forward<Args>(args)...);
            _new->next.swap(head);
            head.swap(_new);
        }

        void
        _head_reNext() noexcept {
            head = head->next;
            --size;
        }

        [[nodiscard]]
        Node_ptr
        _last_ptr() const noexcept {
            Node_ptr _curr = head.get();
            while (_curr->next) {
                _curr = _curr->next.get();
            }
            return _curr;
        }

        [[nodiscard]]
        bool _curr_not_null(Node_ptr __this_node) noexcept{
            return __this_node == nullptr;
        }

        [[nodiscard]]
        bool _curr_is_head_and_headNext_n_null(Node_ptr __this_node) noexcept {
            return __this_node == head.get() && head->next;
        }

        [[nodiscard]]
        bool _curr_is_head_and_headNext_null(Node_ptr __this_node) noexcept {
            return __this_node == head.get() && !head->next;
        }

        void _earse_at_back() noexcept {
            auto __temp = head.get();
            while(__temp->next->next){
                __temp = __temp->next.get();
            }
            __temp->next = __temp->next->next;
        }

        //NO NULL-CHECKING FOR NEXT NODE, MAKE SURE NEXT NODE IS NOT NULL
        void _pop_back_node(Node_ptr __begin, Node_ptr __end) noexcept {
            while (__begin) {
                if (__begin->next.get() == __end)
                    __begin->next = __begin->next->next;
                __begin = __begin->next.get();
            }
            --size;
        }

        [[nodiscard]]
        bool _next_is_null() noexcept{
            auto __ptr = head.get();
            return __ptr->next == nullptr;
        }

        [[nodiscard]]
        Node_ptr _M_before_this_node(Node_ptr pos) noexcept {
            auto __curr = head.get();
            while(__curr->next.get() != pos){
                __curr = __curr->next.get();
            }
            return __curr;
        }

    public:

        struct iterator {

            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            Node_ptr this_node;

            explicit
            iterator(Node_ptr a_ptr) noexcept
                : this_node(a_ptr) {
            }

            [[nodiscard]]
            reference
            operator*() const {
                return this_node->m_data;
            }

            [[nodiscard]]
            pointer
            operator->() const {
                return std::addressof(this_node->m_data);
            }

            //default constructor
            iterator&
                operator++() {
                if (this_node) this_node = this_node->next.get();
                return *this;
            }

            iterator
            operator++(int) {
                iterator buff {*this};
                ++(*this);
                return buff;
            }

            [[nodiscard]]
            friend
            bool operator==(const iterator& lhs, const iterator& rhs) noexcept {
                return lhs.this_node == rhs.this_node;
            }

            [[nodiscard]]
            friend
            bool operator!=(const iterator& lhs, const iterator& rhs) noexcept {
                return lhs.this_node != rhs.this_node;
            }

            [[nodiscard]]
            explicit
            operator bool() const {
                return this_node != nullptr;
            }

            [[nodiscard]]
            friend
            iterator operator+(const iterator& __iter, int __index) noexcept {
                iterator __buffe {__iter};
                for (;__index > 0; --__index)
                    ++__buffe;
                return __buffe;
            }

            [[nodiscard]]
            friend
            iterator operator+(int __index, const iterator& __iter) noexcept {
                return __iter + __index;
            }
        };

        struct const_iterator{
            using Base_const = const Node_ptr;

            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = const T*;
            using reference = const T&;

            const Base_node* this_node;

            explicit
            const_iterator(Base_const ptr) noexcept
                : this_node(ptr) {
            }

            [[nodiscard]]
            const_iterator operator++(int) noexcept {
                const_iterator __temp{*this};
                ++(*this);
                return __temp;
            }

            [[nodiscard]]
            const_iterator& operator++() noexcept {
                if(this_node) this_node = this_node->next.get();
                return *this;
            }

            reference operator*() const noexcept {
                return this_node->m_data;
            }

            pointer operator->() const noexcept {
                return std::addressof(this_node->m_data);
            }

            [[nodiscard]]
            friend
            bool operator==(const const_iterator& lhs, const const_iterator& rhs) noexcept {
                return lhs.this_node == rhs.this_node;
            }

            [[nodiscard]]
            friend
            bool operator!=(const const_iterator& lhs, const const_iterator& rhs) noexcept {
                return lhs.this_node != rhs.this_node;
            }

            [[nodiscard]]
            explicit
            operator bool() const {
                return this_node != nullptr;
            }

            [[nodiscard]]
            friend
            const_iterator operator+(const const_iterator& __iter, int __index) noexcept {
                const_iterator __buffe {__iter};
                for (;__index > 0; --__index)
                    ++__buffe;
                return __buffe;
            }

            [[nodiscard]]
            friend
            const_iterator operator+(int __index, const const_iterator& __iter) noexcept {
                return __iter + __index;
            }
        };

        template<typename InputIterator,
        typename = typename std::enable_if<std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category,std::input_iterator_tag>::value>::type>
        f_list(InputIterator first, InputIterator last) noexcept
            : size(0U)  {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

        f_list() noexcept
            : size(0U) {
        }

        f_list(std::initializer_list<T> __init_list)
            : size(0U) {
            for (const auto& i : __init_list)
                emplace_back(i);
        }


        size_t length() const noexcept {
            return size;
        }

        bool empty() const noexcept {
            return head == nullptr;
        }

        T front() const noexcept {
            return head->m_data;
        }

        template<typename... Args>
        void emplace_front(Args&&... args) noexcept {
            if (empty()) _create_at_head(std::forward<Args>(args)...);
            else {
                _create_at_front(std::forward<Args>(args)...);
            }
            ++size;
        }

        template<typename... Args>
        void emplace_back(Args&&... args) noexcept {
            if (empty()) _create_at_head(std::forward<Args>(args)...);
            else {
                _create_at_last(std::forward<Args>(args)...);
            }
            ++size;
        }

        void push_front(T&& _data) noexcept {
            emplace_front(std::move(_data));
        }

        void push_front(const T& _data) noexcept {
            emplace_front(_data);
        }

        void push_back(T&& _data) noexcept {
            emplace_back(std::move(_data));
        }

        void push_back(const T& _data) noexcept {
            emplace_back(_data);
        }

        [[nodiscard]]
        iterator begin() noexcept {
            return iterator{head.get()};
        }

        [[nodiscard]]
        iterator end() noexcept {
            return iterator{END};
        }

        template<typename Val>
        [[nodiscard]]
        iterator find(const Val& __val) noexcept {
            auto __ptr = head.get();
            while (__ptr) {
                if (__ptr->m_data == __val)
                    return iterator{__ptr};
                __ptr = __ptr->next.get();
            }
            return end();
        }

         template<typename Val>
        [[nodiscard]]
        const_iterator find(const Val& __val) const noexcept {
            auto __ptr = head.get();
            while (__ptr) {
                if (__ptr->m_data == __val)
                    return iterator{__ptr};
                __ptr = __ptr->next.get();
            }
            return end();
        }

        void clear() noexcept {
            head.reset();
            size = 0;
        }

        void erase(iterator __begin, iterator __end) noexcept {
            while (__begin != __end) {
                auto __curr = __begin;
                ++__begin;
                erase(__curr);
            }
        }

        void erase(iterator pos) noexcept {

            auto __begin = head.get();

            if (_curr_not_null(pos.this_node)) return;

            if (_curr_is_head_and_headNext_null(pos.this_node)){
                clear(); 
                return;
            }
            if (_curr_is_head_and_headNext_n_null(pos.this_node)) {
                _head_reNext();
                return;
            }

            _pop_back_node(__begin,pos.this_node);
        }

        void swap(f_list& __inst) noexcept {
            head.swap(__inst.head);
            std::swap(size,__inst.size);
        }

        void pop_front() noexcept {
            if (empty()) return;
            _head_reNext();
        }

        void pop_back() noexcept {
            if(empty()) return;
            if(_next_is_null()){
                clear();
                return;
            } 
            _earse_at_back();
            --size;
        }

        void replace_at(iterator pos, const T& arg) noexcept {
            if(empty() || pos == end()) return;

            if(_curr_is_head_and_headNext_null(pos.this_node)) {
                head = std::make_shared<Base_node>(arg);
                return;
            }


            if(_curr_is_head_and_headNext_n_null(pos.this_node)){
                std::shared_ptr<Base_node> __temp = head->next;
                head = std::make_shared<Base_node>(arg);
                head->next.swap(__temp);
                return;
            }

            auto __prev = _M_before_this_node(pos.this_node);
            std::shared_ptr<Base_node> __temp = __prev->next->next;
            _create_at(arg);
            __prev->next->next.swap(__temp);
        }

        void replace_at(iterator pos, T&& arg) noexcept {
            if(empty() || pos == end()) return;

            if(_curr_is_head_and_headNext_null(pos.this_node)) {
                head = std::make_shared<Base_node>(arg);
                return;
            }

            if(_curr_is_head_and_headNext_n_null(pos.this_node)){
                std::shared_ptr<Base_node> __temp = head->next;
                head = std::make_shared<Base_node>(arg);
                head->next.swap(__temp);
                return;
            }

            auto __prev = _M_before_this_node(pos.this_node);
            std::shared_ptr<Base_node> __temp = __prev->next->next;
            _create_at(__prev,std::move(arg));
            __prev->next->next.swap(__temp);
        }

        //overload operator+=, No support for move semantics
        f_list& operator+=(const f_list& __val) noexcept {
            if(empty()){ 
                head = __val.head;
                return *this;
            }
            else{
                auto __temp = _last_ptr();
                __temp->next = __val.head;
                return *this;
            }  
            ++size;
        }

        [[nodiscard]]
        const_iterator cbegin() const {
            return const_iterator{head.get()};
        }

        [[nodiscard]]
        const_iterator cend() const {
            return const_iterator{END};
        }
    };
}
#endif //F_LIST_H
