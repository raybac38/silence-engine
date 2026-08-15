#pragma once

#include <vector>
#include <iostream>
#include <cassert>

template <typename T>
class SparseSet
{
public:
    SparseSet(size_t initial_capacity = 1024)
    {
        this->sparse.reserve(initial_capacity);
        this->denseToSparse.reserve(initial_capacity);
    }

    void insert(size_t index, T value)
    {
        if (index >= sparse.size())
        {
            sparse.resize(index + 1, INVALID_INDEX);
        } // need more space

        if (sparse[index] != INVALID_INDEX) // already use, re-use
        {
            printf("[ERROR] Sparse Index : Insert an already existing element\n");
            exit(EXIT_FAILURE);
        }

        sparse[index] = dense.size();
        dense.push_back(std::move(value));
        denseToSparse.push_back(index);
    }

    T &at(size_t index)
    {
        if (index >= sparse.size() || sparse[index] == INVALID_INDEX)
        {
            printf("[ERROR] Sparse Index : Access an non-existing element\n");
            exit(EXIT_FAILURE);
        }
        return dense[sparse[index]];
    }

    void delet(size_t index)
    {
        if (index >= sparse.size() || sparse[index] == INVALID_INDEX)
        {
            printf("[ERROR] Sparse Index : Deleting an non-existing element\n");
            exit(EXIT_FAILURE);
        }

        size_t indexDenseASupprimer = sparse[index];
        size_t dernierIndexDense = dense.size() - 1;

        if (indexDenseASupprimer != dernierIndexDense)
        {
            T dernierElement = std::move(dense[dernierIndexDense]);
            size_t idDernierElement = denseToSparse[dernierIndexDense];

            // Permutation
            dense[indexDenseASupprimer] = std::move(dernierElement);
            denseToSparse[indexDenseASupprimer] = idDernierElement;

            // Update pointer
            sparse[idDernierElement] = indexDenseASupprimer;
        }

        // Remove last element
        dense.pop_back();
        denseToSparse.pop_back();
        sparse[index] = INVALID_INDEX;
    }

    // Does existe ?
    bool has(size_t index) const
    {
        return index < sparse.size() && sparse[index] != INVALID_INDEX;
    }

    std::vector<T> &data()
    {
        return this->dense;
    }

    size_t size() const
    {
        return dense.size();
    }

private:
    static constexpr size_t INVALID_INDEX = static_cast<size_t>(-1);
    std::vector<T> dense;
    std::vector<size_t> denseToSparse; // Indispensable pour la suppression !
    std::vector<size_t> sparse;
};
