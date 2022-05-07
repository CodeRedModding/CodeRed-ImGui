#include "CodeRed.hpp"

namespace ImClasses
{
	TextData::TextData(const std::string& text, TextColors textColor, TextStyles textStyle) : Text(text), Color(ImColorMap[textColor]), Style(textStyle) {}

	TextData::~TextData() {}

	TextData& TextData::operator=(const TextData& textData)
	{
		Text = textData.Text;
		Color = textData.Color;
		Style = textData.Style;
		return *this;
	}

	QueueData::QueueData(const std::string& text, TextColors textColor, TextStyles textStyle) : TextData(text, textColor, textStyle), Id(textColor) {}

	QueueData::~QueueData() {}

	QueueData& QueueData::operator=(const QueueData& queueData)
	{
		Text = queueData.Text;
		Color = queueData.Color;
		Style = queueData.Style;
		Id = queueData.Id;
		return *this;
	}

	FunctionCount::FunctionCount() : Calls(0) {}

	FunctionCount::~FunctionCount() {}

	FunctionData::FunctionData() :
		FullName("null"),
		Package("null"),
		Caller("null"),
		Function("null"),
		Index(0)
	{

	}

	FunctionData::FunctionData(const FunctionData& functionData) :
		FullName(functionData.FullName),
		Package(functionData.Package),
		Caller(functionData.Caller),
		Function(functionData.Function),
		Index(functionData.Index)
	{

	}

	FunctionData::FunctionData(class UObject* caller, class UFunction* function) :
		FullName("null"),
		Package("null"),
		Caller("null"),
		Function("null"),
		Index(0)
	{
		// Requires and actual SDK for your game, so that's why this is commented out.

		//if (caller && function)
		//{
		//	FullName = function->GetFullName();
		//	Package = function->GetPackageObj()->GetName();
		//	Caller = caller->GetName();
		//	Function = function->GetName();
		//	Index = function->ObjectInternalInteger;
		//}
	}

	FunctionData::FunctionData(const std::string& fullName, const std::string& package, const std::string& caller, const std::string& function, int32_t index) :
		FullName(fullName),
		Package(package),
		Caller(caller),
		Function(function),
		Index(index)
	{

	}

	FunctionData::~FunctionData() {}

	FunctionData& FunctionData::operator=(const FunctionData& functionData)
	{
		FullName = functionData.FullName;
		Package = functionData.Package;
		Caller = functionData.Caller;
		Function = functionData.Function;
		Index = functionData.Index;
		return *this;
	}
}

ImInterface::ImInterface(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor) :
	WindowTitle(title),
	ShowCursor(bShowCursor),
	WindowName(name),
	Attached(false),
	ToggleCallback(toggleCallback),
	Render(false),
	Focused(false)
{

}

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

bool ImInterface::ShouldRender()
{
	if (!Render)
	{
		SetShouldRender(false);
	}

	return (IsAttached() && Render);
}

bool* ImInterface::ShouldBegin()
{
	return &Render;
}

bool ImInterface::IsFocused() const
{
	return Focused;
}

void ImInterface::SetAttached(bool bAttached)
{
	Attached = bAttached;
}

void ImInterface::SetShouldRender(bool bShouldRender)
{
	Render = bShouldRender;

	if (ToggleCallback)
	{
		ToggleCallback(WindowName, Render);
	}
}

void ImInterface::SetIsFocused(bool bFocused)
{
	Focused = bFocused;
}

