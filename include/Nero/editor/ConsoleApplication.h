#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <imgui/imgui.h>
#include <ctype.h>
#include <stdio.h>

namespace nero
{
	struct ConsoleApplication
	{
		char                  InputBuf[256];
		ImVector<char*>       Items;
		ImVector<const char*> Commands;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImGuiTextFilter       Filter;
		bool                  AutoScroll;
		bool                  ScrollToBottom;

		ConsoleApplication()
		{
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;
			Commands.push_back("help");
			Commands.push_back("history");
			Commands.push_back("clear");
			AutoScroll = true;
			ScrollToBottom = true;
			AddLog("Engine Console");
		}

		~ConsoleApplication()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
			{
				free(History[i]);
			}
		}

		// Portable helpers
		static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
		static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
		static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
		static void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

		void    ClearLog()
		{
			for (int i = 0; i < Items.Size; i++)
			{
				free(Items[i]);
			}

			Items.clear();
			ScrollToBottom = true;
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf)-1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
			if (AutoScroll)
			{
				ScrollToBottom = true;
			}
		}

		void    Draw(const char* title, bool* p_open)
		{
			ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::End();
				return;
			}

			ImGui::SameLine(100.f);
			ImGui::TextWrapped("Enter 'help' for help, press TAB to use text completion.");
			// Command-line
			bool reclaim_focus = false;
			ImGui::Text("Command"); ImGui::SameLine(100.f);
			if (ImGui::InputText("##input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
			{
				char* s = InputBuf;
				Strtrim(s);
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			}

			ImGui::Dummy(ImVec2(0.f, 3.f));

			ImGui::Text("Filter"); ImGui::SameLine(100.f);
			Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180); ImGui::SameLine();
			if (ImGui::Button("Clear")) { ClearLog(); } ImGui::SameLine();
			bool copy_to_clipboard = ImGui::Button("Copy");

			ImGui::Dummy(ImVec2(0.f, 3.f));

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) ClearLog();
				ImGui::EndPopup();
			}

			// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
			// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
			// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
			// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
			//     ImGuiListClipper clipper(Items.Size);
			//     while (clipper.Step())
			//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			// However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
			// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
			// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
			// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
			if (copy_to_clipboard)
				ImGui::LogToClipboard();
			for (int i = 0; i < Items.Size; i++)
			{
				const char* item = Items[i];
				if (!Filter.PassFilter(item))
					continue;

				// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
				bool pop_color = false;
				if (strstr(item, "[error]"))            { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
				else if (strncmp(item, "# ", 2) == 0)   { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }
				ImGui::TextUnformatted(item);
				if (pop_color)
					ImGui::PopStyleColor();
			}
			if (copy_to_clipboard)
				ImGui::LogFinish();
			if (ScrollToBottom)
				ImGui::SetScrollHereY(1.0f);
			ScrollToBottom = false;
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::PopStyleColor();

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(0); // Auto focus previous widget

			ImGui::Dummy(ImVec2(5.f, 0.f));
			ImGui::End();
		}

		void    ExecCommand(const char* command_line)
		{
			AddLog("# %s\n", command_line);

			// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
			HistoryPos = -1;
			for (int i = History.Size-1; i >= 0; i--)
				if (Stricmp(History[i], command_line) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(command_line));

			// Process command
			if (Stricmp(command_line, "clear") == 0)
			{
				ClearLog();
			}
			else if (Stricmp(command_line, "help") == 0)
			{
				AddLog("Commands:");
				for (int i = 0; i < Commands.Size; i++)
					AddLog("- %s", Commands[i]);
			}
			else if (Stricmp(command_line, "history") == 0)
			{
				int first = History.Size - 10;
				for (int i = first > 0 ? first : 0; i < History.Size; i++)
					AddLog("%3d: %s\n", i, History[i]);
			}


			// On commad input, we scroll to bottom even if AutoScroll==false
			ScrollToBottom = true;
		}

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
		{
			ConsoleApplication* console = (ConsoleApplication*)data->UserData;
			return console->TextEditCallback(data);
		}

		int     TextEditCallback(ImGuiInputTextCallbackData* data)
		{
			//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
				{
					// Example of TEXT COMPLETION

					// Locate beginning of current word
					const char* word_end = data->Buf + data->CursorPos;
					const char* word_start = word_end;
					while (word_start > data->Buf)
					{
						const char c = word_start[-1];
						if (c == ' ' || c == '\t' || c == ',' || c == ';')
							break;
						word_start--;
					}

					// Build a list of candidates
					ImVector<const char*> candidates;
					for (int i = 0; i < Commands.Size; i++)
						if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
							candidates.push_back(Commands[i]);

					if (candidates.Size == 0)
					{
						// No match
						AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
					}
					else if (candidates.Size == 1)
					{
						// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
						data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
						data->InsertChars(data->CursorPos, candidates[0]);
						data->InsertChars(data->CursorPos, " ");
					}
					else
					{
						// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
						int match_len = (int)(word_end - word_start);
						for (;;)
						{
							int c = 0;
							bool all_candidates_matches = true;
							for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
								if (i == 0)
									c = toupper(candidates[i][match_len]);
								else if (c == 0 || c != toupper(candidates[i][match_len]))
									all_candidates_matches = false;
							if (!all_candidates_matches)
								break;
							match_len++;
						}

						if (match_len > 0)
						{
							data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
							data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
						}

						// List matches
						AddLog("Possible matches:\n");
						for (int i = 0; i < candidates.Size; i++)
							AddLog("- %s\n", candidates[i]);
					}

					break;
				}
			case ImGuiInputTextFlags_CallbackHistory:
				{
					// Example of HISTORY
					const int prev_history_pos = HistoryPos;
					if (data->EventKey == ImGuiKey_UpArrow)
					{
						if (HistoryPos == -1)
							HistoryPos = History.Size - 1;
						else if (HistoryPos > 0)
							HistoryPos--;
					}
					else if (data->EventKey == ImGuiKey_DownArrow)
					{
						if (HistoryPos != -1)
							if (++HistoryPos >= History.Size)
								HistoryPos = -1;
					}

					// A better implementation would preserve the data on the current input line along with cursor position.
					if (prev_history_pos != HistoryPos)
					{
						const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
						data->DeleteChars(0, data->BufTextLen);
						data->InsertChars(0, history_str);
					}
				}
			}
			return 0;
		}
	};

}
#endif // CONSOLEAPPLICATION_H
