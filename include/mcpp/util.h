#pragma once

#include "block.h"
#include <cstddef>
#include <iterator>
#include <ostream>
#include <vector>

/** @file
 * @brief Coordinate class.
 *
 */
namespace mcpp {

struct Coordinate2D;

/**
 * Represented using integers since sub-unit coordinates are not of particular
 * relevance. Allows for operations such as addition between coordinates.
 */
struct Coordinate {
    /**
     * @brief Constructs a Coordinate object with integer values.
     *
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @param z The z-coordinate.
     */
    constexpr Coordinate(int x, int y, int z) : x(x), y(y), z(z) {}

    /**
     * @brief Constructs a Coordinate object with zero values.
     */
    constexpr Coordinate() : x(0), y(0), z(0) {}

    /**
     * @brief Constructs a Coordinate object with double values.
     *
     * @param x The x-coordinate as a double.
     * @param y The y-coordinate as a double.
     * @param z The z-coordinate as a double.
     */
    constexpr Coordinate(double x, double y, double z)
        : x(static_cast<int>(x)), y(static_cast<int>(y)),
          z(static_cast<int>(z)) {}

    /**
     * @brief Adds two Coordinate objects.
     *
     * @param obj The Coordinate object to add.
     * @return A new Coordinate object representing the sum of the two
     * coordinates.
     */
    Coordinate operator+(const Coordinate& obj) const;

    /**
     * @brief Checks if two Coordinate objects are equal.
     *
     * @param obj The Coordinate object to compare with.
     * @return True if the coordinates are equal, false otherwise.
     */
    bool operator==(const Coordinate& obj) const;

    /**
     * @brief Checks if two Coordinate objects are not equal.
     *
     * @param obj The Coordinate object to compare with.
     * @return True if the coordinates are not equal, false otherwise.
     */
    bool operator!=(const Coordinate& obj) const;

    /**
     * @brief Subtracts one Coordinate object from another.
     *
     * @param obj The Coordinate object to subtract.
     * @return A new Coordinate object representing the difference between the
     * two coordinates.
     */
    Coordinate operator-(const Coordinate& obj) const;

    /**
     * @brief Creates a copy of the Coordinate object.
     *
     * @return A new Coordinate object that is a copy of the current object.
     */
    [[nodiscard]] Coordinate clone() const;

    /**
     * @brief Outputs the Coordinate object to an ostream.
     *
     * @param out The output stream.
     * @param coord The Coordinate object to output.
     * @return The output stream with the Coordinate object's values.
     */
    friend std::ostream& operator<<(std::ostream& out, const Coordinate& coord);

    int x;
    int y;
    int z;
};

/**
 * @brief Height-agnostic coordinate class.
 *
 * Represented using integers since sub-unit coordinates are not of particular
 * relevance. Allows for operations such as addition between flat coordinates.
 */
struct Coordinate2D {
    /**
     * @brief Constructs a Coordinate2D object with integer values.
     *
     * @param x The x-coordinate.
     * @param z The z-coordinate.
     */
    constexpr Coordinate2D(int x, int z) : x(x), z(z) {}

    /**
     * @brief Constructs a Coordinate2D object with zero values.
     */
    constexpr Coordinate2D() : x(0), z(0) {}

    /**
     * @brief Constructs a Coordinate2D object with double values.
     *
     * @param x The x-coordinate as a double.
     * @param z The z-coordinate as a double.
     */
    constexpr Coordinate2D(double x, double z)
        : x(static_cast<int>(x)), z(static_cast<int>(z)) {}

    /**
     * @brief Constructs a Coordinate2D object from a Coordinate object.
     *
     * @param coord The Coordinate object.
     */
    constexpr Coordinate2D(const Coordinate& coord) : x(coord.x), z(coord.z) {}

    /**
     * @brief Constructs a Coordinate object from a Coordinate2D object and a
     * y value.
     *
     * @param coord The Coordinate2D object.
     * @param y The y value.
     */
    constexpr Coordinate withHeight(int y) const;

