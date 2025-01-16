#ifndef ENTROPY_ASSETID_H
#define ENTROPY_ASSETID_H

#include <functional>

struct AssetId {
    /**
     * Default constructor for the AssetId class.
     * Automatically generates and assigns a unique identifier.
     */
    AssetId()
        : m_id(++ID) {
    }

    /**
     * Constructs an AssetId object with the specified unique identifier.
     *
     * @param id The unique identifier to be assigned to the AssetId object.
     */
    explicit AssetId(const unsigned int id)
        : m_id(id) {
    }

    /**
     * Overloads a specific operator for the class or type.
     *
     * @param lhs The left-hand side operand for the operator.
     * @param rhs The right-hand side operand for the operator.
     *
     * @return The result of the operation based on the overloaded operator.
     */
    bool operator<(const AssetId &rhs) const {
        return m_id < rhs.m_id;
    }

    /**
     * Overloads the assignment operator to allow copying of one object
     * to another.
     *
     * @param rhs The right-hand side object to copy from.
     * @return A reference to the assigned object.
     */
    bool operator==(const AssetId &rhs) const {
        return m_id == rhs.m_id;
    }

    bool operator!=(const AssetId &rhs) const {
        return m_id != rhs.m_id;
    }

    [[nodiscard]] unsigned int GetId() const {
        return m_id;
    }

    [[nodiscard]] bool IsValid() const { return m_id != InvalidId; }

    static AssetId MakeInvalid() { return AssetId(InvalidId); }

    static constexpr unsigned int InvalidId = -1;

private:
    unsigned int m_id;
    static unsigned int ID;
};

template<>
struct std::hash<AssetId> {
    std::size_t operator()(const AssetId &key) const noexcept {
        return std::hash<unsigned int>{}(key.GetId());
    }
};

#endif // ENTROPY_ASSETID_H
