#pragma once

// https://niemand.com.ar/2019/01/17/improving-your-cheat-template-hooking-directx11/

namespace ImUtils
{
	int32_t Stricmp(const char* s1, const char* s2);
	int32_t Strnicmp(const char* s1, const char* s2, int32_t n);
	char* Strdup(const char* s);
	void Strtrim(char* s);
}

// Rounding
static constexpr float DEFAULT_WINDOW_ROUNDING = 5.0f;
static constexpr float DEFAULT_CHILD_ROUNDING = 5.0f;
static constexpr float DEFAULT_FRAME_ROUNDING = 5.0f;
static constexpr float DEFAULT_POPUP_ROUNDING = 5.0f;
static constexpr float DEFAULT_SCROLLBAR_ROUNDING = 3.0f;
static constexpr float DEFAULT_GRAB_ROUNDING = 4.0f;
static constexpr float DEFAULT_LOGSLIDER_DEADZONE = 5.0f;
static constexpr float DEFAULT_TAB_ROUNDING = 5.0f;
// Borders
static constexpr float DEFAULT_WINDOW_BORDER_SIZE = 0.0f;
static constexpr float DEFAULT_CHILD_BORDER_SIZE = 0.0f;
static constexpr float DEFAULT_POPUP_BORDER_SIZE = 0.0f;
static constexpr float DEFAULT_FRAME_BORDER_SIZE = 0.0f;
static constexpr float DEFAULT_TAB_BORDER_SIZE = 0.0f;
// Main
static constexpr float DEFAULT_WINDOW_PADDING_X = 10.0f;
static constexpr float DEFAULT_WINDOW_PADDING_Y = 10.0f;
static constexpr float DEFAULT_FRAME_PADDING_X = 8.0f;
static constexpr float DEFAULT_FRAME_PADDING_Y = 4.0f;
static constexpr float DEFAULT_CELL_PADDING_X = 5.0f;
static constexpr float DEFAULT_CELL_PADDING_Y = 3.0f;
static constexpr float DEFAULT_ITEM_SPACING_X = 8.0f;
static constexpr float DEFAULT_ITEM_SPACING_Y = 5.0f;
static constexpr float DEFAULT_INNER_SPACING_X = 4.0f;
static constexpr float DEFAULT_INNER_SPACING_Y = 4.0f;
static constexpr float DEFAULT_TOUCH_PADDING_X = 0.0f;
static constexpr float DEFAULT_TOUCH_PADDING_Y = 0.0f;
static constexpr float DEFAULT_INDENT_SPACING = 25.0f;
static constexpr float DEFAULT_SCROLLBAR_SIZE = 15.0f;
static constexpr float DEFAULT_GRAB_MINI_SIZE = 10.0f;

// These text style and color enums are from another component/file in CodeRed, just including them here to show what they are.
enum class TextStyles : uint8_t
{
	Regular = 0,
	Italic = 1,
	Bold = 2,
	END = 3
};

enum class TextColors : int32_t
{
	Black = 0,
	Grey = 1,
	White = 2,
	Red = 3,
	Orange = 4,
	Yellow = 5,
	Green = 6,
	Blue = 7,
	Aqua = 8,
	Purple = 9,
	Pink = 10,
	User = 11,
	END = 12
};

enum class ImSettingIds : int32_t
{
	IM_WindowPadding_X,
	IM_WindowPadding_Y,
	IM_FramePadding_X,
	IM_FramePadding_Y,
	IM_CellPadding_X,
	IM_CellPadding_Y,
	IM_ItemSpacing_X,
	IM_ItemSpacing_Y,
	IM_InnerSpacing_X,
	IM_InnerSpacing_Y,
	IM_TouchPadding_X,
	IM_TouchPadding_Y,
	IM_IndentSpacing,
	IM_ScrollbarSize,
	IM_GrabMinSize,
	IM_WindowBorderSize,
	IM_ChildBorderSize,
	IM_PopupBorderSize,
	IM_FrameBorderSize,
	IM_TabBorderSize,
	IM_WindowRounding,
	IM_ChildRounding,
	IM_FrameRounding,
	IM_PopupRounding,
	IM_ScrollbarRounding,
	IM_GrabRounding,
	IM_LogSliderDeadzone,
	IM_TabRounding,
	IM_END
};

