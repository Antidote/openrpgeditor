#pragma once
#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Animation.hpp"
#include "Database/Skills.hpp"

class DBSkillsTab final : public IDBEditorTab {
public:
  DBSkillsTab() = delete;
  explicit DBSkillsTab(Skills& skills, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Skill>& skills() { return m_skills.skills(); }
  [[nodiscard]] const std::vector<Skill>& skills() const { return m_skills.skills(); }
  [[nodiscard]] Skill* skill(const int id) { return m_skills.skill(id); }
  [[nodiscard]] const Skill* skill(const int id) const { return m_skills.skill(id); }

private:
  Skills& m_skills;
  Skill* m_selectedSkill{};
  int m_maxSkills{};
  int m_editMaxSkills{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  std::vector<Effect> m_effects;
  EffectsEditor m_effectsEditor;
  int m_message_template{0};
  std::vector<std::string> m_message_templateList{"casts %1!", "does %1!", "uses %1"};
  std::optional<ObjectPicker<Animation>> m_animationPicker;
};