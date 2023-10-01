#ifndef M_VECTOR_H
#define M_VECTOR_H

#include <iostream>
#include <cassert>

static const int INIT_CAPACITY = 100;
static const int REALLOC_COEFF = 2;
static const double VECTOR_LOAD_FACTOR = 0.9;
    static int called = 0;

template <typename T>
class M_vector 
{
protected:
    T *arr_ = nullptr;
    
    int size_ = 0;
    int capacity_ = 0;
    const T EMPTY;

public:
    M_vector (T empty, int capacity = INIT_CAPACITY);
    ~M_vector ();

    bool add (T &val);
    bool rm (int num);

    int get_size () {return size_;};
    int get_capacity () {return capacity_;};
    T operator [] (int num) const {return arr_[num];};
    
private:
    bool recalloc ();
};


template <typename T> M_vector<T>::M_vector (T empty, int capacity) : 
    EMPTY (empty),
    capacity_ (capacity)
{
    recalloc ();
}

template <typename T> M_vector<T>::~M_vector ()
{
    free (arr_);
    arr_ = nullptr;
}

template <typename T> bool M_vector<T>::recalloc ()
{
    if (!arr_)
    {    
        arr_ = (T *)std::calloc (capacity_, sizeof (T));
        return true;
    }
    else 
    {
        arr_ = (T *)std::realloc (arr_, sizeof (T) * capacity_ * REALLOC_COEFF);
        capacity_ *= REALLOC_COEFF;
        fprintf (stderr, "new capacity\n");
    }

    assert (arr_);
    if (!arr_ || !capacity_)
    {
        fprintf (stderr, "Error: failed to allocate Vector array.\n");
        return false;
    }

    return true;
}

template <typename T> bool M_vector<T>::rm (int rm_idx)
{
    assert (arr_);
    
    called--;
    if (!arr_)
    {
        fprintf (stderr, "Error: failed to remove array elem, array = [%p].\n", arr_);
        return false;
    }
    // arr_[rm_idx] = EMPTY;

    for (int idx = rm_idx; idx < size_ - 1; ++idx)
    {
        //swap ();
        arr_[idx] = arr_[idx + 1];
    }
    arr_[--size_] = EMPTY;
    return true;
}

template <typename T> bool M_vector<T>::add (T &val)
{
    assert (arr_);

    called++;
    if (((double)size_ / (double)capacity_) > VECTOR_LOAD_FACTOR)
    {
        bool status = recalloc ();
        if (status == false) return false;
    }

    arr_[size_++] = val;
    return true;
}


#endif /* M_VECTOR_H */