    /**
     * @brief Adds two Coordinate2D objects.
     *
     * @param obj The Coordinate2D object to add.
     * @return A new Coordinate2D object representing the sum of the two
     * coordinates.
     */
    Coordinate2D operator+(const Coordinate2D& obj) const;

    // TODO: Add Coordinate + Coordinate2D

    /**
     * @brief Checks if two Coordinate2D objects are equal.
     *
     * @param obj The Coordinate2D object to compare with.
     * @return True if the flat coordinates are equal, false otherwise.
     */
    bool operator==(const Coordinate2D& obj) const;

    /**
     * @brief Checks if two Coordinate2D objects are not equal.
     *
     * @param obj The Coordinate2D object to compare with.
     * @return True if the flat coordinates are not equal, false otherwise.
     */
    bool operator!=(const Coordinate2D& obj) const;

    /**
     * @brief Subtracts one Coordinate2D object from another.
     *
     * @param obj The Coordinate2D object to subtract.
     * @return A new Coordinate2D object representing the difference between
     * the two coordinates.
     */
    Coordinate2D operator-(const Coordinate2D& obj) const;

    /**
     * @brief Creates a copy of the Coordinate2D object.
     *
     * @return A new Coordinate2D object that is a copy of the current object.
     */
    [[nodiscard]] Coordinate2D clone() const;

    /**
     * @brief Outputs the Coordinate2D object to an ostream.
     *
     * @param out The output stream.
     * @param coord The Coordinate2D object to output.
     * @return The output stream with the Coordinate object's values.
     */
    friend std::ostream& operator<<(std::ostream& out,
                                    const Coordinate2D& coord);

    int x;
    int z;
};

constexpr Coordinate Coordinate2D::withHeight(int y) const {
    return Coordinate(this->x, y, this->z);
}

/**
 * Stores a 3D cuboid of BlockTypes while preserving their relative location to
 * the base point they were gathered at and each other.
 */
struct Chunk {
    /**
     * @brief An iterator for the Chunk's 3D block data.
     *
     * This iterator allows for range-based for loops and standard iterator
     * operations over the 3D block data stored within a Chunk. It provides a
     * linear interface to traverse the 3D grid of blocks, enabling sequential
     * access to the elements stored in the chunk.
     */
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BlockType;
        using pointer = BlockType*;
        using reference = BlockType&;

        /**
         * @brief Constructs an iterator at the given pointer position.
         *
         * @param ptr Pointer to the position in the height array.
         */
        Iterator(pointer ptr) : m_ptr(ptr) {}

        /**
         * @brief Dereference the iterator to access the value at the current
         * position.
         *
         * @return Reference to the current element.
         */
        reference operator*() const { return *m_ptr; }

        /**
         * @brief Access the pointer to the current element.
         *
         * @return Pointer to the current element.
         */
        pointer operator->() { return m_ptr; }

        /**
         * @brief Pre-increment operator. Advances the iterator to the next
         * position.
         *
         * @return Reference to the updated iterator.
         */
        Iterator& operator++() {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Post-increment operator. Advances the iterator to the next
         * position.
         *
         * @param int Unused dummy parameter to differentiate from prefix
         * increment.
         * @return Iterator to the original position before incrementing.
         */
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if both iterators point to the same position, false
         * otherwise.
         */
        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        };

        /**
         * @brief Inequality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if iterators point to different positions, false
         * otherwise.
         */
        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        };

