#include "midi_info.hpp"
#include "json.hpp"

MidiInfo::MidiInfo() noexcept
    : m_emptyTracksCount(0), m_totalNotes(0), m_tpqn(0), m_bpms({})
{}

auto MidiInfo::readMidiFile(std::string filename) -> bool
{
    filename.erase(
        std::remove(filename.begin(), filename.end(), '"'), filename.end());

    m_midiFile.read(filename.c_str());
    if(m_midiFile.status() == 0)
    {
        return false;
    }

    m_midiFile.deltaTicks();

    m_tpqn = m_midiFile.getTicksPerQuarterNote();

    m_durationText = getMidiDuration(
        toCompound(static_cast<uint32_t>(m_midiFile.getTotalTimeInSeconds())));

    return true;
}

auto MidiInfo::getMidiDuration(const std::vector<entry>& entries) noexcept
    -> std::string
{
    std::string result;

    for(const auto& entry : entries)
    {
        result += std::to_string(entry.first) + entry.second;

        if(entry != entries.back())
        {
            result += " ";
        }
    }

    return result;
}

auto MidiInfo::toCompound(uint32_t seconds) noexcept -> std::vector<entry>
{
    constexpr uint32_t WEEK{7U * 24U * 60U * 60U};
    constexpr uint32_t DAY{24U * 60U * 60U};
    constexpr uint32_t HOUR{60U * 60U};
    constexpr uint32_t MINUTE{60U};
    constexpr uint32_t SECOND{1U};

    static constexpr std::array<entry, 5> timeTable{{
        {WEEK, "w"},   //
        {DAY, "d"},    //
        {HOUR, "h"},   //
        {MINUTE, "m"}, //
        {SECOND, "s"}  //
    }};

    std::vector<entry> result;

    for(const auto& time : timeTable)
    {
        uint32_t timeValue{seconds / time.first};

        if(timeValue > 0U)
        {
            result.emplace_back(timeValue, time.second);
        }

        seconds %= time.first;
    }

    if(result.empty())
    {
        result.push_back(timeTable.back());
    }

    return result;
}

void MidiInfo::setTrackText() noexcept
{
    std::string text;

    if(m_midiFile.getTrackCount() == 0)
    {
        text = "(no tracks)";
    }
    else
    {
        for(int i{0}; i < m_midiFile.getTrackCount(); ++i)
        {
            if(m_midiFile.getEventCount(i) == 0)
            {
                ++m_emptyTracksCount;
            }
            else
            {
                for(int j{0}; j < m_midiFile.getEventCount(i); ++j)
                {
                    MidiEvent event{m_midiFile.getEvent(i, j)};

                    if(event.isTempo())
                    {
                        m_bpms.push_back(
                            static_cast<uint16_t>(event.getTempoBPM()));
                    }

                    if(event.isNoteOn())
                    {
                        ++m_totalNotes;
                    }
                }
            }
        }

        text = std::to_string(m_midiFile.getTrackCount()) + " track";
        if(m_midiFile.getTrackCount() > 1)
        {
            text += "s";
        }

        if(m_emptyTracksCount > 0)
        {
            text += " (" + std::to_string(m_emptyTracksCount) + " empty)";
        }
    }

    m_trackCountText = text;
}

void MidiInfo::setBpmText() noexcept
{
    if(m_bpms.empty())
    {
        m_bpmText = "(no tempo events)";
        return;
    }

    std::string temp;

    if(m_bpms.size() > 1)
    {
        const auto minmax = std::minmax_element(m_bpms.begin(), m_bpms.end());

        if(*minmax.first == *minmax.second)
        {
            temp = std::to_string(*minmax.first);
        }
        else
        {
            temp = std::to_string(*minmax.first) + "-" +
                   std::to_string(*minmax.second);
        }

        if(std::adjacent_find(m_bpms.begin(), m_bpms.end()) != m_bpms.end())
        {
            temp += " (stacked)";
        }
    }
    else
    {
        temp = std::to_string(m_bpms.front());
    }

    m_bpmText = temp;
}

auto MidiInfo::getJsonOutput() const noexcept -> std::string
{
    nlohmann::json json;

    json["Ticks Per Quarter Note"] = m_tpqn;
    json["Tempo"]                  = m_bpmText;
    json["MIDI Duration"]          = m_durationText;
    json["Number of Tracks"]       = m_trackCountText;
    json["Total Notes"]            = m_totalNotes;

    return json.dump(2);
}