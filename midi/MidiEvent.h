//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Feb 14 21:47:39 PST 2015
// Last Modified: Sat Feb 14 21:54:52 PST 2015
// Filename:      midifile/include/MidiEvent.h
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   A class which stores a MidiMessage and a timestamp
//                for the MidiFile class.
//

#ifndef _MIDIEVENT_H_INCLUDED
#define _MIDIEVENT_H_INCLUDED

#include "MidiMessage.h"
#include <vector>

using namespace std;

class MidiEvent : public MidiMessage
{
public:
	MidiEvent(void);
	MidiEvent(int command);
	MidiEvent(int command, int param1);
	MidiEvent(int command, int param1, int param2);
	MidiEvent(int aTime, int aTrack, vector<uchar> &message);
	MidiEvent(const MidiMessage &message);
	MidiEvent(const MidiEvent &mfevent);

	~MidiEvent();

	MidiEvent &operator=(const MidiEvent &mfevent);
	MidiEvent &operator=(const MidiMessage &message);
	MidiEvent &operator=(const vector<uchar> &bytes);
	MidiEvent &operator=(const vector<char> &bytes);
	MidiEvent &operator=(const vector<int> &bytes);
	void clearVariables(void);

	// functions related to event linking (note-ons to note-offs).
	void unlinkEvent(void);
	void unlinkEvents(void);
	void linkEvent(MidiEvent *mev);
	void linkEvents(MidiEvent *mev);
	void linkEvent(MidiEvent &mev);
	void linkEvents(MidiEvent &mev);
	int isLinked(void);
	MidiEvent *getLinkedEvent(void);
	int getTickDuration(void);
	double getDurationInSeconds(void);

	int tick;		// delta or absolute MIDI ticks
	int track;		// [original] track number of event in MIDI file
	double seconds; // calculated time in sec. (doTimeAnalysis())
	int seq;		// sorting sequence number of event

private:
	MidiEvent *eventlink; // used to match note-ons and note-offs
};

#endif /* _MIDIEVENT_H_INCLUDED */