      private:
        pointer m_ptr;
    };

    /**
     * @brief An iterator for the const Chunk's 3D block data.
     *
     * This iterator allows for range-based for loops and standard const
     * iterator operations over the 3D block data stored within a Chunk. It
     * provides a linear interface to traverse the 3D grid of blocks, enabling
     * sequential immutable access to the elements stored in the chunk.
     */
    struct ConstIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BlockType;
        using pointer = const BlockType*;
        using reference = const BlockType&;

        /**
         * @brief Constructs an iterator at the given pointer position.
         *
         * @param ptr Pointer to the position in the height array.
         */
        ConstIterator(pointer ptr) : m_ptr(ptr) {}

        /**
         * @brief Dereference the iterator to access the value at the current
         * position.
         *
         * @return Reference to the current element.
         */
        reference operator*() const { return *m_ptr; }

        /**
         * @brief Access the pointer to the current element.
         *
         * @return Pointer to the current element.
         */
        pointer operator->() { return m_ptr; }

        /**
         * @brief Pre-increment operator. Advances the iterator to the next
         * position.
         *
         * @return Reference to the updated iterator.
         */
        ConstIterator& operator++() {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Post-increment operator. Advances the iterator to the next
         * position.
         *
         * @param int Unused dummy parameter to differentiate from prefix
         * increment.
         * @return Iterator to the original position before incrementing.
         */
        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if both iterators point to the same position, false
         * otherwise.
         */
        friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
            return a.m_ptr == b.m_ptr;
        };

        /**
         * @brief Inequality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if iterators point to different positions, false
         * otherwise.
         */
        friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
            return a.m_ptr != b.m_ptr;
        };

      private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&raw_data[0]); }
    Iterator end() { return Iterator(&raw_data[_x_len * _y_len * _z_len]); }
    ConstIterator begin() const { return ConstIterator(&raw_data[0]); }
    ConstIterator end() const {
        return ConstIterator(&raw_data[_x_len * _y_len * _z_len]);
    }

    /**
     * Initialized by copying from a flat vector of blocks
     */
    Chunk(const Coordinate& loc1, const Coordinate& loc2,
          const std::vector<BlockType>& block_list);

    ~Chunk();

    Chunk& operator=(const Chunk& other) noexcept;

    /**
     * Accesses the Minecraft block at absolute position pos and returns its
     * BlockType if it is in the included area.
     * @param pos: Abolute position in the Minecraft world to query BlockType
     * for
     * @return BlockType at specified location
     */
    BlockType get_worldspace(const Coordinate& pos) const;

    /**
     * Local equivalent of get_worldspace, equivalent to a 3D array access of
     * the internal data.
     * @param x: x element of array access
     * @param y: y element of array access
     * @param z: z element of array access
     * @return BlockType at specified location
     */
    BlockType get(int x, int y, int z) const;

    /**
     * Gets the x length of the Chunk.
     * @return x length of the Chunk
     */
    int x_len() const;

    /**
     * Gets the y length of the Chunk.
     * @return y length of the Chunk
     */
    int y_len() const;

    /**
     * Gets the z length of the Chunk.
     * @return z length of the Chunk
     */
    int z_len() const;

    /**
     * Gets the minimum coordinate in the Chunk.
     * @return the minimum coordinate in the Chunk
     */
    Coordinate base_pt() const;

  private:
    Coordinate _base_pt;
    int _y_len;
    int _x_len;
    int _z_len;
    BlockType* raw_data;
};

/**
 * Represents a 2D area of the world with the y coordinates of the highest
 * non-air blocks at each (x,z)
 */
struct HeightMap {
    /**
     * @brief An iterator for the HeightMap structure.
     *
     * This iterator allows for range-based for loops and standard iterator
     * operations over the height data stored within a HeightMap.
     */
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = int*;
        using reference = int&;

        /**
         * @brief Constructs an iterator at the given pointer position.
         *
         * @param ptr Pointer to the position in the height array.
         */
        Iterator(pointer ptr) : m_ptr(ptr) {}

        /**
         * @brief Dereference the iterator to access the value at the current
         * position.
         *
         * @return Reference to the current element.
         */
        reference operator*() const { return *m_ptr; }

        /**
         * @brief Access the pointer to the current element.
         *
         * @return Pointer to the current element.
         */
        pointer operator->() { return m_ptr; }

