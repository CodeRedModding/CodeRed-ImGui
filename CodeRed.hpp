#pragma once
#include "pch.hpp"

enum class ImArgumentIds : uint8_t
{
	IM_None,
	IM_Interfaces,
	IM_Example
};

enum class TextStyles : uint8_t
{
	Regular,
	Italic,
	Bold,
	END
};

enum class TextColors : uint32_t
{
	Black,
	Grey,
	White,
	Red,
	Orange,
	Yellow,
	Green,
	Blue,
	Aqua,
	Purple,
	Pink,
	END
};

enum class CornerPositions : uint8_t
{
	Custom,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

static std::map<TextColors, ImVec4> ImColorMap = {
	{ TextColors::Black, ImVec4(0.0f, 0.0f, 0.0f, 1.0f) },							// #000000
	{ TextColors::Grey, ImVec4(0.72549f, 0.72549f, 0.72549f, 1.0f) },				// #B9B9B9
	{ TextColors::White, ImVec4(1.0f, 1.0f, 1.0f, 1.0f) },							// #FFFFFF
	{ TextColors::Red, ImVec4(0.95686f, 0.10196f, 0.18431f, 1.0f) },				// #F41A2F
	{ TextColors::Orange, ImVec4(1.0f, 0.55686f, 0.0f, 1.0f) },						// #FF8E00
	{ TextColors::Yellow, ImVec4(0.99215f, 0.96078f, 0.0f, 1.0f) },					// #FDF500
	{ TextColors::Green, ImVec4(0.16470f, 0.99215f, 0.18039f, 1.0f) },				// #2AFD2E
	{ TextColors::Blue, ImVec4(0.12549f, 0.41568f, 1.0f, 1.0f) },					// #206AFF
	{ TextColors::Aqua, ImVec4(0.09019f, 0.90980f, 1.0f, 1.0f) },					// #17E8FF
	{ TextColors::Purple, ImVec4(0.64705f, 0.12941f, 1.0f, 1.0f) },					// #A521FF
	{ TextColors::Pink, ImVec4(1.0f, 0.12941, 1.0f, 1.0f) },						// #FF21FF
};

static std::map<TextStyles, ImFont*> ImFontMap = {
	{ TextStyles::Regular, nullptr },
	{ TextStyles::Italic, nullptr },
	{ TextStyles::Bold, nullptr }
};

namespace ImClasses
{
	class TextData
	{
	public:
		std::string Text;
		ImVec4 Color;
		TextStyles Style;

	public:
		TextData(const std::string& text, TextColors textColor, TextStyles textStyle);
		~TextData();

	public:
		TextData& operator=(const TextData& textData);
	};

	class QueueData : public TextData
	{
	public:
		TextColors Id;

	public:
		QueueData(const std::string& text, TextColors textColor, TextStyles textStyle);
		~QueueData();

	public:
		QueueData& operator=(const QueueData& queueData);
	};

	class FunctionCount
	{
	public:
		size_t Calls;

	public:
		FunctionCount();
		~FunctionCount();
	};

	class FunctionData : public FunctionCount
	{
	public:
		std::string FullName;
		std::string Package;
		std::string Caller;
		std::string Function;
		int32_t Index;

	public:
		FunctionData();
		FunctionData(const FunctionData& functionData);
		FunctionData(class UObject* caller, class UFunction* function);
		FunctionData(const std::string& fullName, const std::string& package, const std::string& caller, const std::string& function, int32_t index);
		~FunctionData();

