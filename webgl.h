// webgl.h
//
// dump character/item models, textures and animations into binary archives

#pragma once
#include "common.h"
#include "math3d.h"
#include "types/Appearance.h"
#include "types/Anim.h"

namespace WebGL {
#pragma pack(push, 1)
  struct ModelHeader {
    uint32 numBones;
    uint32 boneOffset;
    uint32 numHardpoints;
    uint32 hardpointOffset;
    uint32 numObjects;
    uint32 objectOffset;
    uint32 numMaterials;
    uint32 numAppearances;
    uint32 materialOffset;
    Vector center;
  };
  struct Bone {
    char name[64];
    uint32 parent;
    Matrix bind;
  };
  struct Hardpoint {
    char name[64];
    uint32 parent;
    Matrix transform;
  };
  struct Material {
    uint32 diffuse;
    uint32 specular;
    uint32 tintBase;
    uint32 tintMask;
  };
  struct Object {
    uint32 material;
    uint32 numGroups;
    uint32 groupOffset;
  };
  struct ObjectGroup {
    uint32 numBones;
    uint32 boneOffset;
    uint32 numVertices;
    uint32 vertexOffset;
    uint32 numIndices;
    uint32 indexOffset;
  };
  struct Vertex {
    Vector position;
    int8 normal[4];
    int16 texcoord[2];
    uint8 bone_idx[4];
    float bone_weight[3];
  };
  typedef uint32 BoneIndex;
  typedef uint16 Index;

  struct TranslationKey {
    uint32 frame;
    Vector value;
  };
  struct RotationKey {
    uint32 frame;
    Quaternion value;
  };
  struct ScaleKey {
    uint32 frame;
    float value;
  };
  struct AnimationCurve {
    char bone[64];
    uint32 numTranslations;
    uint32 translationOffset;
    uint32 numRotations;
    uint32 rotationOffset;
    uint32 numScales;
    uint32 scaleOffset;
  };
  struct AnimationSequence {
    uint32 numFrames;
    float velocity;
    uint32 numBones;
    uint32 animationOffset;
  };
#pragma pack(pop)

  void WriteModel(std::string const& name);
  void WriteAnimation(std::string const& name);

  void AllItems(bool models, bool info, bool load);
  void AnimSets();
  void ItemInfo();
  void ClassInfo();
  void GenericItems();

  class Archive {
    std::map<uint32, MemoryFile> files_;
  public:
    Archive() {}
    Archive(File& file, bool compression = true) {
      load(file, compression);
    }
    void load(File& file, bool compression = true);
    bool has(uint32 id);
    File& create(uint32 id);

    static void compare(File& diff, Archive& lhs, Archive& rhs, char const*(*Func)(uint32) = nullptr);

    void write(File& file, bool compression = true);
  };
}
