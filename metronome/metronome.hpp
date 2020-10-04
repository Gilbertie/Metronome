#pragma once

#include <cstddef>
#include <deque>

using namespace std;

#define LED_RED   17
#define LED_GREEN 27
#define BTN_MODE  23
#define BTN_TAP   24
#define beat_samples 4

class metronome
{

public:
	metronome()
	{
        m_timing = false;
        deque <unsigned int> dq;
        bpm_min = 20;
        bpm_max = 300;
	}
	~metronome() {}

public:
	// Call when entering "learn" mode
	void start_timing()
	{
        // clear records
        while (dq.size() != 0)
        {
            dq.pop_front();
        }
        // then switch
        m_timing = true;
}

	// Call when leaving "learn" mode
	void stop_timing()
	{
        m_timing = false;
        if (dq.size() < beat_samples)
        {
            set_bpm(0);
            return;
        }
        set_bpm(60 * 1000 * (beat_samples - 1) / (dq.back() - dq.front()));
	}

	void printDeque() {
        deque<int>::iterator it;
        for (it = dq.begin(); it != dq.end(); it++)
        {
            cout << "\t" << *it;
        }
        cout << "\n";
	}

	void tap()
	{
        // record taps
        dq.push_back(millis());
        if (dq.size() > beat_samples) {
            dq.pop_front();
        }
        printDeque();
	}

	bool is_timing() { return m_timing; }

	int get_bpm() { return bpm;	}

	void set_bpm(int input)
	{
        if (input < bpm_min)
        {
            bpm = bpm_min;
        }
        else if (input > bpm_max)
        {
            bpm = bpm_max;
        }
        else
        {
            bpm = input;
        }
	}

	int get_bpm_max() { return bpm_max; }

	void reset_bpm_max() { bpm_max = INT_MAX; }

	int get_bpm_min() { return bpm_min; }

	void reset_bpm_min() { bpm_min = 0; }

public:
	bool m_timing;
	deque <int> dq;
	int bpm_min;
	int bpm_max;
	volatile int bpm;
};