enum class ImColorIds : int32_t
{
	IM_Text,
	IM_TextDisabled,
	IM_WindowBg,
	IM_ChildBg,
	IM_PopupBg,
	IM_Border,
	IM_BorderShadow,
	IM_FrameBg,
	IM_FrameBgHovered,
	IM_FrameBgActive,
	IM_TitleBg,
	IM_TitleBgActive,
	IM_TitleBgCollapsed,
	IM_MenuBarBg,
	IM_ScrollbarBg,
	IM_ScrollbarGrab,
	IM_ScrollbarGrabHovered,
	IM_ScrollbarGrabActive,
	IM_CheckMark,
	IM_SliderGrab,
	IM_SliderGrabActive,
	IM_Button,
	IM_ButtonHovered,
	IM_ButtonActive,
	IM_Header,
	IM_HeaderHovered,
	IM_HeaderActive,
	IM_Separator,
	IM_SeparatorHovered,
	IM_SeparatorActive,
	IM_ResizeGrip,
	IM_ResizeGripHovered,
	IM_ResizeGripActive,
	IM_Tab,
	IM_TabHovered,
	IM_TabActive,
	IM_TabUnfocused,
	IM_TabUnfocusedActive,
	IM_PlotLines,
	IM_PlotLinesHovered,
	IM_PlotHistogram,
	IM_PlotHistogramHovered,
	IM_TableHeaderBg,
	IM_TableBorderStrong,
	IM_TableBorderLight,
	IM_TableRowBg,
	IM_TableRowBgAlt,
	IM_TextSelectedBg,
	IM_DragDropTarget,
	IM_NavHighlight,
	IM_NavWindowingHighlight,
	IM_NavWindowingDimBg,
	IM_ModalWindowDimBg,
	IM_END
};

enum class ImArgumentIds : uint8_t
{
	IM_None,
	IM_Maps,
	IM_Keys
};

static std::map<ImSettingIds, std::string> ImSettingNames =
{
	{ ImSettingIds::IM_WindowRounding, "WindowRounding" },
	{ ImSettingIds::IM_ChildRounding, "ChildRounding" },
	{ ImSettingIds::IM_FrameRounding, "FrameRounding" },
	{ ImSettingIds::IM_PopupRounding, "PopupRounding" },
	{ ImSettingIds::IM_ScrollbarRounding, "ScrollbarRounding" },
	{ ImSettingIds::IM_GrabRounding, "GrabRounding" },
	{ ImSettingIds::IM_LogSliderDeadzone, "LogSliderDeadzone" },
	{ ImSettingIds::IM_TabRounding, "TabRounding" },
	{ ImSettingIds::IM_WindowBorderSize, "WindowBorderSize" },
	{ ImSettingIds::IM_ChildBorderSize, "ChildBorderSize" },
	{ ImSettingIds::IM_PopupBorderSize, "PopupBorderSize" },
	{ ImSettingIds::IM_FrameBorderSize, "FrameBorderSize" },
	{ ImSettingIds::IM_TabBorderSize, "TabBorderSize" },
	{ ImSettingIds::IM_WindowPadding_X, "WindowPaddingX" },
	{ ImSettingIds::IM_WindowPadding_Y, "WindowPaddingY" },
	{ ImSettingIds::IM_FramePadding_X, "FramePaddingX" },
	{ ImSettingIds::IM_FramePadding_Y, "FramePaddingY" },
	{ ImSettingIds::IM_CellPadding_X, "CellPaddingX" },
	{ ImSettingIds::IM_CellPadding_Y, "CellPaddingY" },
	{ ImSettingIds::IM_ItemSpacing_X, "ItemSpacingX" },
	{ ImSettingIds::IM_ItemSpacing_Y, "ItemSpacingY" },
	{ ImSettingIds::IM_InnerSpacing_X, "InnerSpacingX" },
	{ ImSettingIds::IM_InnerSpacing_Y, "InnerSpacingY" },
	{ ImSettingIds::IM_TouchPadding_X, "TouchPaddingX" },
	{ ImSettingIds::IM_TouchPadding_Y, "TouchPaddingY" },
	{ ImSettingIds::IM_IndentSpacing, "IndentSpacing" },
	{ ImSettingIds::IM_ScrollbarSize, "ScrollbarSize" },
	{ ImSettingIds::IM_GrabMinSize, "GrabMinSize" }
};

