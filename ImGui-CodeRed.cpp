#include "ImGui-CodeRed.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImUtils
{
	int32_t Stricmp(const char* s1, const char* s2)
	{
		int32_t d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d;
	}

	int32_t Strnicmp(const char* s1, const char* s2, int32_t n)
	{
		int32_t d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d;
	}

	char* Strdup(const char* s)
	{
		IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return reinterpret_cast<char*>(memcpy(buf, reinterpret_cast<const void*>(s), len));
	}

	void Strtrim(char* s)
	{
		char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0;
	}
}

ImStyle::ImStyle() : HardcodedStyle(false), StyleName("New Style")
{
	Initialize();
}

ImStyle::ImStyle(const ImStyle& imStyle) :  HardcodedStyle(false), StyleName(imStyle.StyleName)
{
	Initialize();
	SetSettings(imStyle.Settings);
	SetColors(imStyle.Colors);
}

ImStyle::ImStyle(const std::string& styleName, bool bHardcoded) : HardcodedStyle(bHardcoded), StyleName(styleName)
{
	Initialize();
}

ImStyle::ImStyle(const std::filesystem::path& styleFile, bool bHardcoded) : HardcodedStyle(bHardcoded), StyleName("New Style")
{
	Initialize();
	ParseFile(styleFile);
}

ImStyle::~ImStyle() {}

void ImStyle::Initialize()
{
	SetName(GetName());
	Settings.clear();
	Colors.clear();

	Settings =
	{
		{ ImSettingIds::IM_WindowPadding_X, DEFAULT_WINDOW_PADDING_X },
		{ ImSettingIds::IM_WindowPadding_Y, DEFAULT_WINDOW_PADDING_Y },
		{ ImSettingIds::IM_FramePadding_X, DEFAULT_FRAME_PADDING_X },
		{ ImSettingIds::IM_FramePadding_Y, DEFAULT_FRAME_PADDING_Y },
		{ ImSettingIds::IM_CellPadding_X, DEFAULT_CELL_PADDING_X },
		{ ImSettingIds::IM_CellPadding_Y, DEFAULT_CELL_PADDING_Y },
		{ ImSettingIds::IM_ItemSpacing_X, DEFAULT_ITEM_SPACING_X },
		{ ImSettingIds::IM_ItemSpacing_Y, DEFAULT_ITEM_SPACING_Y },
		{ ImSettingIds::IM_InnerSpacing_X, DEFAULT_INNER_SPACING_X },
		{ ImSettingIds::IM_InnerSpacing_Y, DEFAULT_INNER_SPACING_Y },
		{ ImSettingIds::IM_TouchPadding_X, DEFAULT_TOUCH_PADDING_X },
		{ ImSettingIds::IM_TouchPadding_Y, DEFAULT_TOUCH_PADDING_Y },
		{ ImSettingIds::IM_IndentSpacing, DEFAULT_INDENT_SPACING },
		{ ImSettingIds::IM_ScrollbarSize, DEFAULT_SCROLLBAR_SIZE },
		{ ImSettingIds::IM_GrabMinSize, DEFAULT_GRAB_MINI_SIZE },
		{ ImSettingIds::IM_WindowBorderSize, DEFAULT_WINDOW_BORDER_SIZE },
		{ ImSettingIds::IM_ChildBorderSize, DEFAULT_CHILD_BORDER_SIZE },
		{ ImSettingIds::IM_PopupBorderSize, DEFAULT_POPUP_BORDER_SIZE },
		{ ImSettingIds::IM_FrameBorderSize, DEFAULT_FRAME_BORDER_SIZE },
		{ ImSettingIds::IM_TabBorderSize, DEFAULT_TAB_BORDER_SIZE },
		{ ImSettingIds::IM_WindowRounding, DEFAULT_WINDOW_ROUNDING },
		{ ImSettingIds::IM_ChildRounding, DEFAULT_CHILD_ROUNDING },
		{ ImSettingIds::IM_FrameRounding, DEFAULT_FRAME_ROUNDING },
		{ ImSettingIds::IM_PopupRounding, DEFAULT_POPUP_ROUNDING },
		{ ImSettingIds::IM_ScrollbarRounding, DEFAULT_SCROLLBAR_ROUNDING },
		{ ImSettingIds::IM_GrabRounding, DEFAULT_GRAB_ROUNDING },
		{ ImSettingIds::IM_LogSliderDeadzone, DEFAULT_LOGSLIDER_DEADZONE },
		{ ImSettingIds::IM_TabRounding, DEFAULT_TAB_ROUNDING },
	};

	for (int32_t i = 0; i < static_cast<int32_t>(ImColorIds::IM_END); i++)
	{
		Colors.emplace(std::make_pair(static_cast<ImColorIds>(i), ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }));
	}
}

ImStyle ImStyle::GetCopy() const
{
	ImStyle thisCopy = *const_cast<ImStyle*>(this);
	thisCopy.HardcodedStyle = false;
	return thisCopy;
}

bool ImStyle::IsHardcoded() const
{
	return HardcodedStyle;
}

bool ImStyle::IsInitialized() const
{
	return (Settings.size() == static_cast<size_t>(ImSettingIds::IM_END) && Colors.size() == static_cast<size_t>(ImColorIds::IM_END));
}

const std::string& ImStyle::GetName() const
{
	return StyleName;
}

void ImStyle::SetName(const std::string& newName)
{
	StyleName = newName;
}

float* ImStyle::GetSetting(ImSettingIds settingId)
{
	if (IsInitialized())
	{
		return &Settings[settingId];
	}
	else
	{
		//Console.Warning("[ImStyle] [") + GetName() + "] Warning: Failed get setting, not properly initialized!");
	}

	return nullptr;
}

float* ImStyle::GetColor(ImColorIds colorId) 
{
	if (IsInitialized())
	{
		return reinterpret_cast<float*>(&Colors[colorId]);
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed get color, not properly initialized!");
	}

	return nullptr;
}

std::map<ImSettingIds, float> ImStyle::GetSettings() const
{
	return Settings;
}

std::map<ImColorIds, ImVec4> ImStyle::GetColors() const
{
	return Colors;
}

void ImStyle::SetSetting(ImSettingIds settingId, float newValue)
{
	if (IsInitialized())
	{
		Settings[settingId] = newValue;
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed set setting, not properly initialized!");
	}
}

void ImStyle::SetSettings(const std::map<ImSettingIds, float>& newSettings)
{
	Settings = newSettings;
}

void ImStyle::SetColor(ImColorIds colorId, const ImVec4& newColor)
{
	if (IsInitialized())
	{
		Colors[colorId] = newColor;
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed set color, not properly initialized!");
	}
}

