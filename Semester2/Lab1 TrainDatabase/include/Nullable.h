#ifndef NULLABLE_H
#define NULLABLE_H

#include <cstring>

template<typename T>
struct Nullable
{
    Nullable();
    Nullable(T data);

    void set(T new_data);
    void set_null();

    bool get(T* return_data);
    bool is_null();

private:
    char data[sizeof(T) / sizeof(char)];
    bool null;
};


template<typename T>
Nullable<T>::Nullable(T data)
: null(false)
{
    memcpy(&(this->data), &data, sizeof(T) / sizeof(char));
}

template<typename T>
Nullable<T>::Nullable()
: null(true)
{}

template<typename T>
bool Nullable<T>::get(T* return_data)
{
    if(null)
        return false;
    memcpy(return_data, &data, sizeof(T) / sizeof(char));
    return true;
}

template<typename T>
bool Nullable<T>::is_null()
{
    return null;
}

template<typename T>
void Nullable<T>::set(T new_data)
{
    memcpy(&data, &new_data, sizeof(T) / sizeof(char));
    null = false;
}

template<typename T>
void Nullable<T>::set_null()
{
    null = true;
}

#endif // NULLABLE_H