static std::map<ImColorIds, std::string> ImColorNames =
{
	{ ImColorIds::IM_Text, XSTR("Text") },
	{ ImColorIds::IM_TextDisabled, XSTR("TextDisabled") },
	{ ImColorIds::IM_WindowBg, XSTR("WindowBg") },
	{ ImColorIds::IM_ChildBg, XSTR("ChildBg") },
	{ ImColorIds::IM_PopupBg, XSTR("PopupBg") },
	{ ImColorIds::IM_Border, XSTR("Border") },
	{ ImColorIds::IM_BorderShadow, XSTR("BorderShadow") },
	{ ImColorIds::IM_FrameBg, XSTR("FrameBg") },
	{ ImColorIds::IM_FrameBgHovered, XSTR("FrameBgHovered") },
	{ ImColorIds::IM_FrameBgActive, XSTR("FrameBgActive") },
	{ ImColorIds::IM_TitleBg, XSTR("TitleBg") },
	{ ImColorIds::IM_TitleBgActive, XSTR("TitleBgActive") },
	{ ImColorIds::IM_TitleBgCollapsed, XSTR("TitleBgCollapsed") },
	{ ImColorIds::IM_MenuBarBg, XSTR("MenuBarBg") },
	{ ImColorIds::IM_ScrollbarBg, XSTR("ScrollbarBg") },
	{ ImColorIds::IM_ScrollbarGrab, XSTR("ScrollbarGrab") },
	{ ImColorIds::IM_ScrollbarGrabHovered, XSTR("ScrollbarGrabHovered") },
	{ ImColorIds::IM_ScrollbarGrabActive, XSTR("ScrollbarGrabActive") },
	{ ImColorIds::IM_CheckMark, XSTR("CheckMark") },
	{ ImColorIds::IM_SliderGrab, XSTR("SliderGrab") },
	{ ImColorIds::IM_SliderGrabActive, XSTR("SliderGrabActive") },
	{ ImColorIds::IM_Button, XSTR("Button") },
	{ ImColorIds::IM_ButtonHovered, XSTR("ButtonHovered") },
	{ ImColorIds::IM_ButtonActive, XSTR("ButtonActive") },
	{ ImColorIds::IM_Header, XSTR("Header") },
	{ ImColorIds::IM_HeaderHovered, XSTR("HeaderHovered") },
	{ ImColorIds::IM_HeaderActive, XSTR("HeaderActive") },
	{ ImColorIds::IM_Separator, XSTR("Separator") },
	{ ImColorIds::IM_SeparatorHovered, XSTR("SeparatorHovered") },
	{ ImColorIds::IM_SeparatorActive, XSTR("SeparatorActive") },
	{ ImColorIds::IM_ResizeGrip, XSTR("ResizeGrip") },
	{ ImColorIds::IM_ResizeGripHovered, XSTR("ResizeGripHovered") },
	{ ImColorIds::IM_ResizeGripActive, XSTR("ResizeGripActive") },
	{ ImColorIds::IM_Tab, XSTR("Tab") },
	{ ImColorIds::IM_TabHovered, XSTR("TabHovered") },
	{ ImColorIds::IM_TabActive, XSTR("TabActive") },
	{ ImColorIds::IM_TabUnfocused, XSTR("TabUnfocused") },
	{ ImColorIds::IM_TabUnfocusedActive, XSTR("TabUnfocusedActive") },
	{ ImColorIds::IM_PlotLines, XSTR("PlotLines") },
	{ ImColorIds::IM_PlotLinesHovered, XSTR("PlotLinesHovered") },
	{ ImColorIds::IM_PlotHistogram, XSTR("PlotHistogram") },
	{ ImColorIds::IM_PlotHistogramHovered, XSTR("PlotHistogramHovered") },
	{ ImColorIds::IM_TableHeaderBg, XSTR("TableHeaderBg") },
	{ ImColorIds::IM_TableBorderStrong, XSTR("TableBorderStrong") },
	{ ImColorIds::IM_TableBorderLight, XSTR("TableBorderLight") },
	{ ImColorIds::IM_TableRowBg, XSTR("TableRowBg") },
	{ ImColorIds::IM_TableRowBgAlt, XSTR("TableRowBgAlt") },
	{ ImColorIds::IM_TextSelectedBg, XSTR("TextSelectedBg") },
	{ ImColorIds::IM_DragDropTarget, XSTR("DragDropTarget") },
	{ ImColorIds::IM_NavHighlight, XSTR("NavHighlight") },
	{ ImColorIds::IM_NavWindowingHighlight, XSTR("NavWindowingHighlight") },
	{ ImColorIds::IM_NavWindowingDimBg, XSTR("NavWindowingDimBg") },
	{ ImColorIds::IM_ModalWindowDimBg, XSTR("ModalWindowDimBg") }
};

