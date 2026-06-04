#ifndef SPARSE_SET_H
#define SPARSE_SET_H

#include <vector>

template <typename T>
class SparseSet
{

public:
    SparseSet()
    {
        this->dense.reserve(1024);
        this->sparse.reserve(1024);
        this->denseToSparse.reserve(1024);
    }

    size_t insert(T t)
    {
        size_t indexInDense = this->dense.size();
        this->dense.push_back(t);

        size_t sparseIndex;

        if (this->freeSpace.size() > 0) // il y a un trou dans sparse
        {
            sparseIndex = this->freeSpace.back();
            this->freeSpace.pop_back();
            this->sparse.at(sparseIndex) = indexInDense;
        }
        else
        {
            sparseIndex = this->sparse.size();
            this->sparse.push_back(indexInDense);
        }

        this->denseToSparse.push_back(sparseIndex);
        return sparseIndex;
    }

    T *at(size_t index)
    {
        size_t denseIndex = sparse[index];
        return &dense[denseIndex];
    }

    void delet(size_t index)
    {
        size_t denseIndex = this->sparse.at(index);
        size_t lastDenseIndex = this->dense.size() - 1;

        if (lastDenseIndex != denseIndex) /// Element n'est pas a la fin
        {
            dense.at(denseIndex) = std::move(dense.at(lastDenseIndex));

            // Récupère son handle sparse
            size_t movedSparseIndex = denseToSparse.at(lastDenseIndex);

            sparse.at(movedSparseIndex) = denseIndex;
            denseToSparse.at(denseIndex) = movedSparseIndex;
        }
        dense.pop_back();
        denseToSparse.pop_back();

        freeSpace.push_back(index);
    }

    std::vector<T> data()
    {
        return this->dense;
    }

private:
    std::vector<T> dense;
    std::vector<size_t> sparse;
    std::vector<size_t> denseToSparse;
    std::vector<size_t> freeSpace; // vector ou sont stocker les endroit vide dans sparse
};

#endif