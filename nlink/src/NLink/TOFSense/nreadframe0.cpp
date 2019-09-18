#include "nreadframe0.h"
#include "../ndatatype.h"

namespace TOFSense {

#pragma pack(1)
struct Frame_t : public FrameBase<8> {
  char header[2]{0x57, 0x10};
  uint8_t reserved0[2]{0xff, 0xff};
  uint8_t id{};
  uint8_t reserved1[2]{0xff, 0xff};
  uint8_t checkSum{};
} frame_;
static_assert(sizeof(Frame_t) == Frame_t::skLength, "Error");
#pragma pack()

std::string commandRead(int id) {
  frame_.id = id;
  frame_.updateCheckSum();
  return frame_.byteArray();
}

} // namespace TOFSense