static std::map<TextColors, ImVec4> ImColorMap =
{
	{ TextColors::Black, ImVec4(0.0f, 0.0f, 0.0f, 1.0f) },					// #000000
	{ TextColors::Grey, ImVec4(0.72549f, 0.72549f, 0.72549f, 1.0f) },		// #B9B9B9
	{ TextColors::White, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) },					// #FFFFFF
	{ TextColors::Red, ImVec4(0.95686f, 0.10196f, 0.18431f, 1.0f) },		// #F41A2F
	{ TextColors::Orange, ImVec4(1.0f, 0.55686f, 0.0f, 1.0f) },				// #FF8E00
	{ TextColors::Yellow, ImVec4(0.99215f, 0.96078f, 0.0f, 1.0f) },			// #FDF500
	{ TextColors::Green, ImVec4(0.16470f, 0.99215f, 0.18039f, 1.0f) },		// #2AFD2E
	{ TextColors::Blue, ImVec4(0.12549f, 0.41568f, 1.0f, 1.0f) },			// #206AFF
	{ TextColors::Aqua, ImVec4(0.09019f, 0.90980f, 1.0f, 1.0f) },			// #17E8FF
	{ TextColors::Purple, ImVec4(0.64705f, 0.12941f, 1.0f, 1.0f) },			// #A521FF
	{ TextColors::Pink, ImVec4(1.0f, 0.12941, 1.0f, 1.0f) },				// #FF21FF
	{ TextColors::User, ImVec4(0.98823f, 0.97254f, 0.49411f, 1.0f) }		// #FCf87E
};

static std::map<TextStyles, ImFont*> ImFontMap =
{
	{ TextStyles::Regular, nullptr },
	{ TextStyles::Italic, nullptr },
	{ TextStyles::Bold, nullptr }
};

// Storage class for ImGui style settings and colors.
class ImStyle
{
private:
	bool HardcodedStyle;
	std::string StyleName;
	std::map<ImSettingIds, float> Settings;
	std::map<ImColorIds, ImVec4> Colors;

public:
	ImStyle();
	ImStyle(const ImStyle& imStyle);
	explicit ImStyle(const std::string& styleName, bool bHardcoded);
	explicit ImStyle(const std::filesystem::path& styleFile, bool bHardcoded);
	~ImStyle();

private:
	void Initialize();

public:
	ImStyle GetCopy() const;
	bool IsHardcoded() const;
	bool IsInitialized() const;
	const std::string& GetName() const;
	void SetName(const std::string& newName);

public:
	float* GetSetting(ImSettingIds settingId);
	float* GetColor(ImColorIds colorId);
	std::map<ImSettingIds, float> GetSettings() const;
	std::map<ImColorIds, ImVec4> GetColors() const;
	void SetSetting(ImSettingIds settingId, float newValue);
	void SetSettings(const std::map<ImSettingIds, float>& newSettings);
	void SetColor(ImColorIds colorId, const ImVec4& newColor);
	void SetColors(const std::map<ImColorIds, ImVec4>& newColors);
	void ApplyStyle(bool bSelect);
	void SaveStyle();
	void DeleteStyle();
	void ParseFile(const std::filesystem::path& styleFile);
};

// Base class for all ImGui interfaces, controls attaching/detaching as well as rendering.
class ImInterface
{
private:
	std::string WindowTitle;
	std::string WindowName;
	bool ShowCursor;
	bool Attached;
	bool Render;

public:
	ImInterface(const std::string& title, const std::string& name, bool bShowCursor = true);
	virtual ~ImInterface();

public:
	const std::string& GetTitle() const;
	const std::string& GetName() const;
	bool ShouldShowCursor() const;
	bool IsAttached() const;
	bool ShouldRender() const;
	bool* ShouldBegin();
	void SetAttached(bool bAttached);
	void SetShouldRender(bool bShouldRender);
	void ToggleRender();

public:
	virtual void OnAttach();
	virtual void OnDetatch();
	virtual void OnRender();
};