void ImStyle::SetColors(const std::map<ImColorIds, ImVec4>& newColors)
{
	Colors = newColors;
}

void ImStyle::ApplyStyle(bool bSelect)
{
	if (IsInitialized())
	{
		ImGuiStyle& imGuiStyle = ImGui::GetStyle();
		ImVec4* imGuiColors = imGuiStyle.Colors;

		imGuiStyle.WindowRounding = Settings[ImSettingIds::IM_WindowRounding];
		imGuiStyle.ChildRounding = Settings[ImSettingIds::IM_ChildRounding];
		imGuiStyle.FrameRounding = Settings[ImSettingIds::IM_FrameRounding];
		imGuiStyle.PopupRounding = Settings[ImSettingIds::IM_PopupRounding];
		imGuiStyle.ScrollbarRounding = Settings[ImSettingIds::IM_ScrollbarRounding];
		imGuiStyle.GrabRounding = Settings[ImSettingIds::IM_GrabRounding];
		imGuiStyle.LogSliderDeadzone = Settings[ImSettingIds::IM_LogSliderDeadzone];
		imGuiStyle.TabRounding = Settings[ImSettingIds::IM_TabRounding];
		imGuiStyle.WindowBorderSize = Settings[ImSettingIds::IM_WindowBorderSize];
		imGuiStyle.ChildBorderSize = Settings[ImSettingIds::IM_ChildBorderSize];
		imGuiStyle.PopupBorderSize = Settings[ImSettingIds::IM_PopupBorderSize];
		imGuiStyle.FrameBorderSize = Settings[ImSettingIds::IM_FrameBorderSize];
		imGuiStyle.TabBorderSize = Settings[ImSettingIds::IM_TabBorderSize];
		imGuiStyle.WindowPadding = { Settings[ImSettingIds::IM_WindowPadding_X], Settings[ImSettingIds::IM_WindowPadding_Y] };
		imGuiStyle.FramePadding = { Settings[ImSettingIds::IM_FramePadding_X], Settings[ImSettingIds::IM_FramePadding_Y] };
		imGuiStyle.CellPadding = { Settings[ImSettingIds::IM_CellPadding_X], Settings[ImSettingIds::IM_CellPadding_Y] };
		imGuiStyle.ItemSpacing = { Settings[ImSettingIds::IM_ItemSpacing_X], Settings[ImSettingIds::IM_ItemSpacing_Y] };
		imGuiStyle.ItemInnerSpacing = { Settings[ImSettingIds::IM_InnerSpacing_X], Settings[ImSettingIds::IM_InnerSpacing_Y] };
		imGuiStyle.TouchExtraPadding = { Settings[ImSettingIds::IM_TouchPadding_X], Settings[ImSettingIds::IM_TouchPadding_Y] };
		imGuiStyle.IndentSpacing = Settings[ImSettingIds::IM_IndentSpacing];
		imGuiStyle.ScrollbarSize = Settings[ImSettingIds::IM_ScrollbarSize];
		imGuiStyle.GrabMinSize = Settings[ImSettingIds::IM_GrabMinSize];

		if (imGuiColors)
		{
			for (int32_t i = 0; i < static_cast<int32_t>(ImColorIds::IM_END); i++)
			{
				imGuiColors[i] = Colors[static_cast<ImColorIds>(i)];
			}

			if (bSelect)
			{
				//ImmediateComponent::SelectStyle(GetName());
			}
		}
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed to apply style, not properly initialized!");
	}
}

void ImStyle::SaveStyle()
{
	if (IsInitialized())
	{
		std::filesystem::path styleFile;// = FileManager.GetStylesFolder() / (Format::RemoveFileChars(GetName()) + ".crs");
		std::ofstream styleStream(styleFile, std::ios::trunc);

		styleStream << "{" << std::endl;
		styleStream << "\t\"StyleName\": \"" << StyleName << "\"," << std::endl;
		styleStream << "\t// Settings" << std::endl;

		for (int32_t i = 0; i < static_cast<int32_t>(ImSettingIds::IM_END); i++)
		{
			ImSettingIds settingId = static_cast<ImSettingIds>(i);
			styleStream << "\t\"" << ImSettingNames[settingId] << "\": " << std::to_string(Settings[settingId]) << "," << std::endl;
		}

		styleStream << "\t// Colors" << std::endl;

		for (int32_t i = 0; i < static_cast<int32_t>(ImColorIds::IM_END); i++)
		{
			ImColorIds colorId = static_cast<ImColorIds>(i);
			ImVec4 color = Colors[colorId];
			styleStream << "\t\"" << ImColorNames[colorId] << "\": [ " << std::to_string(color.x) << ", " << std::to_string(color.y) << ", " << std::to_string(color.z) << ", " << std::to_string(color.w) << " ]," << std::endl;
		}

		styleStream << "}";
		styleStream.close();

		if (!IsHardcoded())
		{
			//Console.Write("[ImStyle] [" + GetName() + "] Successfully saved style!", TextColors::Purple);
		}
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed to save style, not properly initialized!");
	}
}

void ImStyle::DeleteStyle()
{
	if (IsInitialized())
	{
		if (!IsHardcoded())
		{
			std::filesystem::path styleFile;// = FileManager.GetStylesFolder() / (GetName() + ".crs");

			if (std::filesystem::exists(styleFile) && styleFile.extension().u16string() == u".crs")
			{
				std::filesystem::remove(styleFile);
			}
			else
			{
				//Console.Error("[ImStyle] [") + GetName() + "] Error: Failed to delete style, style exists in memory but not locally!");
			}

			Settings.clear();
			Colors.clear();
		}
		else
		{
			//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed to delete style, you cannot delete or modify a default style!");
		}
	}
	else
	{
		//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Failed to delete style, not properly initialized!");
	}
}

