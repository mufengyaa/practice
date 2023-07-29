#include <iostream>
#include <assert.h>
#include <string.h>

using namespace std;

namespace mystring
{
    class string
    {
    public:
        typedef char *iterator;

        // iterator

        iterator begin() const
        {
            return _str;
        }
        iterator end() const
        {
            return _str + _size;
        }

        friend ostream &operator<<(ostream &_cout, const string &s)
        {
            for (auto c : s) // 由于是按照字符为单位打印,那么中间即使有\0也不影响它的打印,所以需要修改其他地方的字符处理函数,否则会遇到\0就结束
            {
                _cout << c;
            }
            return _cout;
        }
        friend istream &operator>>(istream &_cin, string &s)
        {
            char c;
            s.clear();
            //_cin>>c;  无法使用普通的cin来读入空格/回车,所以无法停止,对于cin来说,他俩都是用于结束的符号,不读入
            c = _cin.get();
            while (c != ' ' && c != '\n')
            {
                s += c;
                c = _cin.get();
            }
            return _cin;
        }

    public:
        string(const char *str = "") : _size(strlen(str))
        {
            _capacity = _size;
            _str = new char[_size + 1];
            // strcpy(_str, str);
            memcpy(_str, str, _size);
        }
        string(const string &s) // 拷贝构造
        {
            _str = new char[s._size];
            // strcpy(_str, s._str);
            memcpy(_str, s._str, s._size);
            _size = s._size;
            _capacity = s._capacity;
        }
        string &operator=(const string &s)
        {
            _str = new char[s._size];
            // strcpy(_str, s._str);
            memcpy(_str, s._str, s._size);
            _size = s._size;
            _capacity = s._capacity;
            return *this;
        }
        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }

        // modify

        void push_back(char c)
        {
            if (_size == _capacity)
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }
            _str[_size++] = c;
            _str[_size] = '\0';
        }
        string &operator+=(char c)
        {
            push_back(c);
            return *this;
        }
        void append(const char *str)
        {
            int len = strlen(str);
            if (_size + len > _capacity)
            {
                reserve(_size + len);
            }
            // strcpy(_str + _size, str);
            memcpy(_str + _size, str, len);
            _size += len;
        }
        string &operator+=(const char *str)
        {
            append(str);
            return *this;
        }
        void clear()
        {
            _size = 0;
            _str[0] = '\0';
        }
        void swap(string &s)
        {
            std::swap(_str, s._str);
            std::swap(_size, s._size);
            std::swap(_capacity, s._capacity);
        }
        const char *c_str() const
        {
            return _str;
        }

        // capacity

        size_t size() const
        {
            return _size;
        }
        size_t capacity() const
        {
            return _capacity;
        }
        bool empty() const
        {
            if (_size == 0)
            {
                return true;
            }
            return false;
        }
        void resize(size_t n, char c = '\0')
        {
            if (n > _size)
            {
                reserve(n);
                for (int i = _size; i < n; i++)
                {
                    _str[i] = c;
                }
            }
            _size = n;
            _str[n] = '\0';
        }
        void reserve(size_t n)
        {
            if (n > _capacity)
            {
                char *tmp = new char[n + 1];
                _capacity = n;
                // strcpy(tmp, _str);
                memcpy(tmp, _str, _size);
                delete[] _str;
                _str = tmp;
            }
        }

        // access

        char &operator[](size_t index)
        {
            assert(index < _size);
            return _str[index];
        }
        const char &operator[](size_t index) const
        {
            assert(index < _size);
            return _str[index];
        }

        // relational operators

        bool operator<(const string &s) const
        {
            int ret = memcmp(_str, s._str, _size < s._size ? _size : s._size);
            return ret == 0 ? _size < s._size : ret < 0;
        }
        bool operator<=(const string &s) const{
            return *this<s||*this==s;
        }
        bool operator>(const string &s) const{
            return !(*this<=s);
        }
        bool operator>=(const string &s) const{
            return !(*this<s);
        }
        bool operator==(const string &s) const{
            return _size==s._size&&memcmp(_str, s._str, _size );
        }
        bool operator!=(const string &s) const{
            return !(*this==s);
        }

        // 返回c在string中第一次出现的位置

        size_t find(char c, size_t pos = 0) const
        {
            assert(pos < _size);
            for (int i = 0; i < _size; i++)
            {
                if (_str[i] == c)
                {
                    return i;
                }
            }
            return npos;
        }

        // 返回子串s在string中第一次出现的位置

        size_t find(const char *s, size_t pos = 0) const
        {
            assert(pos < _size);
            const char *ppos = strstr(_str, s);
            return ppos - _str;
        }

        // 在pos位置上插入字符c/字符串str，并返回该字符的位置

        string &insert(size_t pos, int n, char c)
        {
            assert(pos >= 0);
            assert(pos <= _size);
            if (n + _size > _capacity)
            {
                reserve(n + _size);
            }
            size_t end = _size;
            for (int i = 0; i < (_size - pos + 1); i++) // 移动的次数
            {
                _str[end + n] = _str[end];
                end--;
            }
            // size_t end = _size + n + 1; //+1是为了让pos位置的字符可以被移动(需要被移动的次数是_size+1-pos)
            // while (end != n)  //因为这里判断的是是否相等,如果不+1,pos位置的字符不会被处理
            // {
            //     end--;  //放前面减是为了让 第一次循环的开始位置 正确
            //     _str[end] = _str[end - n];
            // }
            for (int i = 0; i < n; i++)
            {
                _str[pos + i] = c;
            }
            _size += n;
            return *this;
        }
        string &insert(size_t pos, const char *str)
        {
            assert(pos >= 0);
            assert(pos <= _size);
            int len = strlen(str);
            if (len + _size > _capacity)
            {
                reserve(len + _size);
            }
            size_t end = _size;
            for (int i = 0; i < (_size - pos + 1); i++) // 移动的次数
            {
                _str[end + len] = _str[end];
                end--;
            }
            for (int i = 0; i < len; i++)
            {
                _str[pos + i] = str[i];
            }
            _size += len;
            return *this;
        }

        // 删除pos位置上的元素，并返回该元素的下一个位置

        string &erase(size_t pos, size_t len = npos)
        {
            assert(pos >= 0);
            assert(pos <= _size);
            if (len == npos || len + pos > _size)
            {
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                int end = pos + len;
                while (end <= _size)
                {
                    _str[pos++] = _str[end++];
                }
                _size -= len;
            }
            return *this;
        }

    private:
        char *_str;
        size_t _capacity;
        size_t _size;
        static size_t npos;
    };
    size_t string::npos = -1;
};