class ImDemo : public ImInterface
{
public:
	ImDemo(const std::string& title, const std::string& name, bool bShowCursor = true);
	~ImDemo() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;
};

class ImStyleManager : public ImInterface
{
private:
	std::string SelectedName;
	std::shared_ptr<ImStyle> SelectedStyle;

private:
	char EditingName[128];
	std::shared_ptr<ImStyle> EditingStyle;
	std::shared_ptr<ImStyle> CompareStyle;

public:
	ImStyleManager(const std::string& title, const std::string& name, bool bShowCursor = true);
	~ImStyleManager() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;
	void Initialize();
	void SelectStyle(const std::shared_ptr<ImStyle>& style);
	void NewStyle();
};

class ImConsole : public ImInterface
{
private:
	class TextData
	{
	public:
		ImVec4 Color;
		TextStyles Style;
		char* Text;

	public:
		TextData(const ImVec4& color, TextStyles textStyle, char* text) : Color(color), Style(textStyle), Text(text) {}
		~TextData() {}

	public:
		void FreeData()
		{
			if (Text)
			{
				free(Text);
			}

			Text = nullptr;
		}

		TextData operator=(TextData textData)
		{
			Color = textData.Color;
			Style = textData.Style;
			Text = textData.Text;
			return *this;
		}
	};

private:
	static inline size_t MaxHistory;
	static inline std::vector<char*> AutoCompletes;
	static inline std::vector<TextData> ConsoleText;
	static inline std::vector<char*> UserHistory;
	static inline std::vector<char*> MapCompletes;
	static inline std::vector<char*> KeyCompletes;

private:
	char InputBuffer[512];
	int32_t HistoryPos;
	ImGuiInputTextFlags InputFlags;
	ImGuiTextFilter Filter;
	int32_t CandidatePos;
	ImArgumentIds ArgumentType;
	std::vector<std::pair<const char*, bool>> Candidates;
	bool AutoScroll;
	bool ScrollToBottom;
	bool HomoMode;

public:
	ImConsole(const std::string& title, const std::string& name, bool bShowCursor = true);
	~ImConsole() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	void ToggleHomosexual();
	static void ClearAutoCompletes();
	static void ClearConsole();
	static void ClearHistory();
	static void ClearMapCompletes();
	static void ClearKeyCompletes();
	static void SetHistorySize(size_t newSize);
	static void AddAutoComplete(const std::string& command);
	static void SetMapCompletes(const std::vector<std::string>& maps);
	void ResetAutoComplete();
	void AddText(TextColors color, TextStyles style, const char* fmt, ...) IM_FMTARGS(2);
	void ExecuteCommand(const char* commandText, TextColors color, TextStyles style);
	void CommandDelegate(const std::string& text, TextColors color, TextStyles style);
	int32_t TextEditCallback(ImGuiInputTextCallbackData* data);
};

class ImFunctionScanner : public ImInterface
{
private:
	class FunctionData
	{
	public:
		char* Full;
		char* Package;
		char* Caller;
		char* Function;

	public:
		FunctionData(char* full, char* package, char* caller, char* function) : Full(full), Package(package), Caller(caller), Function(function) {}
		~FunctionData() {}

	public:
		void FreeData()
		{
			if (Full) { free(Full); }
			if (Package) { free(Package); }
			if (Caller) { free(Caller); }
			if (Function) { free(Function); }
		}
	};

private:
	std::string ClipboardText;
	ImGuiTableFlags TableFlags;
	std::vector<FunctionData> FunctionHistory;
	ImGuiTextFilter Whitelist;
	ImGuiTextFilter Blacklist;
	static inline bool ScanFunctions;

public:
	ImFunctionScanner(const std::string& title, const std::string& name, bool bShowCursor = true);
	~ImFunctionScanner() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	static bool IsScanning();
	void SaveToFile();
	bool PassesFilter(const char* textToFilter);
	void OnProcessEvent(class UObject* caller, class UFunction* function);
};