        /**
         * @brief Pre-increment operator. Advances the iterator to the next
         * position.
         *
         * @return Reference to the updated iterator.
         */
        Iterator& operator++() {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Post-increment operator. Advances the iterator to the next
         * position.
         *
         * @param int Unused dummy parameter to differentiate from prefix
         * increment.
         * @return Iterator to the original position before incrementing.
         */
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if both iterators point to the same position, false
         * otherwise.
         */
        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        };

        /**
         * @brief Inequality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if iterators point to different positions, false
         * otherwise.
         */
        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        };

      private:
        pointer m_ptr;
    };

    /**
     * @brief An iterator for the const HeightMap structure.
     *
     * This iterator allows for range-based for loops and standard const
     * iterator operations over the height data stored within a HeightMap.
     */
    struct ConstIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = int;
        using pointer = const int*;
        using reference = const int&;

        /**
         * @brief Constructs an iterator at the given pointer position.
         *
         * @param ptr Pointer to the position in the height array.
         */
        ConstIterator(pointer ptr) : m_ptr(ptr) {}

        /**
         * @brief Dereference the iterator to access the value at the current
         * position.
         *
         * @return Reference to the current element.
         */
        reference operator*() const { return *m_ptr; }

        /**
         * @brief Access the pointer to the current element.
         *
         * @return Pointer to the current element.
         */
        pointer operator->() { return m_ptr; }

        /**
         * @brief Pre-increment operator. Advances the iterator to the next
         * position.
         *
         * @return Reference to the updated iterator.
         */
        ConstIterator& operator++() {
            m_ptr++;
            return *this;
        }

        /**
         * @brief Post-increment operator. Advances the iterator to the next
         * position.
         *
         * @param int Unused dummy parameter to differentiate from prefix
         * increment.
         * @return Iterator to the original position before incrementing.
         */
        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if both iterators point to the same position, false
         * otherwise.
         */
        friend bool operator==(const ConstIterator& a, const ConstIterator& b) {
            return a.m_ptr == b.m_ptr;
        };

        /**
         * @brief Inequality comparison operator.
         *
         * @param a First iterator to compare.
         * @param b Second iterator to compare.
         * @return true if iterators point to different positions, false
         * otherwise.
         */
        friend bool operator!=(const ConstIterator& a, const ConstIterator& b) {
            return a.m_ptr != b.m_ptr;
        };

      private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&raw_heights[0]); }
    Iterator end() { return Iterator(&raw_heights[_x_len * _z_len]); }
    ConstIterator begin() const { return ConstIterator(&raw_heights[0]); }
    ConstIterator end() const {
        return ConstIterator(&raw_heights[_x_len * _z_len]);
    }

    HeightMap(const Coordinate2D& loc1, const Coordinate2D& loc2,
              const std::vector<int>& heights);

    ~HeightMap();

    HeightMap& operator=(const HeightMap& other) noexcept;

    /**
     * Get the height using an offset from the origin/base point of the heights
     * area
     * @param x: x offset to access underlying array
     * @param z: z offset to access underlying array
     * @return: height at specified offset
     */
    int get(int x, int z) const;

    /**
     * Get the height at a Minecraft coordinate if saved inside the height map
     * @param loc: 2D coordinate in Minecraft world to access in the map
     * @return: height at specified coordinate
     */
    int get_worldspace(const Coordinate2D& loc) const;

    /**
     * Fill a coordinate inplace with the highest y coordinate at the `loc`'s x
     * and z components.
     * @param loc: Coordinate to fill y value for
     */
    void fill_coord(Coordinate& out) const;

    /**
     * Gets the x length of the HeightMap.
     * @return x length of the HeightMap
     */
    int x_len() const;

    /**
     * Gets the z length of the HeightMap.
     * @return z length of the HeightMap
     */
    int z_len() const;

    /**
     * Gets the minimum 2D coordinate in the HeightMap.
     * @return the minimum 2D coordinate in the HeightMap.
     */
    Coordinate2D base_pt() const;

  private:
    Coordinate2D _base_pt;
    int _x_len;
    int _z_len;
    int* raw_heights;
};

} // namespace mcpp
