#pragma once

#include "json.hpp"
#include <vector>
#include <string>

using json = nlohmann::json;

class NoteApp {
public:
    struct Note {
        std::string title;
        std::string description;
    };

    std::vector<Note> NOTES;

    int selected_note = -1;
    bool is_editing = false;

    void render();

    void AddNote();
    void ShowNotes();
    void EditNote();
    void DeleteNote();

    void SaveNotes(const std::vector<Note>& notes, const std::string& filename);
    std::vector<Note> LoadNotes(const std::string& filename);

private:
    void FillNote(Note &note);
};
