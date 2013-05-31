#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector
{
public:
	Vector();
	Vector(unsigned int, const T*);
	Vector(const Vector&);
	~Vector();

	Vector& operator=(const Vector&);
	T& operator[](unsigned int) const;

	void addElement(const T&);
	void removeElement(unsigned int);
	void pop_back();
	void clear();
	T back();

	unsigned int getSize() const;
	bool elementExists(const T&) const;
	bool isEmpty() const;

private:
	unsigned int size;
	unsigned int end;
	T* array;

	void delVector();
	void copyVector(const Vector&);
};

template <typename T>
Vector<T>::Vector()
{
	size = end = 0;
	array = 0;
}

template <typename T>
Vector<T>::Vector(unsigned int _size, const T* _array)
{
	size = end = _size;
	array = new T[size];

	for (unsigned int i = 0; i < size; i++)
	{
		array[i] = _array[i];
	}
}

template <typename T>
Vector<T>::Vector(const Vector& other)
{
	copyVector(other);
}

template <typename T>
Vector<T>::~Vector()
{
	delVector();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other)
	{
		delVector();
		copyVector(other);
	}

	return *this;
}

template <typename T>
T& Vector<T>::operator[](unsigned int index) const
{
	return array[index];
}

template <typename T>
void Vector<T>::addElement(const T& element)
{
	if (end < size)
	{
		array[end] = element;
		end++;
	}
	else
	{
		T* holdingArray = array;
		
		array = new T[size + 42];
		for (unsigned int i = 0; i < size; i++)
		{
			array[i] = holdingArray[i];
		}

		array[end] = element;
		end++;
		
		size += 42;

		delete [] holdingArray;
	}
}

template <typename T>
void Vector<T>::removeElement(unsigned int index)
{
	if (!isEmpty())
	{
		for (unsigned int i = index + 1; i < end; i++)
		{
			array[i-1] = array[i];
		}

		end--;
	}
}

template <typename T>
void Vector<T>::pop_back()
{
	if (!isEmpty())
	{
		end--;
	}
}

template <typename T>
void Vector<T>::clear()
{
	for (int i = end - 1; i >= 0; i--)
	{
		removeElement(i);
	}
}

template <typename T>
T Vector<T>::back()
{
	return array[end - 1];
}

template <typename T>
unsigned int Vector<T>::getSize() const
{
	return end;
}

template <typename T>
bool Vector<T>::elementExists(const T& element) const
{
	if (!isEmpty())
	{
		for (unsigned int i = 0; i < end; i++)
		{
			if (array[i] == element)
			{
				return true;
			}
		}
	}

	return false;
}

template <typename T>
bool Vector<T>::isEmpty() const
{
	return !end;
}

template <typename T>
void Vector<T>::delVector()
{
	delete [] array;
}

template <typename T>
void Vector<T>::copyVector(const Vector& other)
{
	if (other.array)
	{
		size = other.size;
		end = other.end;

		array = new T[size];
		for (unsigned int i = 0; i < end; i++)
		{
			array[i] = other.array[i];
		}
	}
	else
	{
		size = end = 0;
		array = 0;
	}
}

#endif