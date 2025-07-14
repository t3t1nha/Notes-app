#include "NoteApp.hpp"

#include "imgui.h"
#include <iostream>
#include <fstream>
#include <limits>

// JSON serialization must be outside class as free functions:

void to_json(json& j, const NoteApp::Note& note) {
    j = json{{"title", note.title}, {"description", note.description}};
}

void from_json(const json& j, NoteApp::Note& note) {
    j.at("title").get_to(note.title);
    j.at("description").get_to(note.description);
}

// ----------------- NoteApp method implementations ---------------------

void NoteApp::render() {
    // List notes with selectable titles
    for (int i = 0; i < (int)NOTES.size(); i++) {
        ImGui::PushID(i);
        if (ImGui::Selectable(NOTES[i].title.c_str(), i == selected_note)) {
            selected_note = i;
            is_editing = false;
        }
        ImGui::PopID();
    }

    ImGui::Separator();

    if (ImGui::Button("Add Note")) {
        NOTES.push_back({"New Title", "New Description"});
        selected_note = (int)NOTES.size() - 1;
        is_editing = true;
    }

    ImGui::SameLine();

    if (selected_note >= 0 && selected_note < (int)NOTES.size()) {
        if (ImGui::Button("Edit Note")) {
            is_editing = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete Note")) {
            NOTES.erase(NOTES.begin() + selected_note);
            selected_note = -1;
            is_editing = false;
        }
    }

    if (is_editing && selected_note >= 0 && selected_note < (int)NOTES.size()) {
        // Temporary buffers for ImGui InputText (fixed max size)
        static char title_buf[256];
        static char desc_buf[1024];

        // Initialize buffers with current values only once on entering edit mode
        static bool init_buffers = false;
        if (!init_buffers) {
            strncpy(title_buf, NOTES[selected_note].title.c_str(), sizeof(title_buf));
            strncpy(desc_buf, NOTES[selected_note].description.c_str(), sizeof(desc_buf));
            init_buffers = true;
        }

        ImGui::InputText("Title", title_buf, sizeof(title_buf));
        ImGui::InputTextMultiline("Description", desc_buf, sizeof(desc_buf), ImVec2(-FLT_MIN, 100));

        if (ImGui::Button("Save")) {
            NOTES[selected_note].title = std::string(title_buf);
            NOTES[selected_note].description = std::string(desc_buf);
            is_editing = false;
            init_buffers = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            is_editing = false;
            init_buffers = false;
        }
    }
}

void NoteApp::SaveNotes(const std::vector<Note>& notes, const std::string& filename) {
    json j = notes;
    std::ofstream file(filename);
    file << j.dump(4);
}

std::vector<NoteApp::Note> NoteApp::LoadNotes(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    json j;
    try {
        file >> j;
        return j.get<std::vector<Note>>();
    } catch (...) {
        std::cerr << "Error reading JSON. Starting with empty notes.\n";
        return {};
    }
}

void NoteApp::FillNote(Note &note) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter title: ";
    std::getline(std::cin, note.title);
    std::cout << "Enter description: ";
    std::getline(std::cin, note.description);
}

void NoteApp::AddNote() {
    Note newNote;
    FillNote(newNote);

    NOTES.push_back(newNote);
}

void NoteApp::ShowNotes() {
    for (int i = 0; i < (int)NOTES.size(); i++) {
        std::cout << "Note " << i + 1 << "\n";
        std::cout << "Title: " << NOTES[i].title << "\n";
        std::cout << "Description: " << NOTES[i].description << "\n\n";
    }
}

void NoteApp::EditNote() {
    int n;
    std::cout << "What note would you like to edit: ";
    std::cin >> n;
    if (n > 0 && n <= (int)NOTES.size())
        FillNote(NOTES[n - 1]);
}

void NoteApp::DeleteNote() {
    int n;
    std::cout << "What note would you like to remove: ";
    std::cin >> n;
    if (n > 0 && n <= (int)NOTES.size())
        NOTES.erase(NOTES.begin() + (n - 1));
}