void ImStyle::ParseFile(const std::filesystem::path& styleFile)
{
	if (!IsInitialized())
	{
		Initialize();
	}

	if (std::filesystem::exists(styleFile))
	{
		SetName(styleFile.stem().u8string());
		std::vector<std::string> fileLines;// = FileManagerComponent::ReadFileLines(styleFile);

		if (fileLines.size() > (static_cast<int32_t>(ImSettingIds::IM_END) + static_cast<int32_t>(ImColorIds::IM_END)))
		{
			for (std::string& line : fileLines)
			{
				if (line.find("StyleName") != std::string::npos)
				{
					size_t startPos = line.find(": \"");
					size_t endPos = line.find("\",");

					if (startPos != std::string::npos && endPos != std::string::npos)
					{
						SetName(line.substr(startPos + 3, (endPos - startPos) - 3));
					}

					continue;
				}

				Format::RemoveAllChars(line, ' ');

				for (auto settingIt = ImSettingNames.begin(); settingIt != ImSettingNames.end(); settingIt++)
				{
					size_t settingPos = line.find("\"") + settingIt->second + "\"");

					if (settingPos != std::string::npos)
					{
						size_t startPos = line.find(":");
						size_t endPos = line.find(",");

						if (startPos != std::string::npos && endPos != std::string::npos)
						{
							std::string settingStr = line.substr(startPos + 1, (endPos - startPos) - 1);
							SetSetting(settingIt->first, std::stof(settingStr));
						}
					}
				}

				for (auto colorIt = ImColorNames.begin(); colorIt != ImColorNames.end(); colorIt++)
				{
					size_t colorPos = line.find("\"") + colorIt->second + "\"");

					if (colorPos != std::string::npos)
					{
						size_t startPos = line.find("[");
						size_t endPos = line.find("]");

						if (startPos != std::string::npos && endPos != std::string::npos)
						{
							std::string colorStr = line.substr(startPos + 1, (endPos - startPos) - 1);
							std::vector<std::string> colorValues = Format::Split(colorStr, ',');

							if (colorValues.size() == 4)
							{
								SetColor(colorIt->first, ImVec4(std::stof(colorValues[0]), std::stof(colorValues[1]), std::stof(colorValues[2]), std::stof(colorValues[3])));
							}
							else
							{
								//Console.Warning("[ImStyle] [") + GetName() + "] Warning: Unsupported color format detected! Did you try to edit the file manually? Naughty naughty...");
								return;
							}
						}
					}
				}
			}

			//Console.Write("[ImStyle] [" + GetName() + "] Loaded style!", TextColors::Purple);
		}
		else
		{
			//Console.Warning("[ImStyle] [" + GetName() + "] Warning: Unsupported style format detected! Did you try to edit the file manually? Naughty naughty...");
		}
	}
	else
	{
		//Console.Warning("[ImStyle] [) + GetName() + "] Error: Failed to locate given file!");
	}
}

ImInterface::ImInterface(const std::string& title, const std::string& name, bool bShowCursor) : WindowTitle(title), WindowName(name), Attached(false), Render(false), ShowCursor(bShowCursor) {}

ImInterface::~ImInterface()
{
	OnDetatch();
}

const std::string& ImInterface::GetTitle() const
{
	return WindowTitle;
}

const std::string& ImInterface::GetName() const
{
	return WindowName;
}

bool ImInterface::ShouldShowCursor() const
{
	return ShowCursor;
}

bool ImInterface::IsAttached() const
{
	return Attached;
}

bool ImInterface::ShouldRender() const
{
	return (IsAttached() && Render);
}

bool* ImInterface::ShouldBegin()
{
	return &Render;
}

void ImInterface::SetAttached(bool bAttached)
{
	Attached = bAttached;
}

void ImInterface::SetShouldRender(bool bShouldRender)
{
	Render = bShouldRender;
}

void ImInterface::ToggleRender()
{
	Render = !Render;
}

void ImInterface::OnAttach()
{
	SetAttached(true);
}

void ImInterface::OnDetatch()
{
	if (IsAttached())
	{
		SetAttached(false);
	}
}

void ImInterface::OnRender() {}

ImDemo::ImDemo(const std::string& title, const std::string& name, bool bShowCursor) : ImInterface(title, name, bShowCursor) {}

ImDemo::~ImDemo()
{
	OnDetatch();
}

void ImDemo::OnAttach()
{
	SetAttached(true);
}

void ImDemo::OnDetatch()
{
	if (IsAttached())
	{
		SetAttached(false);
	}
}

void ImDemo::OnRender()
{
	if (ShouldRender())
	{
		ImGui::ShowDemoWindow(ShouldBegin());
	}
}

ImStyleManager::ImStyleManager(const std::string& title, const std::string& name, bool bShowCursor) : ImInterface(title, name, bShowCursor) {}

ImStyleManager::~ImStyleManager()
{
	OnDetatch();
}

void ImStyleManager::OnAttach()
{
	Initialize();
	SetAttached(true);
}

void ImStyleManager::OnDetatch()
{
	if (IsAttached())
	{
		SelectedStyle = nullptr;
		EditingStyle = nullptr;
		SetAttached(false);
	}
}

