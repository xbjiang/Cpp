#include <string>

template <typename T>
class NamedValue
{
private:
    const std::string& nameValue; // this two members have to be initialized with initialization list
    const T objectValue;
public:
    NamedValue(const char* name, const T& value);
    NamedValue(const std::string& name, const T& value);
};

template <typename T>
NamedValue<T>::NamedValue(const char* name, const T& value) : nameValue(name), objectValue(value)
{
}

template <typename T>
NamedValue<T>::NamedValue(const std::string& name, const T& value) : nameValue(name), objectValue(value)
{
}

int main()
{
    NamedValue<int> p("Jack", 2);
    NamedValue<int> s("Tom", 3);
    //p = s; // If there are const or reference members, compiler will not generate default assignment operator or copy constructor
}