void ImInterface::ToggleRender()
{
	SetShouldRender(!Render);
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

ImDemo::ImDemo(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor) : ImInterface(title, name, toggleCallback, bShowCursor) {}

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

ImFunctionScanner::ImFunctionScanner(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor) : ImInterface(title, name, toggleCallback, bShowCursor) {}

ImFunctionScanner::~ImFunctionScanner()
{
	OnDetatch();
}

void ImFunctionScanner::OnAttach()
{
	TableFlags = (ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY);
	ScanFunctions = false;
	SetAttached(true);
}

void ImFunctionScanner::OnDetatch()
{
	if (IsAttached())
	{
		SetAttached(false);
	}
}

void ImFunctionScanner::OnRender()
{
	if (ShouldRender())
	{
		std::string newTitle = GetTitle();

		if (!FunctionHistory.empty())
		{
			newTitle += (" - " + (HideDuplicates ? std::to_string(FunctionMap.size()) : std::to_string(FunctionHistory.size())) + " Functions###FunctionScanner");
		}
		else
		{
			newTitle += "###FunctionScanner";
		}

		ImGui::SetNextWindowSize(ImVec2(500.0f, 625.0f), ImGuiCond_Once);

		if (ImGui::Begin(newTitle.c_str(), ShouldBegin(), ImGuiWindowFlags_NoNav))
		{
			SetIsFocused(ImGui::IsWindowFocused());

			if (ImGui::BeginTable("###Scanner_Filter", 2))
			{
				ImGui::TableSetupColumn("Text", ImGuiTableColumnFlags_WidthFixed, 50.0f);
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextUnformatted("Whitelist");
					ImGui::TableSetColumnIndex(1);
					Whitelist.Draw(" Filter: (\"incl\") (\"error\")###Scanner_Whitelist");
				}

				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextUnformatted("Blacklist");
					ImGui::TableSetColumnIndex(1);
					Blacklist.Draw(" Filter: (\"excl\") (\"error\")###Scanner_Blacklist");
				}

				ImGui::EndTable();
			}

			ImGui::Spacing();

			if (IsScanning()) { if (ImGui::Button("Stop Monitoring")) { ScanFunctions = false; } }
			else { if (ImGui::Button("Start Monitoring")) { ScanFunctions = true; } }

			ImGui::SameLine(); if (ImGui::Button("Clear Table")) { FunctionHistory.clear(); }
			ImGui::SameLine(); if (ImGui::Button("Save to File")) { SaveToFile(); }
			ImGui::SameLine(); ImGui::Checkbox("Hide duplicates###Scanner_Hide_Dupes", &HideDuplicates);
			ImGui::Spacing();

			if (HideDuplicates)
			{
				if (ImGui::BeginTable("###FunctionScanner_Duplicate_Table", 4, TableFlags))
				{
					bool copy_to_clipboard = false;

					if (ImGui::BeginPopupContextWindow())
					{
						copy_to_clipboard = ImGui::Selectable("Copy to Clipboard");
						ImGui::EndPopup();
					}

					ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthFixed, 20.0f);
					ImGui::TableSetupColumn("Package Object", ImGuiTableColumnFlags_WidthFixed, 100.0f);
					ImGui::TableSetupColumn("Calling Class", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Function Name", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					for (const auto& dataIt : FunctionMap)
					{
						const ImClasses::FunctionData& data = dataIt.second;

						if (PassesFilter(data.FullName))
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::Text("%d", data.Calls);
							ImGui::TableSetColumnIndex(1);
							ImGui::TextUnformatted(data.Package.c_str());
							ImGui::TableSetColumnIndex(2);
							ImGui::TextUnformatted(data.Caller.c_str());
							ImGui::TableSetColumnIndex(3);
							ImGui::TextUnformatted(data.Function.c_str());

							if (copy_to_clipboard)
							{
								ClipboardText += data.FullName;
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
				if (ImGui::BeginTable("###FunctionScanner_Default_Table", 3, TableFlags))
				{
					bool copy_to_clipboard = false;

					if (ImGui::BeginPopupContextWindow())
					{
						copy_to_clipboard = ImGui::Selectable("Copy to Clipboard");
						ImGui::EndPopup();
					}

					ImGui::TableSetupColumn("Package Object", ImGuiTableColumnFlags_WidthFixed, 100.0f);
					ImGui::TableSetupColumn("Calling Class", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Function Name", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					for (size_t i = 0; i < FunctionHistory.size(); i++)
					{
						const ImClasses::FunctionData& data = FunctionHistory[i];

						if (PassesFilter(data.FullName))
						{
							ImGui::TableNextRow();
							ImGui::TableSetColumnIndex(0);
							ImGui::TextUnformatted(data.Package.c_str());
							ImGui::TableSetColumnIndex(1);
							ImGui::TextUnformatted(data.Caller.c_str());
							ImGui::TableSetColumnIndex(2);
							ImGui::TextUnformatted(data.Function.c_str());

							if (copy_to_clipboard)
							{
								ClipboardText += data.FullName;
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

			ImGui::End();
		}
		else
		{
			SetShouldRender(false);
			ScanFunctions = false;
		}
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
	if (!FunctionHistory.empty())
	{
		std::ofstream functionTable("FunctionDump_" + std::to_string(std::time(nullptr)) + ".txt");

		if (HideDuplicates)
		{
			for (const auto& functionData : FunctionMap)
			{
				functionTable << functionData.second.FullName << std::endl;
			}
		}
		else
		{
			for (const auto& functionData : FunctionHistory)
			{
				functionTable << functionData.FullName << std::endl;
			}
		}

		functionTable.close();
	}
}

void ImFunctionScanner::ClearTable()
{
	FunctionMap.clear();
	FunctionHistory.clear();
}

bool ImFunctionScanner::PassesFilter(const std::string& textToFilter)
{
	const char* filterText = textToFilter.c_str();

	if (Whitelist.IsActive())
	{
		if (Whitelist.PassFilter(filterText))
		{
			if (Blacklist.IsActive())
			{
				return (!Blacklist.PassFilter(filterText));
			}

			return true;
		}

		return false;
	}

	if (Blacklist.IsActive())
	{
		if (!Blacklist.PassFilter(filterText))
		{
			if (Whitelist.IsActive())
			{
				return (Whitelist.PassFilter(filterText));
			}

			return true;
		}

		return false;
	}

	return true;
}

ImNotification::ImNotification(const std::string& title, const std::string& name) : ImInterface(title, name, NULL, false) {}

ImNotification::~ImNotification() { OnDetatch(); }

void ImNotification::OnAttach()
{
	WindowPadding = 10.0f;
	WindowOffset = 0.0f;
	Corner = CornerPositions::TopLeft;
	WindowFlags = (ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs);
	DescriptionColor = ImColorMap[TextColors::White];
	DeltaTime = 0.0f;
	CutoffTime = 300.0f;
	BaseAlpha = 0.85f;
	LastAlpha = BaseAlpha;
	FadeOut = false;
	SetAttached(true);
}

void ImNotification::OnDetatch()
{
	if (IsAttached())
	{
		SetAttached(false);
	}
}

void ImNotification::OnRender()
{
	if (ShouldRender())
	{
		if (!FadeOut && (DeltaTime > CutoffTime))
		{
			DeltaTime = 0.0f;
			LastAlpha = BaseAlpha;
			TitleColor.w = LastAlpha;
			DescriptionColor.w = LastAlpha;
			FadeOut = true;
		}
		else if (FadeOut && (LastAlpha < 0.0f))
		{
			DeltaTime = 0.0f;
			LastAlpha = BaseAlpha;
			TitleColor.w = LastAlpha;
			DescriptionColor.w = LastAlpha;
			FadeOut = false;
			SetShouldRender(false);
			return;
		}

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos;
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos;
		ImVec2 window_pos_pivot;

		window_pos.x = (Corner == CornerPositions::TopRight || Corner == CornerPositions::BottomRight) ? (work_pos.x + work_size.x - WindowPadding) : (work_pos.x + WindowPadding);
		window_pos.y = (Corner == CornerPositions::BottomRight || Corner == CornerPositions::BottomLeft) ? (work_pos.y + work_size.y - WindowPadding) : (work_pos.y + WindowPadding);
		window_pos_pivot.x = (Corner == CornerPositions::TopRight || Corner == CornerPositions::BottomRight) ? 1.0f : 0.0f;
		window_pos_pivot.y = (Corner == CornerPositions::BottomRight || Corner == CornerPositions::BottomLeft) ? 1.0f : 0.0f;
		
		if (Corner == CornerPositions::TopLeft) { window_pos.y += WindowOffset; }
		else if (Corner == CornerPositions::TopRight) { window_pos.y += WindowOffset; }
		else if (Corner == CornerPositions::BottomLeft) { window_pos.y -= WindowOffset; }
		else if (Corner == CornerPositions::BottomRight) { window_pos.y -= WindowOffset; }

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowSize(ImVec2(325.0f, 75.0f), ImGuiCond_Always);

		if (FadeOut)
		{
			LastAlpha -= (GetRenderTime() / 100.0f);
			ImGui::SetNextWindowBgAlpha(LastAlpha);
		}
		else
		{
			ImGui::SetNextWindowBgAlpha(BaseAlpha);
		}

		if (ImGui::Begin(GetTitle().c_str(), ShouldBegin(), WindowFlags))
		{
			SetIsFocused(ImGui::IsWindowFocused());

			if (ImGui::BeginChild(std::string("###" + GetName() + "_Notification_ScrollRegion").c_str()))
			{
				TitleColor.w = LastAlpha;
				DescriptionColor.w = LastAlpha;

				ImExtensions::TextStyled(Title.c_str(), TitleColor, ImFontMap[TextStyles::Bold]);
				ImGui::PushTextWrapPos(ImGui::GetWindowWidth() - 1.0f);
				ImExtensions::TextStyled(Description.c_str(), DescriptionColor, ImFontMap[TextStyles::Regular]);
				ImGui::PopTextWrapPos();
			}

			DeltaTime += GetRenderTime();
			ImGui::End();
		}
	}
}

float ImNotification::GetRenderTime() const
{
	float framerate = ImGui::GetIO().Framerate;
	if (framerate <= 15) { framerate = 4.0f; }
	if (framerate <= 30) { framerate = 2.0f; }
	else if (framerate <= 60) { framerate = 1.0f; }
	else if (framerate <= 120) { framerate = 0.5f; }
	else if (framerate <= 144) { framerate = 0.4167f; }
	else if (framerate <= 240) { framerate = 0.25f; }
	else if (framerate <= 360) { framerate = 0.1667f; }
	else if (framerate <= 480) { framerate = 0.125f; }
	else if (framerate > 480) { framerate = 0.1f; }
	return framerate;
}

ImNotification* ImNotification::SetInformation(const std::string& title, const std::string& description, TextColors color, CornerPositions corner)
{
	SetTitle(title);
	SetDescription(description);
	SetColor(color);
	SetPosition(corner);
	return this;
}

ImNotification* ImNotification::SetTitle(const std::string& title)
{
	Title = title;
	return this;
}

ImNotification* ImNotification::SetDescription(const std::string& description)
{
	Description = description;
	return this;
}

ImNotification* ImNotification::SetColor(TextColors color)
{
	TitleColor = ImColorMap[color];
	TitleColor.w = LastAlpha;
	return this;
}

ImNotification* ImNotification::SetPosition(CornerPositions corner)
{
	Corner = corner;
	return this;
}

ImNotification* ImNotification::SetOffset(float offset)
{
	WindowOffset = offset;
	return this;
}

ImNotificationManager::ImNotificationManager(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback) : ImInterface(title, name, toggleCallback, false) {}

ImNotificationManager::~ImNotificationManager() { OnDetatch(); }

void ImNotificationManager::OnAttach()
{
	// Create notifications here for them to be properly managed, rendering for them is called in the managers "OnRender" function.
	CreateNotification(new ImNotification("Example Notification", "example_notification"))->SetInformation("This is an example", "This is an example description!", TextColors::Green);
	SetAttached(true);
}

void ImNotificationManager::OnDetatch()
{
	if (IsAttached())
	{
		ActiveNotifications.clear();

		for (auto& notification : CreatedNotifications)
		{
			if (notification.second)
			{
				notification.second->OnDetatch();
			}
		}

		CreatedNotifications.clear();
		SetAttached(false);
	}
}

void ImNotificationManager::OnRender()
{
	if (!ActiveNotifications.empty())
	{
		for (auto notificationIt = ActiveNotifications.begin(); notificationIt != ActiveNotifications.end();)
		{
			if (*notificationIt && !(*notificationIt)->ShouldRender())
			{
				notificationIt = ActiveNotifications.erase(notificationIt);
			}
			else
			{
				notificationIt++;
			}
		}
	}

	for (const auto& notification : CreatedNotifications)
	{
		if (notification.second && notification.second->ShouldRender())
		{
			bool alreadyActive = false;

			for (const auto& activeNotification : ActiveNotifications)
			{
				if (activeNotification == notification.second)
				{
					alreadyActive = true;
					break;
				}
			}

			if (!alreadyActive)
			{
				ActiveNotifications.push_back(notification.second);
			}
		}
	}

	if (!ActiveNotifications.empty())
	{
		for (size_t i = 0; i < ActiveNotifications.size(); i++)
		{
			std::shared_ptr<ImNotification> notification = ActiveNotifications[i];

			if (notification)
			{
				float offset = 0.0f;

				if (i != 0)
				{
					offset = (i * 85.0f); // 85 being the notification's window height (75) plus padding (10).
				}

				notification->SetOffset(offset);
				notification->OnRender();
			}
		}
	}
}

void ImNotificationManager::ToggleNotification(const std::string& windowName)
{
	std::shared_ptr<ImNotification> notification = GetNotification(windowName);

	if (notification)
	{
		notification->ToggleRender();
	}
}

std::shared_ptr<ImNotification> ImNotificationManager::GetNotification(const std::string& windowName)
{
	if (CreatedNotifications.find(windowName) != CreatedNotifications.end())
	{
		return CreatedNotifications[windowName];
	}

	return nullptr;
}

std::shared_ptr<ImNotification> ImNotificationManager::CreateNotification(ImNotification* notification)
{
	if (notification)
	{
		std::string notificationName = notification->GetName();

		if (CreatedNotifications.find(notificationName) == CreatedNotifications.end())
		{
			CreatedNotifications.emplace(notificationName, std::shared_ptr<ImNotification>(notification)).first->second->OnAttach();
			return CreatedNotifications[notificationName];
		}
	}

	return nullptr;
}

void ImFunctionScanner::OnProcessEvent(class UObject* caller, class UFunction* function)
{
	if (ShouldRender() && IsScanning() && caller && function)
	{
		// Requires and actual SDK for your game, so that's why this is commented out.
		// 
		//std::string fullName = function->GetFullName();

		//if (PassesFilter(fullName.c_str()))
		//{
		//	ImClasses::FunctionData newEntry(caller, function);
		//	FunctionHistory.push_back(newEntry);

		//	for (auto& data : FunctionMap)
		//	{
		//		if (data.first == newEntry.Index)
		//		{
		//			data.second.Calls++;
		//			return;
		//		}
		//	}

		//	FunctionMap.push_back(std::make_pair(newEntry.Index, newEntry));
		//}
	}
}

ImTerminal::ImTerminal(const std::string& title, const std::string& name, std::function<void(std::string, bool)> toggleCallback, bool bShowCursor) : ImInterface(title, name, toggleCallback, bShowCursor) {}

ImTerminal::~ImTerminal()
{
	OnDetatch();
}

void ImTerminal::OnAttach()
{
	MaxUserHistory = 64;
	MaxConsoleHistory = 256;
	HistoryPos = -1;
	InputFlags = (ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit);
	CandidatePos = 0;
	ArgumentType = ImArgumentIds::IM_None;
	AutoScroll = true;
	ScrollToBottom = false;
	memset(InputBuffer, 0, IM_ARRAYSIZE(InputBuffer));
	SetAttached(true);
}

void ImTerminal::OnDetatch()
{
	if (IsAttached())
	{
		ClearCommands();
		SetAttached(false);
	}
}

void ImTerminal::OnRender()
{
	if (ShouldRender())
	{
		ImGui::SetNextWindowSize(ImVec2(765.0f, 455.0f), ImGuiCond_Once);

		if (ImGui::Begin(GetTitle().c_str(), ShouldBegin(), ImGuiWindowFlags_NoNav))
		{
			SetIsFocused(ImGui::IsWindowFocused());

			bool copy_to_clipboard = false;
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

			if (ImGui::BeginChild("###Terminal_ScrollRegion", ImVec2(0.0f, -footer_height_to_reserve), false))
			{
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::BeginMenu("Filter Text###Terminal_FilterMenu"))
					{
						Filter.Draw("Filter: (\"incl,-excl\") (\"error\")###Terminal_Filter", (ImGui::GetWindowSize().x / 4.0f));
						ImGui::EndMenu();
					}

					ImGui::MenuItem("Auto Scroll", "", &AutoScroll);
					copy_to_clipboard = ImGui::Selectable("Copy to Clipboard");
					if (ImGui::Selectable("Clear Terminal###Terminal_ClearText")) { ConsoleText.clear(); }
					if (ImGui::Selectable("Clear History###Terminal_ClearHistory")) { UserHistory.clear(); }

					ImGui::EndPopup();
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 1.0f));
				if (copy_to_clipboard) { ImGui::LogToClipboard(); }

				for (size_t i = 0; i < ConsoleText.size(); i++)
				{
					const ImClasses::TextData& textData = ConsoleText[i];
					const char* itemText = textData.Text.c_str();
					if (!Filter.PassFilter(itemText)) { continue; }

					float fontSize = 1.0f;
					ImFont* font = ImFontMap[textData.Style];
					if (font) { fontSize = font->FontSize; }

					ImGui::PushTextWrapPos(ImGui::GetWindowWidth() - fontSize);
					ImExtensions::TextStyled(itemText, textData.Color, font);
					ImGui::PopTextWrapPos();
				}

				if (copy_to_clipboard) { ImGui::LogFinish(); }
				if (ScrollToBottom || (AutoScroll && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))) { ImGui::SetScrollHereY(1.0f); }

				ScrollToBottom = false;

				ImGui::PopStyleVar();
				ImGui::EndChild();

				bool reclaim_focus = false;

				ImGui::PushItemWidth((ImGui::GetWindowSize().x - (ImGui::GetStyle().WindowPadding.x * 2.0f)) - 55.0f);

				if (ImGui::InputText("###Terminal_InputText", InputBuffer, IM_ARRAYSIZE(InputBuffer), InputFlags, [](ImGuiInputTextCallbackData* data) -> int32_t { return reinterpret_cast<ImTerminal*>(data->UserData)->TextEditCallback(data); }, reinterpret_cast<void*>(this)))
				{
					char* bufferText = InputBuffer;
					ImExtensions::Strtrim(bufferText);

					if (bufferText[0]) { ExecuteCommand(bufferText); }

					strcpy_s(bufferText, sizeof(bufferText), "");
					reclaim_focus = true;
				}

				ImGui::SameLine(); if (ImGui::Button("Enter###Terminal_EnterButton"))
				{
					char* bufferText = InputBuffer;
					ImExtensions::Strtrim(bufferText);
					if (bufferText[0]) { ExecuteCommand(bufferText); }
					strcpy_s(bufferText, sizeof(bufferText), "");
					reclaim_focus = true;
				}

				ImGui::PopItemWidth();

				if (std::strlen(InputBuffer) == 0) { ResetAutoComplete(); }

				if (!Candidates.empty() && ImGui::IsWindowFocused())
				{
					ImVec2 consolePos = ImGui::GetWindowPos();
					consolePos.y += ImGui::GetWindowHeight() + 5.0f;

					if (ArgumentType == ImArgumentIds::IM_Interfaces) { consolePos.x += 85.0f; }
					else if (ArgumentType == ImArgumentIds::IM_Example) { consolePos.x += 0.0f; } // Example, purpose of different commands having different auto-complete positions.

					ImGui::SetNextWindowPos(consolePos);

					if (!Candidates.empty())
					{
						ImGui::OpenPopup("###Terminal_CandidatesPopup");

						if (ImGui::BeginPopup("###Terminal_CandidatesPopup", ImGuiWindowFlags_NoFocusOnAppearing))
						{
							ImVec2 childSize(175.0f, 0.0f);

							for (size_t i = 0; i < Candidates.size(); i++)
							{
								childSize.y += 18.25f;

								if (childSize.y > 130.0f) // Cutoff point, if there are more than "i" candidates it will no longer grow in size.
								{
									childSize.y = 130.0f;
									break;
								}
							}

							if (ImGui::BeginChild("###Terminal_CandidatesChild", childSize))
							{
								for (size_t i = 0; i < Candidates.size(); i++)
								{
									const std::pair<std::string, bool>& candidate = Candidates[i];

									ImExtensions::TextStyled(
										Candidates[i].first.c_str(),
										(candidate.second ? ImColorMap[TextColors::White] : ImColorMap[TextColors::Grey]),
										(candidate.second ? ImFontMap[TextStyles::Italic] : nullptr
									));
								}

								ImGui::EndChild();
							}

							ImGui::EndPopup();
						}
					}
				}

				ImGui::SetItemDefaultFocus();
				if (reclaim_focus) { ImGui::SetKeyboardFocusHere(-1); }
			}

			ImGui::End();
		}
	}
}

void ImTerminal::SetHistorySize(size_t newSize)
{
	if (newSize <= 5120)
	{
		MaxConsoleHistory = newSize;
	}
}

void ImTerminal::AddCommand(const std::string& str)
{
	CommandCompletes.push_back(ImExtensions::StrCpy(str.c_str()));
}

void ImTerminal::RemoveCommand(const std::string& str)
{
	for (size_t i = 0; i < CommandCompletes.size(); i++)
	{
		if (std::string(CommandCompletes[i]) == str)
		{
			free(CommandCompletes[i]);
			CommandCompletes[i] = nullptr;
			break;
		}
	}

	for (auto completeIt = CommandCompletes.begin(); completeIt != CommandCompletes.end();)
	{
		if (*completeIt == nullptr)
		{
			completeIt = CommandCompletes.erase(completeIt);
		}
		else
		{
			completeIt++;
		}
	}
}

void ImTerminal::ClearCommands()
{
	for (size_t i = 0; i < CommandCompletes.size(); i++)
	{
		if (CommandCompletes[i])
		{
			free(CommandCompletes[i]);
		}
	}

	CommandCompletes.clear();
}

void ImTerminal::AddArgument(ImArgumentIds argumentId, const std::string& str)
{
	if (ArgumentCompletes.find(argumentId) != ArgumentCompletes.end())
	{
		ArgumentCompletes[argumentId].push_back(str);
	}
	else
	{
		ArgumentCompletes[argumentId] = std::vector<std::string>{ str };
	}
}

void ImTerminal::RemoveArgument(ImArgumentIds argumentId, const std::string& str)
{
	if (ArgumentCompletes.find(argumentId) != ArgumentCompletes.end())
	{
		std::vector<std::string>& arguments = ArgumentCompletes[argumentId];
		std::vector<std::string>::iterator argumentIt = std::find(arguments.begin(), arguments.end(), str);

		if (argumentIt != arguments.end())
		{
			arguments.erase(argumentIt);
		}
	}
}

void ImTerminal::AddDisplayText(const std::string& text, TextColors textColor, TextStyles textStyle)
{
	ImClasses::TextData newData(text, textColor, textStyle);

	if (ConsoleText.size() >= MaxConsoleHistory)
	{
		for (size_t i = 0; i < ConsoleText.size(); i++)
		{
			if (i == 0)
			{
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

void ImTerminal::AddDisplayText(const ImClasses::QueueData& queueData)
{
	AddDisplayText(queueData.Text, queueData.Id, queueData.Style);
}

void ImTerminal::ConsoleDelegate(const std::string& text, TextColors textColor, TextStyles textStyle)
{
	if (true) // In CodeRed this is where I check if DirectX has been hooked yet or not, if it isn't and this function is called I add it to a "queue" to display later on.
	{
		if (!ConsoleQueue.empty())
		{
			for (size_t i = 0; i < ConsoleQueue.size(); i++)
			{
				AddDisplayText(ConsoleQueue[i]);
			}

			ConsoleQueue.clear();
		}

		AddDisplayText(text, textColor, textStyle);
	}
	else
	{
		ConsoleQueue.push_back(ImClasses::QueueData(text, textColor, textStyle));
	}
}

void ImTerminal::ExecuteCommand(const std::string& command, TextStyles textStyle)
{
	ResetAutoComplete();
	AddDisplayText("# " + command, TextColors::Yellow, textStyle);

	HistoryPos = -1;

	if (UserHistory.size() >= MaxUserHistory)
	{
		for (size_t i = 0; i < UserHistory.size(); i++)
		{
			if (i == 0)
			{
				continue;
			}

			UserHistory[i - 1] = UserHistory[i];
		}

		UserHistory[ConsoleText.size() - 1] = command;
	}
	else
	{
		UserHistory.push_back(command);
	}

	// Here is where you would execute the command on a separate thread, as calling anything in the render thread will crash your game.
	// ExampleClass::ExecuteCommand(command);
	ScrollToBottom = true;
}

void ImTerminal::ResetAutoComplete()
{
	CandidatePos = 0;
	ArgumentType = ImArgumentIds::IM_None;
	Candidates.clear();
}

int32_t ImTerminal::TextEditCallback(ImGuiInputTextCallbackData* data)
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
				const char* history_str = (HistoryPos >= 0) ? UserHistory[HistoryPos].c_str() : "";
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
			data->InsertChars(data->CursorPos, Candidates[completePos].first.c_str());
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

					for (size_t i = 0; i < CommandCompletes.size(); i++)
					{
						if (CommandCompletes[i])
						{
							if (ImExtensions::Strnicmp(CommandCompletes[i], word_start, (int32_t)(word_end - word_start)) == 0)
							{
								Candidates.push_back(std::make_pair(CommandCompletes[i], first));
								first = false;
							}
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

					if (inputStr.find("imgui_toggle") == 0)
					{
						ArgumentType = ImArgumentIds::IM_Interfaces;
					}
					else
					{
						ArgumentType = ImArgumentIds::IM_None;
						break;
					}

					if (ArgumentCompletes.find(ArgumentType) != ArgumentCompletes.end())
					{
						bool first = true;

						const std::vector<std::string>& completes = ArgumentCompletes[ArgumentType];

						for (size_t i = 0; i < completes.size(); i++)
						{
							if (ImExtensions::Strnicmp(completes[i].c_str(), argument_start, (int32_t)(argument_end - argument_start)) == 0)
							{
								Candidates.push_back(std::make_pair(completes[i].c_str(), first));
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