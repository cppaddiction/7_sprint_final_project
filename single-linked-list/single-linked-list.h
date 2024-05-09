#include <new>
#include <string>
#include <cassert>
#include <cstddef>
#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
        };
        
    template <typename ValueType>
    class BasicIterator {
    
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node)
        :node_(node)
        {
        }

    public:
    
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
        :node_(other.node_)
        { 
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_==rhs.node_? true:false;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_!=rhs.node_? true:false;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_==rhs.node_? true:false;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_!=rhs.node_? true:false;
        }
                
        BasicIterator& operator++() noexcept {
            assert(node_!=nullptr);
            node_=node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator it=*this;
            ++(*this);
            return it;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_!=nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_!=nullptr);
            return &node_->value;
        }

    private:
    
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList(std::initializer_list<Type> values) {
        CopyAndSwap(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        CopyAndSwap(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this!=&rhs)
        {
            CopyAndSwap(rhs);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }
    
    SingleLinkedList()=default;

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return static_cast<int>(size_)==0 ? true : false;
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept {
        while(head_.next_node!=nullptr)
        {
            auto temp=head_.next_node;
            head_.next_node=(*head_.next_node).next_node;
            delete temp;
        }
        size_=0;
    }
    
    ~SingleLinkedList()
    {
        Clear();
    }
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        if(size_!=0)
            return Iterator(head_.next_node);
        else
            return end();
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        if(size_!=0)
            return ConstIterator(head_.next_node);
        else
            return end();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        if(size_!=0)
            return ConstIterator(head_.next_node);
        else
            return cend();
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_!=nullptr);
        ConstIterator old_pos=pos;
        Node* new_node=new Node(value, (++pos).node_);
        old_pos.node_->next_node=new_node;
        Iterator result((++old_pos).node_);
        size_+=1;
        return result;
    }

    void PopFront() noexcept {
        if(!IsEmpty())
        {
            auto first=head_.next_node;
            head_.next_node=head_.next_node->next_node;
            delete first;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_!=nullptr);
        ConstIterator old_pos=pos;
        old_pos.node_->next_node=(++pos).node_->next_node;
        delete pos.node_;
        Iterator result(old_pos.node_->next_node);
        return result;
    }

    private:
        template<typename Iterable>
        void CopyAndSwap(const Iterable& other)
        {
            SingleLinkedList temp;
            ConstIterator it=temp.before_begin();
            for(const auto& val: other)
            {
                auto result=temp.InsertAfter(it, val);
                it=result;
            }
            swap(temp);
        }
        Node head_={};
        size_t size_=0;
};

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs==rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs<rhs)||(lhs==rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs<=rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs<rhs);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs)
{
    lhs.swap(rhs);
} 