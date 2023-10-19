
template <class iterator, class Ref, class Ptr>
struct myreverse_iterator
{
    typedef myreverse_iterator<iterator, Ref, Ptr> self;
    myreverse_iterator(iterator tmp)
    {
        _it = tmp;
    }
    Ref operator*()
    {
        iterator tmp = _it;
        return *(--tmp);
    }
    Ptr operator->()
    {
        return &(operator*());
    }
    self &operator++()
    {
        --_it;
        return (*this);
    }
    self operator++(int)
    {
        self tmp = *this;
        --_it;
        return (tmp);
    }
    self &operator--()
    {
        ++_it;
        return (*this);
    }
    self operator--(int)
    {
        self tmp = *this;
        ++_it;
        return (tmp);
    }
    bool operator!=(const self& it) const
    {
        return _it != it._it;
    }
    bool operator==(const self& it) const
    {
        return _it == it._it;
    }

    iterator _it;
};