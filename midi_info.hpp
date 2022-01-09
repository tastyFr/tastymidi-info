#ifndef MIDI_INFO_HPP
#define MIDI_INFO_HPP

#include "midi/MidiFile.h"
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

using entry = std::pair<uint32_t, const char*>;

class MidiInfo
{
private:
    static std::string getMidiDuration(
        const std::vector<entry>& entries) noexcept;
    static std::vector<entry> toCompound(uint32_t seconds) noexcept;

    std::string getTrackText() const noexcept;
    std::string getBpmText() const noexcept;

public:
    bool readMidiFile(std::string filename);
    void setTrackText() noexcept;
    void setBpmText() noexcept;
    std::string getJsonOutput() const noexcept;

    MidiInfo() noexcept;
    ~MidiInfo() noexcept = default;

private:
    uint32_t m_emptyTracksCount;
    uint32_t m_totalNotes;
    uint32_t m_tpqn;
    std::string m_trackCountText;
    std::string m_bpmText;
    std::string m_durationText;
    std::vector<uint16_t> m_bpms;

    MidiFile m_midiFile;
};

#endif // MIDI_INFO_HPP