#include "JsonHistoryRepository.hpp"

#include <fstream>
#include <iostream>

namespace noonoo {

// --- JSON helpers (format-aware, not a general parser) ---

static std::string jsonEscape(const std::string& s)
{
    std::string out;
    for (char c : s) {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else                out += c;
    }
    return out;
}

static std::string jsonUnescape(const std::string& s)
{
    std::string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            if      (s[i+1] == '"')  { out += '"';  i++; }
            else if (s[i+1] == '\\') { out += '\\'; i++; }
            else                       out += s[i];
        } else {
            out += s[i];
        }
    }
    return out;
}

static std::string extractString(const std::string& line, const std::string& key)
{
    std::string search = "\"" + key + "\":\"";
    auto pos = line.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = line.find('"', pos);
    if (end == std::string::npos) return "";
    return jsonUnescape(line.substr(pos, end - pos));
}

static int extractInt(const std::string& line, const std::string& key)
{
    std::string search = "\"" + key + "\":";
    auto pos = line.find(search);
    if (pos == std::string::npos) return 0;
    pos += search.size();
    size_t end = pos;
    while (end < line.size() && line[end] != ',' && line[end] != '}' && line[end] != ']') end++;
    try { return std::stoi(line.substr(pos, end - pos)); }
    catch (...) { return 0; }
}

static bool extractBool(const std::string& line, const std::string& key)
{
    std::string search = "\"" + key + "\":";
    auto pos = line.find(search);
    if (pos == std::string::npos) return false;
    pos += search.size();
    return line.substr(pos, 4) == "true";
}

// parses all question objects from the "sq":[...] portion of a session line
static std::vector<QuestionRecord> extractQuestions(const std::string& line, Difficulty d)
{
    std::vector<QuestionRecord> questions;
    auto arrayStart = line.find("\"sq\":[");
    if (arrayStart == std::string::npos) return questions;
    arrayStart += 6;  // skip past "sq":[

    size_t pos = arrayStart;
    while ((pos = line.find("{\"q\":\"", pos)) != std::string::npos) {
        auto end = line.find('}', pos);
        if (end == std::string::npos) break;
        std::string obj = line.substr(pos, end - pos + 1);
        QuestionRecord r;
        r.questionText  = extractString(obj, "q");
        r.correctAnswer = extractInt(obj, "ca");
        r.userAnswer    = extractInt(obj, "ua");
        r.wasCorrect    = extractBool(obj, "ok");
        r.difficulty    = d;
        questions.push_back(r);
        pos = end + 1;
    }
    return questions;
}

// --- JsonHistoryRepository ---

JsonHistoryRepository::JsonHistoryRepository(const std::string& filePath)
    : _filePath(filePath)
{}

void JsonHistoryRepository::BeginSession(Difficulty d)
{
    _current = GameSession{ d, 0, 0, {} };
}

void JsonHistoryRepository::RecordQuestion(const QuestionRecord& q)
{
    if (_current) _current->questions.push_back(q);
}

void JsonHistoryRepository::EndSession(int score, int total)
{
    if (!_current) return;
    _current->score = score;
    _current->total = total;
    _sessions.push_back(*_current);
    _current.reset();
    Save();
}

const std::vector<GameSession>& JsonHistoryRepository::GetAllSessions() const
{
    return _sessions;
}

int JsonHistoryRepository::GetTotalCorrect() const
{
    int total = 0;
    for (const auto& s : _sessions)
        total += s.score;
    return total;
}

void JsonHistoryRepository::Load()
{
    std::ifstream in(_filePath);
    if (!in.is_open()) return;  // no history file yet — start fresh

    _sessions.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.find("\"sd\":") == std::string::npos) continue;
        GameSession s;
        s.difficulty = static_cast<Difficulty>(extractInt(line, "sd"));
        s.score      = extractInt(line, "sc");
        s.total      = extractInt(line, "st");
        s.questions  = extractQuestions(line, s.difficulty);
        _sessions.push_back(s);
    }
}

void JsonHistoryRepository::Save() const
{
    std::ofstream out(_filePath);
    if (!out.is_open()) {
        std::cerr << "noonoo: failed to open history file for writing: " << _filePath << "\n";
        return;
    }

    for (const auto& s : _sessions) {
        out << "{\"sd\":" << static_cast<int>(s.difficulty)
            << ",\"sc\":" << s.score
            << ",\"st\":" << s.total
            << ",\"sq\":[";
        for (size_t j = 0; j < s.questions.size(); j++) {
            const auto& q = s.questions[j];
            out << "{\"q\":\""  << jsonEscape(q.questionText) << "\""
                << ",\"ca\":"   << q.correctAnswer
                << ",\"ua\":"   << q.userAnswer
                << ",\"ok\":"   << (q.wasCorrect ? "true" : "false")
                << "}";
            if (j + 1 < s.questions.size()) out << ",";
        }
        out << "]}\n";
    }
}

} // namespace noonoo
