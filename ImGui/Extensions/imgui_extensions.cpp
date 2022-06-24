#include "imgui_extensions.h"

namespace ImExtensions
{
	int32_t Stricmp(const char* s1, const char* s2)
	{
		int32_t d;

		while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
		{
			s1++;
			s2++;
		}

		return d;
	}

	int32_t Strnicmp(const char* s1, const char* s2, int32_t n)
	{
		int32_t d = 0;

		while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
		{
			s1++;
			s2++;
			n--;
		}

		return d;
	}

	char* StrCpy(const char* s)
	{
		IM_ASSERT(s);
		size_t length = strlen(s) + 1;
		void* buffer = malloc(length);
		IM_ASSERT(buffer);
		memcpy_s(buffer, length, s, length);
		return reinterpret_cast<char*>(buffer);
	}

	void Strtrim(char* s)
	{
		char* strEnd = s + strlen(s);

		while (strEnd > s && strEnd[-1] == ' ')
		{
			strEnd--;
			*strEnd = 0;
		}
	}

	void Text(const std::string& text)
	{
		ImGui::TextEx(text.c_str());
	}

	void TextColored(const std::string& text, const ImVec4& textColor)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		Text(text);
		ImGui::PopStyleColor();
	}

	void TextStyled(const std::string& text, const ImVec4& textColor, ImFont* textFont)
	{
		if (textFont) { ImGui::PushFont(textFont); }
		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		Text(text);
		ImGui::PopStyleColor();
		if (textFont) { ImGui::PopFont(); }
	}

	void TextStyled(const std::string& text, ImFont* textFont)
	{
		if (textFont) { ImGui::PushFont(textFont); }
		Text(text);
		if (textFont) { ImGui::PopFont(); }
	}

	void HelpMarker(const std::string& desc, float posScale)
	{
		ImGuiContext& g = *GImGui;
		ImGui::PushStyleColor(ImGuiCol_Text, g.Style.Colors[ImGuiCol_TextDisabled]);
		Text("(?)");
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * posScale);
			Text(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}