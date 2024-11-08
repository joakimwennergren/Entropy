#pragma once

#include <functional>

class AssetId {
public:
    AssetId()
        : m_id(++ID) {
    }

    explicit AssetId(unsigned int id)
        : m_id(id) {
    }

    bool operator<(const AssetId &rhs) const {
        return m_id < rhs.m_id;
    }

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