void ImStyleManager::OnRender()
{
	if (ShouldRender())
	{
		ImGui::SetNextWindowSize(ImVec2(500.0f, 610.0f), ImGuiCond_Once);

		if (ImGui::Begin(GetTitle().c_str(), ShouldBegin()))
		{
			if (ImGui::BeginTable("StyleTable###StyleManager_Table", 2))
			{
				ImGui::TableSetupColumn("PickerColumn", ImGuiTableColumnFlags_WidthFixed, 75.0f);
				ImGui::TableSetupColumn("NameColumn", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Style Picker");
				ImGui::TableSetColumnIndex(1);

				if (ImGui::BeginCombo("###StylePickerBox", SelectedName.c_str()))
				{
					for (const auto& stylePair : Immediate.StyleMap)
					{
						bool isSelected = (SelectedName == stylePair.first);
						if (ImGui::Selectable(stylePair.first.c_str(), isSelected)) { SelectStyle(stylePair.second); }
						if (isSelected) { ImGui::SetItemDefaultFocus(); }
					}

					ImGui::EndCombo();
				}

				ImGui::SameLine();
				if (ImGui::Button("New Style")) { NewStyle(); }
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted("Style Name");
				ImGui::TableSetColumnIndex(1);
				ImGui::InputText("###StyleManager_EditingName", EditingName, sizeof(EditingName));

				ImGui::EndTable();
			}

			ImGui::Spacing();

			if (ImGui::Button("Save Changes"))
			{
				std::string editingName = EditingName;

				if (SelectedName != editingName
					&& (Immediate.StyleMap.find(SelectedName) != Immediate.StyleMap.end())
					&& (Immediate.StyleMap.find(editingName) == Immediate.StyleMap.end()))
				{
					std::shared_ptr<ImStyle> style = Immediate.StyleMap[SelectedName];

					if (style && !style->IsHardcoded())
					{
						style->DeleteStyle();
						style->SetName(editingName);
						style->SetSettings(EditingStyle->GetSettings());
						style->SetColors(EditingStyle->GetColors());
						style->SaveStyle();
						style->ApplyStyle(true);
						SelectStyle(style);
					}
				}
				else if (Immediate.StyleMap.find(editingName) != Immediate.StyleMap.end())
				{
					std::shared_ptr<ImStyle> style = Immediate.StyleMap[editingName];

					if (style && !style->IsHardcoded())
					{
						style->SetSettings(EditingStyle->GetSettings());
						style->SetColors(EditingStyle->GetColors());
						style->SaveStyle();
						style->ApplyStyle(true);
						SelectStyle(style);
					}
				}
				else
				{
					EditingStyle->SetName(editingName);
					std::shared_ptr<ImStyle> style = Immediate.CreateStyle(new ImStyle(EditingStyle->GetCopy()));
					style->SaveStyle();
					style->ApplyStyle(true);
					SelectStyle(style);
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Delete Style"))
			{
				if (!SelectedStyle->IsHardcoded())
				{
					if (Immediate.StyleMap.find(SelectedName) != Immediate.StyleMap.end())
					{
						Initialize();
						Immediate.DeleteStyle(SelectedName);
						Initialize();
					}
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Reset to Default"))
			{
				SelectedStyle->ApplyStyle(false);
				EditingStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
				CompareStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
			}

			ImGui::Spacing();

			if (ImGui::BeginTabBar("Tabs###StyleManager_Tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Settings###StyleManager_Settings"))
				{
					if (ImGui::BeginChild("SettingRegion######StyleManager_SettingRegion", ImVec2(0.0f, -ImGui::GetStyle().ItemSpacing.y), false))
					{
						ImGui::TextUnformatted("Borders");
						if (ImGui::SliderFloat("WindowBorderSize", EditingStyle->GetSetting(ImSettingIds::IM_WindowBorderSize), 0.0f, 1.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("ChildBorderSize", EditingStyle->GetSetting(ImSettingIds::IM_ChildBorderSize), 0.0f, 1.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("PopupBorderSize", EditingStyle->GetSetting(ImSettingIds::IM_PopupBorderSize), 0.0f, 1.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("FrameBorderSize", EditingStyle->GetSetting(ImSettingIds::IM_FrameBorderSize), 0.0f, 1.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("TabBorderSize", EditingStyle->GetSetting(ImSettingIds::IM_TabBorderSize), 0.0f, 1.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }

						ImGui::TextUnformatted("Rounding");
						if (ImGui::SliderFloat("WindowRounding", EditingStyle->GetSetting(ImSettingIds::IM_WindowRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("ChildRounding", EditingStyle->GetSetting(ImSettingIds::IM_ChildRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("FrameRounding", EditingStyle->GetSetting(ImSettingIds::IM_FrameRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("PopupRounding", EditingStyle->GetSetting(ImSettingIds::IM_PopupRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("ScrollbarRounding", EditingStyle->GetSetting(ImSettingIds::IM_ScrollbarRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("GrabRounding", EditingStyle->GetSetting(ImSettingIds::IM_GrabRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("LogSliderDeadzone", EditingStyle->GetSetting(ImSettingIds::IM_LogSliderDeadzone), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("TabRounding", EditingStyle->GetSetting(ImSettingIds::IM_TabRounding), 0.0f, 12.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }

						ImGui::TextUnformatted("Main");

						ImVec2 windowPadding = { *EditingStyle->GetSetting(ImSettingIds::IM_WindowPadding_X), *EditingStyle->GetSetting(ImSettingIds::IM_WindowPadding_Y) };
						if (ImGui::SliderFloat2("WindowPadding", reinterpret_cast<float*>(&windowPadding), 0.0f, 20.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_WindowPadding_X, windowPadding.x);
							EditingStyle->SetSetting(ImSettingIds::IM_WindowPadding_Y, windowPadding.y);
							EditingStyle->ApplyStyle(false);
						}

						ImVec2 framePadding = { *EditingStyle->GetSetting(ImSettingIds::IM_FramePadding_X), *EditingStyle->GetSetting(ImSettingIds::IM_FramePadding_Y) };
						if (ImGui::SliderFloat2("FramePadding", reinterpret_cast<float*>(&framePadding), 0.0f, 20.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_FramePadding_X, framePadding.x);
							EditingStyle->SetSetting(ImSettingIds::IM_FramePadding_Y, framePadding.y);
							EditingStyle->ApplyStyle(false);
						}

						ImVec2 cellPadding = { *EditingStyle->GetSetting(ImSettingIds::IM_CellPadding_X), *EditingStyle->GetSetting(ImSettingIds::IM_CellPadding_Y) };
						if (ImGui::SliderFloat2("CellPadding", reinterpret_cast<float*>(&cellPadding), 0.0f, 20.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_CellPadding_X, cellPadding.x);
							EditingStyle->SetSetting(ImSettingIds::IM_CellPadding_Y, cellPadding.y);
							EditingStyle->ApplyStyle(false);
						}

						ImVec2 itemSpacing = { *EditingStyle->GetSetting(ImSettingIds::IM_ItemSpacing_X), *EditingStyle->GetSetting(ImSettingIds::IM_ItemSpacing_Y) };
						if (ImGui::SliderFloat2("ItemSpacing", reinterpret_cast<float*>(&itemSpacing), 0.0f, 20.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_ItemSpacing_X, itemSpacing.x);
							EditingStyle->SetSetting(ImSettingIds::IM_ItemSpacing_Y, itemSpacing.y);
							EditingStyle->ApplyStyle(false);
						}

						ImVec2 innerSpacing = { *EditingStyle->GetSetting(ImSettingIds::IM_InnerSpacing_X), *EditingStyle->GetSetting(ImSettingIds::IM_InnerSpacing_Y) };
						if (ImGui::SliderFloat2("InnerSpacing", reinterpret_cast<float*>(&innerSpacing), 0.0f, 20.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_InnerSpacing_X, innerSpacing.x);
							EditingStyle->SetSetting(ImSettingIds::IM_InnerSpacing_Y, innerSpacing.y);
							EditingStyle->ApplyStyle(false);
						}

						ImVec2 touchPadding = { *EditingStyle->GetSetting(ImSettingIds::IM_TouchPadding_X), *EditingStyle->GetSetting(ImSettingIds::IM_TouchPadding_Y) };
						if (ImGui::SliderFloat2("TouchPadding", reinterpret_cast<float*>(&touchPadding), 0.0f, 10.0f, "%.0f"))
						{
							EditingStyle->SetSetting(ImSettingIds::IM_TouchPadding_X, touchPadding.x);
							EditingStyle->SetSetting(ImSettingIds::IM_TouchPadding_Y, touchPadding.y);
							EditingStyle->ApplyStyle(false);
						}

						if (ImGui::SliderFloat("IndentSpacing", EditingStyle->GetSetting(ImSettingIds::IM_IndentSpacing), 0.0f, 30.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("ScrollbarSize", EditingStyle->GetSetting(ImSettingIds::IM_ScrollbarSize), 1.0f, 20.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }
						if (ImGui::SliderFloat("GrabMinSize", EditingStyle->GetSetting(ImSettingIds::IM_GrabMinSize), 1.0f, 20.0f, "%.0f")) { EditingStyle->ApplyStyle(false); }

						ImGui::EndChild();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Colors###StyleManager_Colors"))
				{
					if (ImGui::BeginChild("ColorRegion###StyleManager_ColorRegion", ImVec2(0.0f, -ImGui::GetStyle().ItemSpacing.y), false))
					{
						for (int32_t i = 0; i < static_cast<int32_t>(ImColorIds::IM_END); i++)
						{
							ImColorIds colorId = static_cast<ImColorIds>(i);
							float* editingColor = EditingStyle->GetColor(colorId);
							float* compareColor = CompareStyle->GetColor(colorId);

							ImGui::PushID(i);
							ImGui::ColorEdit4("##Color", editingColor, ImGuiColorEditFlags_AlphaBar);
							ImGui::PopID();

							if (memcmp(editingColor, compareColor, sizeof(ImVec4)) != 0)
							{
								CompareStyle->SetColor(colorId, *reinterpret_cast<ImVec4*>(compareColor));
								EditingStyle->ApplyStyle(false);
							}

							ImGui::SameLine();
							ImGui::TextUnformatted(ImColorNames[static_cast<ImColorIds>(i)].c_str());
						}

						ImGui::EndChild();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}

		ImGui::End();
	}
}

void ImStyleManager::Initialize()
{
	SelectedStyle = Immediate.SelectedStyle;
	SelectedName = SelectedStyle->GetName();

	memset(EditingName, 0, sizeof(EditingName));
	EditingStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	CompareStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	strcpy_s(EditingName, sizeof(EditingName), EditingStyle->GetName().c_str());

	if (SelectedStyle->IsHardcoded())
	{
		strcat_s(EditingName, sizeof(EditingName), " (Preview)");
	}
}

void ImStyleManager::SelectStyle(const std::shared_ptr<ImStyle>& style)
{
	//Manager.GetSetting(VariableIds::IMGUI_SELECTED_STYLE)->SetValue(style->GetName());

	SelectedStyle = style;
	SelectedName = SelectedStyle->GetName();
	SelectedStyle->ApplyStyle(true);

	memset(EditingName, 0, sizeof(EditingName));
	EditingStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	CompareStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	strcpy_s(EditingName, sizeof(EditingName), EditingStyle->GetName().c_str());

	if (style->IsHardcoded())
	{
		strcat_s(EditingName, sizeof(EditingName), " (Preview)");
	}
}

void ImStyleManager::NewStyle()
{
	SelectedStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	SelectedName = SelectedStyle->GetName() + "(Copy)";

	memset(EditingName, 0, sizeof(EditingName));
	EditingStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	CompareStyle = std::make_shared<ImStyle>(SelectedStyle->GetCopy());
	strcpy_s(EditingName, sizeof(EditingName), EditingStyle->GetName().c_str());
	strcat_s(EditingName, sizeof(EditingName), " (Copy)");
}

ImConsole::ImConsole(const std::string& title, const std::string& name, bool bShowCursor) : ImInterface(title, name, bShowCursor) {}

ImConsole::~ImConsole()
{
	OnDetatch();
}

void ImConsole::OnAttach()
{
	memset(InputBuffer, 0, sizeof(InputBuffer));
	MaxHistory = 256;
	HistoryPos = -1;
	InputFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit;
	CandidatePos = 0;
	ArgumentType = ImArgumentIds::IM_None;
	AutoScroll = true;
	ScrollToBottom = false;
	HomoMode = false;

	//UMapList_TA* mapList = Instances.IUMapList_TA();

	//if (mapList)
	//{
	//	std::vector<std::string> mapNames;

	//	for (UMapData_TA* mapData : mapList->AllMaps)
	//	{
	//		if (mapData)
	//		{
	//			std::string mapName = mapData->GetFullName();

	//			size_t pos1 = mapName.find("MapData_TA MapList.");

	//			if (pos1 != std::string::npos)
	//			{
	//				mapName = mapName.replace(pos1, 19, "");

	//				size_t pos2 = mapName.find("Maps.");

	//				if (pos2 != std::string::npos)
	//				{
	//					mapName = mapName.replace(pos1, 5, "");
	//				}
	//			}

	//			mapNames.push_back(mapName);
	//		}
	//	}

	//	SetMapCompletes(mapNames);
	//}

	for (const std::pair<std::string, MappableKeys>& keys : GKeyNames)
	{
		KeyCompletes.push_back(ImUtils::Strdup(keys.first.c_str()));
	}

	SetAttached(true);
}

void ImConsole::OnDetatch()
{
	if (IsAttached())
	{
		ClearAutoCompletes();
		ClearConsole();
		ClearHistory();
		ClearMapCompletes();
		ClearKeyCompletes();
		SetAttached(false);
	}
}

void ImConsole::OnRender()
{
	if (ShouldRender())
	{
		ImGui::SetNextWindowSize(ImVec2(765.0f, 455.0f), ImGuiCond_Once);

		if (ImGui::Begin(GetTitle().c_str(), ShouldBegin(), ImGuiWindowFlags_NoNav))
		{
			bool copy_to_clipboard = false;
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

			if (ImGui::BeginChild("ScrollingRegion###Console_ScrollRegion", ImVec2(0, -footer_height_to_reserve), false))
			{
				if (Immediate.CommandQueue.size() > 0)
				{
					for (const std::pair<std::string, TextColors>& command : Immediate.CommandQueue)
					{
						AddText(command.second, TextStyles::Regular, "%s", command.first.c_str());
					}

					Immediate.CommandQueue.clear();
				}

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::BeginMenu("Filter Text"))
					{
						Filter.Draw("Filter: (\"incl,-excl\") (\"error\")###Console_Filter", ImGui::GetWindowSize().x / 4);
						ImGui::EndMenu();
					}

					ImGui::MenuItem("Auto Scroll", "", &AutoScroll);
					copy_to_clipboard = ImGui::Selectable("Copy to Clipboard");
					if (ImGui::Selectable("Clear Console")) { ClearConsole(); }
					if (ImGui::Selectable("Clear History")) { ClearHistory(); }

					ImGui::EndPopup();
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

				if (copy_to_clipboard)
				{
					ImGui::LogToClipboard();
				}

				for (size_t i = 0; i < ConsoleText.size(); i++)
				{
					const TextData& textData = ConsoleText[i];
					const char* itemText = textData.Text;

					if (!Filter.PassFilter(itemText))
					{
						continue;
					}

					float fontSize = 1.0f;

					if (!HomoMode)
					{
						//if (ImmediateComponent::AreFontsLoaded())
						//{
							ImFont* font = ImFontMap[textData.Style];

							if (font)
							{
								fontSize = font->FontSize;
								ImGui::PushFont(font);
							}
						//}

						ImGui::PushTextWrapPos(ImGui::GetWindowWidth() - fontSize);
						ImGui::PushStyleColor(ImGuiCol_Text, textData.Color);
						ImGui::TextUnformatted(itemText);
						ImGui::PopStyleColor();

						//if (ImmediateComponent::AreFontsLoaded())
						//{
							ImGui::PopFont();
						//}

						ImGui::PopTextWrapPos();
					}
					else
					{
						// I'm gay, so you can't complain or cancel me for my variable name choices.
						static const int32_t queerColors[8] = { 3, 4, 5, 6, 7, 8, 9, 10 };
						static int32_t gayIndex = 0;
						static int32_t fontIndex = 0;

						char itemArray[2];
						size_t itemLength = std::strlen(itemText);

						for (size_t j = 0; j < itemLength; j++)
						{
							memset(itemArray, 0, sizeof(itemArray));
							strncpy_s(itemArray, itemText + j, 1);

							//if (ImmediateComponent::AreFontsLoaded())
							//{
								ImGui::PushFont(ImFontMap[textData.Style]);
							//}

							ImGui::PushStyleColor(ImGuiCol_Text, ImColorMap[static_cast<TextColors>(queerColors[gayIndex])]);
							ImGui::SameLine(0.0f, 0.0f);
							ImGui::TextUnformatted(itemArray);
							ImGui::PopStyleColor();

							gayIndex = (gayIndex < 7) ? (gayIndex + 1) : 0;
							fontIndex = (fontIndex < 3) ? (fontIndex + 1) : 0;
						}

						gayIndex = fontIndex = 0;
						ImGui::TextUnformatted("\n");

						//if (ImmediateComponent::AreFontsLoaded())
						//{
							ImGui::PopFont();
						//}
					}
				}

				if (copy_to_clipboard)
				{
					ImGui::LogFinish();
				}

				if (ScrollToBottom || (AutoScroll && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())))
				{
					ImGui::SetScrollHereY(1.0f);
				}

				ScrollToBottom = false;

				ImGui::PopStyleVar();
				ImGui::EndChild();

				bool reclaim_focus = false;

				ImGui::PushItemWidth((ImGui::GetWindowSize().x - (ImGui::GetStyle().WindowPadding.x * 2)) - 55);

				if (ImGui::InputText("###ConsoleInputText", InputBuffer, IM_ARRAYSIZE(InputBuffer), InputFlags, [](ImGuiInputTextCallbackData* data) -> int32_t { return reinterpret_cast<ImConsole*>(data->UserData)->TextEditCallback(data); }, reinterpret_cast<void*>(this)))
				{
					char* bufferText = InputBuffer;
					ImUtils::Strtrim(bufferText);

					if (bufferText[0])
					{
						ExecuteCommand(bufferText, TextColors::White, TextStyles::Regular);
					}

					strcpy_s(bufferText, sizeof(bufferText), "");
					reclaim_focus = true;
				}

				ImGui::SameLine();
				if (ImGui::Button("Enter"))
				{
					char* bufferText = InputBuffer;
					ImUtils::Strtrim(bufferText);

					if (bufferText[0])
					{
						ExecuteCommand(bufferText, TextColors::White, TextStyles::Regular);
					}

					strcpy_s(bufferText, sizeof(bufferText), "");
					reclaim_focus = true;
				}

				if (!Candidates.empty() && ImGui::IsWindowFocused())
				{
					ImVec2 consolePos = ImGui::GetWindowPos();
					consolePos.y += ImGui::GetWindowHeight() + 5.0f;

					if (ArgumentType == ImArgumentIds::IM_Maps)
					{
						consolePos.x += 135.0f;
					}
					else if (ArgumentType == ImArgumentIds::IM_Keys)
					{
						consolePos.x += 100.0f;
					}

					ImGui::SetNextWindowPos(consolePos);
					ImGui::OpenPopup("###Console_CandidatesPopup");

					if (ImGui::BeginPopup("###Console_CandidatesPopup", ImGuiWindowFlags_NoFocusOnAppearing))
					{
						ImVec2 childSize(175.0f, 0.0f);

						for (size_t i = 0; i < Candidates.size(); i++)
						{
							childSize.y += 18.25f;

							if (childSize.y > 130.0f)
							{
								childSize.y = 130.0f;
								break;
							}
						}

						if (ImGui::BeginChild("###Console_CandidatesChild", childSize))
						{
							for (size_t i = 0; i < Candidates.size(); i++)
							{
								const std::pair<const char*, bool>& candidate = Candidates[i];

								if (candidate.second)
								{
									ImGui::PushStyleColor(ImGuiCol_Text, ImColorMap[TextColors::White]);
								}
								else
								{
									//if (ImmediateComponent::AreFontsLoaded())
									//{
										ImGui::PushFont(ImFontMap[TextStyles::Italic]);
									//}

									ImGui::PushStyleColor(ImGuiCol_Text, ImColorMap[TextColors::Grey]);
								}

								ImGui::TextUnformatted(Candidates[i].first);
								ImGui::PopStyleColor();

								//if (!candidate.second && ImmediateComponent::AreFontsLoaded())
								//{
									ImGui::PopFont();
								//}
							}

							ImGui::EndChild();
						}

						ImGui::EndPopup();
					}
				}

				ImGui::PopItemWidth();
				ImGui::SetItemDefaultFocus();

				if (reclaim_focus)
				{
					ImGui::SetKeyboardFocusHere(-1);
				}
			}
		}

		ImGui::End();
	}
}

void ImConsole::ToggleHomosexual()
{
	HomoMode = !HomoMode;
}

void ImConsole::ClearHistory()
{
	for (size_t i = 0; i < UserHistory.size(); i++)
	{
		if (UserHistory[i])
		{
			free(UserHistory[i]);
		}
	}

	UserHistory.clear();
}

void ImConsole::ClearMapCompletes()
{
	for (size_t i = 0; i < MapCompletes.size(); i++)
	{
		if (MapCompletes[i])
		{
			free(MapCompletes[i]);
		}
	}

	MapCompletes.clear();
}

void ImConsole::ClearKeyCompletes()
{
	for (size_t i = 0; i < KeyCompletes.size(); i++)
	{
		if (KeyCompletes[i])
		{
			free(KeyCompletes[i]);
		}
	}

	KeyCompletes.clear();
}

void ImConsole::ClearAutoCompletes()
{
	for (size_t i = 0; i < AutoCompletes.size(); i++)
	{
		if (AutoCompletes[i])
		{
			free(AutoCompletes[i]);
		}
	}

	AutoCompletes.clear();
}

void ImConsole::ClearConsole()
{
	for (size_t i = 0; i < ConsoleText.size(); i++)
	{
		ConsoleText[i].FreeData();
	}

	ConsoleText.clear();
}

void ImConsole::SetHistorySize(size_t newSize)
{
	if (newSize <= 5120)
	{
		MaxHistory = newSize;
	}
}

void ImConsole::AddAutoComplete(const std::string& command)
{
	AutoCompletes.push_back(ImUtils::Strdup(command.c_str()));
}

void ImConsole::SetMapCompletes(const std::vector<std::string>& maps)
{
	ClearMapCompletes();

	for (const std::string& map : maps)
	{
		MapCompletes.push_back(ImUtils::Strdup(map.c_str()));
	}
}

void ImConsole::ResetAutoComplete()
{
	CandidatePos = 0;
	ArgumentType = ImArgumentIds::IM_None;
	Candidates.clear();
}

void ImConsole::AddText(TextColors color, TextStyles style, const char* fmt, ...)
{
	char buffer[2048];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, IM_ARRAYSIZE(buffer), fmt, args);
	buffer[IM_ARRAYSIZE(buffer) - 1] = 0;
	va_end(args);

	TextData newData(ImColorMap[color], style, ImUtils::Strdup(buffer));

	if (ConsoleText.size() >= MaxHistory)
	{
		for (size_t i = 0; i < ConsoleText.size(); i++)
		{
			if (i == 0)
			{
				ConsoleText[i].FreeData();
				continue;
			}

			ConsoleText[i - 1] = ConsoleText[i];
		}

		ConsoleText[ConsoleText.size() - 1] = newData;
	}
	else
	{
		ConsoleText.push_back(newData);
	}
}

void ImConsole::ExecuteCommand(const char* commandText, TextColors color, TextStyles style)
{
	ResetAutoComplete();
	AddText(TextColors::User, style, "# %s", commandText);

	HistoryPos = -1;

	if (UserHistory.size() >= 128)
	{
		for (size_t i = 0; i < UserHistory.size(); i++)
		{
			if (i == 0)
			{
				free(UserHistory[i]);
				continue;
			}

			UserHistory[i - 1] = UserHistory[i];
		}

		UserHistory[ConsoleText.size() - 1] = ImUtils::Strdup(commandText);
	}
	else
	{
		UserHistory.push_back(ImUtils::Strdup(commandText));
	}

	std::string commandLine = commandText;
	size_t spacePos = commandLine.find(" ");
	std::string command = commandLine;
	std::string parameter;

	if (spacePos != std::string::npos)
	{
		command = commandLine.substr(0, commandLine.find(" "));
		parameter = commandLine.substr(commandLine.find(" "));

		if (parameter.length() > 1)
		{
			parameter.erase(0, 1);
		}
	}

	Manager.QueueCommand(command, parameter);
	ScrollToBottom = true;
}

void ImConsole::CommandDelegate(const std::string& text, TextColors color, TextStyles style)
{
	AddText(color, style, "%s", text.c_str());
}

int32_t ImConsole::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackHistory:
	{
		if (Candidates.empty())
		{
			int32_t prev_history_pos = HistoryPos;

			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (HistoryPos == -1)
				{
					HistoryPos = UserHistory.size() - 1;
				}
				else if (HistoryPos > 0)
				{
					HistoryPos--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (HistoryPos != -1)
				{
					if (++HistoryPos >= UserHistory.size())
					{
						HistoryPos = -1;
					}
				}
			}

			if (prev_history_pos != HistoryPos)
			{
				const char* history_str = (HistoryPos >= 0) ? UserHistory[HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		else
		{
			int32_t prev_can_pos = CandidatePos;

			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (CandidatePos == 0)
				{
					CandidatePos = Candidates.size() - 1;
				}
				else if (CandidatePos > 0)
				{
					CandidatePos--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (CandidatePos == Candidates.size() - 1)
				{
					CandidatePos = 0;
				}
				else if (CandidatePos < Candidates.size())
				{
					CandidatePos++;
				}
			}

			for (auto& candidate : Candidates)
			{
				candidate.second = false;
			}

			Candidates[CandidatePos].second = true;
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		if (!Candidates.empty())
		{
			int32_t completePos = CandidatePos;

			if (completePos < 0 || completePos > Candidates.size())
			{
				completePos = 0;
			}

			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;

			while (word_start > data->Buf)
			{
				const char c = word_start[-1];

				if (c == ' ' || c == '\t' || c == ',' || c == ';')
				{
					break;
				}

				word_start--;
			}

			data->DeleteChars((int32_t)(word_start - data->Buf), (int32_t)(word_end - word_start));
			data->InsertChars(data->CursorPos, Candidates[completePos].first);
			data->InsertChars(data->CursorPos, " ");
			ResetAutoComplete();
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackEdit:
	{
		if (strnlen_s(data->Buf, sizeof(InputBuffer)) > 0)
		{
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			bool isArgument = false;

			while (word_start > data->Buf)
			{
				const char c = word_start[-1];

				if (c == ' ')
				{
					isArgument = true;
					break;
				}
				else if (c == '\t' || c == ',' || c == ';')
				{
					break;
				}

				word_start--;
			}

			ResetAutoComplete();

			if (strnlen_s(word_start, sizeof(InputBuffer)) > 0 || isArgument)
			{
				if (!isArgument)
				{
					bool first = true;

					for (size_t i = 0; i < AutoCompletes.size(); i++)
					{
						if (ImUtils::Strnicmp(AutoCompletes[i], word_start, (int32_t)(word_end - word_start)) == 0)
						{
							Candidates.push_back(std::make_pair(AutoCompletes[i], first));
							first = false;
						}
					}
				}
				else
				{
					const char* argument_end = data->Buf + data->CursorPos;
					const char* argument_start = argument_end;

					while (argument_start > data->Buf)
					{
						if (argument_start[-1] == ' ')
						{
							break;
						}

						argument_start--;
					}

					std::string inputStr = data->Buf;
					int32_t foundSpaces = 0;

					for (const char& c : inputStr)
					{
						if (c == ' ')
						{
							foundSpaces++;
						}

						if (foundSpaces > 1)
						{
							return 0;
						}
					}

					if (inputStr.find("training_open_freeplay") == 0)
					{
						ArgumentType = ImArgumentIds::IM_Maps;
					}
					else if (inputStr.find("keybind_create") == 0)
					{
						ArgumentType = ImArgumentIds::IM_Keys;
					}
					else
					{
						ArgumentType = ImArgumentIds::IM_None;
						break;
					}

					if (ArgumentType == ImArgumentIds::IM_Maps)
					{
						bool first = true;

						for (size_t i = 0; i < MapCompletes.size(); i++)
						{
							if (ImUtils::Strnicmp(MapCompletes[i], argument_start, (int32_t)(argument_end - argument_start)) == 0)
							{
								Candidates.push_back(std::make_pair(MapCompletes[i], first));
								first = false;
							}
						}
					}
					else if (ArgumentType == ImArgumentIds::IM_Keys)
					{
						bool first = true;

						for (size_t i = 0; i < KeyCompletes.size(); i++)
						{
							if (ImUtils::Strnicmp(KeyCompletes[i], argument_start, (int32_t)(argument_end - argument_start)) == 0)
							{
								Candidates.push_back(std::make_pair(KeyCompletes[i], first));
								first = false;
							}
						}
					}
				}
			}
		}
		else
		{
			ResetAutoComplete();
		}

		break;
	}
	}

	return 0;
}

ImFunctionScanner::ImFunctionScanner(const std::string& title, const std::string& name, bool bShowCursor) : ImInterface(title, name, bShowCursor) {}

ImFunctionScanner::~ImFunctionScanner()
{
	OnDetatch();
}

void ImFunctionScanner::OnAttach()
{
	TableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY;
	ScanFunctions = false;
	SetAttached(true);
}

void ImFunctionScanner::OnDetatch()
{
	if (IsAttached())
	{
		for (FunctionData& data : FunctionHistory)
		{
			data.FreeData();
		}

		SetAttached(false);
	}
}

void ImFunctionScanner::OnRender()
{
	if (ShouldRender())
	{
		char title[128];
		sprintf_s(title, GetTitle().c_str());
		sprintf_s(title + strlen(title), sizeof(title), " - %d Functions###FunctionScanner", FunctionHistory.size());

		ImGui::SetNextWindowSize(ImVec2(500.0f, 625.0f), ImGuiCond_Once);

		if (ImGui::Begin(title, ShouldBegin()))
		{
			if (ImGui::BeginTable("FilterTable###Scanner_Filter", 2))
			{
				ImGui::TableSetupColumn("Text", ImGuiTableColumnFlags_WidthFixed, 50.f);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Whitelist");
				ImGui::TableSetColumnIndex(1);
				Whitelist.Draw(" Filter: (\"incl\") (\"error\")###Scanner_Whitelist");

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Blacklist");
				ImGui::TableSetColumnIndex(1);
				Blacklist.Draw(" Filter: (\"excl\") (\"error\")###Scanner_Blacklist");

				ImGui::EndTable();
			}

			ImGui::Spacing();

			if (IsScanning()) { if (ImGui::Button("Stop Monitoring")) { ScanFunctions = false; } }
			else { if (ImGui::Button("Start Monitoring")) { ScanFunctions = true; } }

			ImGui::SameLine();
			if (ImGui::Button("Clear Table")) { FunctionHistory.clear(); }
			ImGui::SameLine();
			if (ImGui::Button("Save to File")) { SaveToFile(); }

			ImGui::Spacing();

			if (ImGui::BeginTable("FunctionTable", 3, TableFlags))
			{
				bool copy_to_clipboard = false;

				if (ImGui::BeginPopupContextWindow())
				{
					copy_to_clipboard = ImGui::Selectable("Copy to Clipboard"));
					ImGui::EndPopup();
				}

				ImGui::TableSetupColumn("Package Object", ImGuiTableColumnFlags_WidthFixed, 100.0f);
				ImGui::TableSetupColumn("Calling Class", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Function Name", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				for (size_t i = 0; i < FunctionHistory.size(); i++)
				{
					const FunctionData& data = FunctionHistory[i];

					if (PassesFilter(data.Full))
					{
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(data.Package);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(data.Caller);
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(data.Function);

						if (copy_to_clipboard)
						{
							ClipboardText += data.Full;
							ClipboardText += "\n";
						}
					}
				}

				if (copy_to_clipboard && ClipboardText.length() > 0)
				{
					ImGui::SetClipboardText(ClipboardText.c_str());
					ClipboardText.clear();
				}

				ImGui::EndTable();
			}
		}
		else
		{
			ScanFunctions = false;
		}

		ImGui::End();
	}
	else
	{
		ScanFunctions = false;
	}
}

bool ImFunctionScanner::IsScanning()
{
	return ScanFunctions;
}

void ImFunctionScanner::SaveToFile()
{
	if (FunctionHistory.size() > 0)
	{
		std::ofstream functionTable;// (FileManager.GetDumpFolder() / ("FunctionDump_" + std::to_string(std::time(nullptr)) + ".txt"));

		size_t totalFunctions = 0;

		for (const FunctionData& functionData : FunctionHistory)
		{
			if (functionData.Full)
			{
				functionTable << functionData.Full << std::endl;
				totalFunctions++;
			}
		}

		functionTable.close();
		//Console.Success("[ImFunctionScanner] (SaveToFile) Successfully dumped " + std::to_string(totalFunctions) + " function(s)!");
	}
}

bool ImFunctionScanner::PassesFilter(const char* textToFilter)
{
	if (Whitelist.IsActive())
	{
		if (Whitelist.PassFilter(textToFilter))
		{
			if (Blacklist.IsActive())
			{
				return !Blacklist.PassFilter(textToFilter);
			}

			return true;
		}

		return false;
	}

	if (Blacklist.IsActive())
	{
		if (!Blacklist.PassFilter(textToFilter))
		{
			if (Whitelist.IsActive())
			{
				return Whitelist.PassFilter(textToFilter);
			}

			return true;
		}

		return false;
	}

	return true;
}

void ImFunctionScanner::OnProcessEvent(class UObject* caller, class UFunction* function)
{
	if (ShouldRender() && IsScanning() && caller && function)
	{
		std::string fullName = function->GetFullName();

		if (PassesFilter(fullName.c_str()))
		{
			FunctionHistory.push_back(FunctionData(
				ImUtils::Strdup(fullName.c_str()),
				ImUtils::Strdup(function->GetPackageObj()->GetName().c_str()),
				ImUtils::Strdup(caller->GetName().c_str()),
				ImUtils::Strdup(function->GetName().c_str())
			));
		}
	}
}