	public:
		FunctionData& operator=(const FunctionData& functionData);
	};
}

// Base class for all ImGui interfaces, controls attaching/detaching as well as rendering.
class ImInterface
{
private:
	std::string WindowTitle; // Text to display inside its ImGui window title.
	std::string WindowName;	// Name used internally to call or manage in other places, must be unique.
	bool ShowCursor; // If you should forcefully show the cursor upon opening the window or not, true by default, should be managed by your WndProc hook.
	bool Attached; // If the interface is attached/created or not.
	bool Render; // If the window should be rendered or not inside ImGui.
	bool Focused; // If the window is focused or not inside ImGui.
	std::function<void(std::string, bool)> ToggleCallback; // Callback function that is called when the render status has changed (open/closing).

public:
	ImInterface(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor);
	virtual ~ImInterface();

public:
	const std::string& GetTitle() const;
	const std::string& GetName() const;
	bool ShouldShowCursor() const;
	bool IsAttached() const;
	bool ShouldRender();
	bool* ShouldBegin();
	bool IsFocused() const;
	void SetAttached(bool bAttached);
	void SetShouldRender(bool bShouldRender);
	void SetIsFocused(bool bFocused);
	void ToggleRender();

public:
	virtual void OnAttach(); // Should be called when the interface is first created and hooked to ImGui, do any initializing here like memset or assigning default values.
	virtual void OnDetatch(); // Can be called by the deconstructor or manually, if you have any pointers allocated you should free them here.
	virtual void OnRender(); // Called each render tick, depends on what version of DirectX you're using.
};

class ImDemo : public ImInterface
{
public:
	ImDemo(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor = true);
	~ImDemo() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;
};

class ImNotification : public ImInterface
{
private:
	float WindowPadding;
	float WindowOffset;
	ImGuiWindowFlags WindowFlags;

private:
	CornerPositions Corner;
	std::string Title;
	std::string Description;
	ImVec4 TitleColor;
	ImVec4 DescriptionColor;
	ImVec2 Size;

private:
	float DeltaTime;
	float CutoffTime;
	float BaseAlpha;
	float LastAlpha;
	bool FadeOut;

public:
	ImNotification(const std::string& title, const std::string& name, const ImVec2& size);
	~ImNotification() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	const ImVec2& GetSize() const;
	float GetRenderTime() const;
	ImNotification* SetInformation(const std::string& title, const std::string& description, TextColors color, CornerPositions corner = CornerPositions::TopLeft);
	ImNotification* SetTitle(const std::string& title);
	ImNotification* SetDescription(const std::string& description);
	ImNotification* SetColor(TextColors color);
	ImNotification* SetPosition(CornerPositions corner);
	ImNotification* SetOffset(float offset);
};

class ImNotificationManager : public ImInterface
{
private:
	static inline std::map<std::string, std::shared_ptr<ImNotification>> CreatedNotifications;
	static inline std::vector<std::shared_ptr<ImNotification>> ActiveNotifications;

public:
	ImNotificationManager(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback);
	~ImNotificationManager() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	static void ToggleNotification(const std::string& windowName);
	static std::shared_ptr<ImNotification> GetNotification(const std::string& windowName);
	static std::shared_ptr<ImNotification> CreateNotification(ImNotification* notification);
};

class ImFunctionScanner : public ImInterface
{
private:
	std::string ClipboardText;
	ImGuiTableFlags TableFlags;
	ImGuiTextFilter Whitelist;
	ImGuiTextFilter Blacklist;
	std::vector<ImClasses::FunctionData> FunctionHistory;
	std::vector<std::pair<int32_t, ImClasses::FunctionData>> FunctionMap;
	static inline bool ScanFunctions;
	static inline bool HideDuplicates;

public:
	ImFunctionScanner(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor = true);
	~ImFunctionScanner() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	static bool IsScanning();
	void SaveToFile();
	void ClearTable();
	bool PassesFilter(const std::string& textToFilter);
	void OnProcessEvent(class UObject* caller, class UFunction* function); // Here is where you could send ProcessEvent to from your game.
};

class ImTerminal : public ImInterface
{
private:
	static inline size_t MaxUserHistory;
	static inline size_t MaxConsoleHistory;
	static inline std::vector<ImClasses::TextData> ConsoleText;
	static inline std::vector<ImClasses::QueueData> ConsoleQueue;
	static inline std::vector<std::string> UserHistory;
	static inline std::vector<char*> CommandCompletes;
	static inline std::map<ImArgumentIds, std::vector<std::string>> ArgumentCompletes;

private:
	char InputBuffer[512];
	int32_t HistoryPos;
	ImGuiInputTextFlags InputFlags;
	ImGuiTextFilter Filter;
	int32_t CandidatePos;
	ImArgumentIds ArgumentType;
	std::vector<std::pair<std::string, bool>> Candidates;
	bool AutoScroll;
	bool ScrollToBottom;

public:
	ImTerminal(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor = true);
	~ImTerminal() override;

public:
	void OnAttach() override;
	void OnDetatch() override;
	void OnRender() override;

public:
	static void SetHistorySize(size_t newSize);
	static void AddCommand(const std::string& str);
	static void RemoveCommand(const std::string& str);
	static void ClearCommands();
	static void AddArgument(ImArgumentIds argumentId, const std::string& str);
	static void RemoveArgument(ImArgumentIds argumentId, const std::string& str);
	static void AddDisplayText(const std::string& text, TextColors textColor, TextStyles textStyle);
	static void AddDisplayText(const ImClasses::QueueData& queueData);
	static void ConsoleDelegate(const std::string& text, TextColors textColor, TextStyles textStyle);

public:
	void ExecuteCommand(const std::string& command, TextStyles textStyle = TextStyles::Regular);
	void ResetAutoComplete();
	int32_t TextEditCallback(struct ImGuiInputTextCallbackData* data);
};