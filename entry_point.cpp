#include "midi_info.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    MidiInfo midiInfo;
    if(argc > 2)
    {
        std::cout << "Usage: midi_info <midi_file>" << std::endl;
    }
    else if(argc == 2)
    {
        if(midiInfo.readMidiFile(argv[1]))
        {
            midiInfo.setTrackText();
            midiInfo.setBpmText();
            std::cout << midiInfo.getJsonOutput() << std::endl;
        }
        else
        {
            std::cout << "Error reading file." << std::endl;
            return 1;
        }
    }
    else if(argc == 1)
    {
        std::cout << "Enter a path to a MIDI file: ";
        std::string filename;

        while(filename.empty())
        {
            std::getline(std::cin, filename);
        }

        if(midiInfo.readMidiFile(filename))
        {
            midiInfo.setTrackText();
            midiInfo.setBpmText();
            std::cout << midiInfo.getJsonOutput() << std::endl;
        }
        else
        {
            std::cout << "Error reading file." << std::endl;
            return 1;
        }
    }

    return 0;
}