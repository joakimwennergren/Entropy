#pragma once

#include <functional>

class TextureId {
public:
  TextureId() : m_id(++ID) {}

  TextureId(unsigned int id) : m_id(id) {}

  bool operator<(const TextureId &rhs) const { return m_id < rhs.m_id; }

  bool operator==(const TextureId &rhs) const { return m_id == rhs.m_id; }

  bool operator!=(const TextureId &rhs) const { return m_id != rhs.m_id; }

  unsigned int GetId() const { return m_id; }

  bool IsValid() const { return m_id != InvalidId; }

  static TextureId MakeInvalid() { return TextureId(InvalidId); }

  static const unsigned int InvalidId = -1;

private:
  unsigned int m_id;

  static unsigned int ID;
};

namespace std {
template <> struct hash<TextureId> {
  std::size_t operator()(const TextureId &key) const noexcept {
    return std::hash<unsigned int>{}(key.GetId());
  }
};
} // namespace std