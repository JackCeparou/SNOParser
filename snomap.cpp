#include "snomap.h"
#include "file.h"
#include "types/GameBalance.h"
#include <Windows.h>

void SnoMap::parse(File& file, std::string const& name) {
  if (!file) return;
  if (file.read32() == 0xDEADBEEF) {
    file.seek(16);
    map_[file.read32()] = name;
  }
}
void SnoMap::save(std::string const& type) {
  File file(path::work() / fmtstring("sno_%08x", SnoLoader::default->hash()) / type + ".txt", "wt");
  for (const auto& kv : map_) {
    file.printf("%d %s\n", kv.first, kv.second.c_str());
  }
}
bool SnoMap::load(std::string const& type) {
  File file(path::work() / fmtstring("sno_%08x", SnoLoader::default->hash()) / type + ".txt", "rt");
  if (!file) return false;
  int id;
  char fname[512];
  for (auto& line : file) {
    size_t space = line.find(' ');
    if (space != std::string::npos) {
      map_[atoi(line.c_str())] = line.substr(space + 1);
    }
  }
  return true;
}

template<class T>
static void insert(std::map<uint32, std::string>& dst, T const& src) {
  for (auto& entry : src) {
    std::string name((char*)&entry);
    dst[HashNameLower(name)] = name;
  }
}

const SnoMap& SnoManager::gameBalance() {
  auto it = instance_.map_.find("GameBalanceId");
  if (it == instance_.map_.end()) {
    SnoMap& map = instance_.map_["GameBalanceId"];
    if (map.load("GameBalanceId")) return map;
    for (auto& gmb : SnoLoader::All<GameBalance>()) {
      insert(map.map_, gmb->x018_ItemTypes);
      insert(map.map_, gmb->x028_Items);
      insert(map.map_, gmb->x078_AffixTable);
      insert(map.map_, gmb->x088_Heros);
      insert(map.map_, gmb->x098_MovementStyles);
      insert(map.map_, gmb->x0A8_Labels);
      insert(map.map_, gmb->x0C8_RareItemNamesTable);
      insert(map.map_, gmb->x0D8_MonsterAffixesTable);
      insert(map.map_, gmb->x0E8_RareMonsterNamesTable);
      insert(map.map_, gmb->x0F8_SocketedEffectsTable);
      insert(map.map_, gmb->x108_ItemDropTable);
      insert(map.map_, gmb->x128_QualityClassTable);
      insert(map.map_, gmb->x158_Hirelings);
      insert(map.map_, gmb->x168_SetItemBonusTable);
      insert(map.map_, gmb->x178_EliteModifiers);
      insert(map.map_, gmb->x198_PowerFormulaTable);
      insert(map.map_, gmb->x1A8_RecipesTable);
      insert(map.map_, gmb->x1B8_ScriptedAchievementEventsTable);
      insert(map.map_, gmb->x1C8_LootRunQuestTierTable);
      insert(map.map_, gmb->x1D8_ParagonBonusesTable);
      insert(map.map_, gmb->x1E8_LegacyItemConversionTable);
      insert(map.map_, gmb->x218_TransmuteRecipesTable);
    }
    map.save("GameBalanceId");
    return map;
  } else {
    return it->second;
  }
}

SnoManager SnoManager::instance_;
